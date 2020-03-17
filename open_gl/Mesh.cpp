#include "Mesh.h"
#include <iterator>
#include <gl/glew.h>
#include "../software_gl/Mesh.h"

namespace OpenGL {

	Mesh::Mesh(const std::string& file)
	{
		SoftwareGL::Mesh mesh{};
		mesh.LoadFromObj(file);
		std::vector<float> flat_positions_ = {};
		std::vector<float> flat_normals_ = {};
		std::vector<float> flat_textures_ = {};
		std::vector<unsigned int> flat_indices_ = {};
		std::vector<SoftwareGL::Vertex> vertices;
		for (const SoftwareGL::Triangle& triangle : mesh)
		{
			auto lambda = [&vertices](const SoftwareGL::Vertex& v)
			{
				auto next =
					static_cast<unsigned int>(vertices.size());
				auto it = std::find(vertices.begin(), vertices.end(), v);
				if (it == vertices.end())
				{
					vertices.push_back(v);
				}
				else
				{
					next =
						static_cast<unsigned int>(
							std::distance(vertices.begin(), it));
				}
				return next;
			};
			flat_indices_.push_back(lambda(triangle.GetV1()));
			flat_indices_.push_back(lambda(triangle.GetV2()));
			flat_indices_.push_back(lambda(triangle.GetV3()));
		}
		for (const SoftwareGL::Vertex& vertex : vertices)
		{
			auto point = vertex.GetPosition();
			auto normal = vertex.GetNormal();
			auto texel = vertex.GetTexture();
			flat_positions_.push_back(point.x);
			flat_positions_.push_back(point.y);
			flat_positions_.push_back(point.z);
			flat_normals_.push_back(normal.x);
			flat_normals_.push_back(normal.y);
			flat_normals_.push_back(normal.z);
			flat_textures_.push_back(texel.x);
			flat_textures_.push_back(texel.y);
		}
		// Position buffer initialization.
		point_buffer_.BindCopy(
			flat_positions_.size() * sizeof(float),
			flat_positions_.data());

		// Normal buffer initialization.
		normal_buffer_.BindCopy(
			flat_normals_.size() * sizeof(float),
			flat_normals_.data());
		
		// Texture coordinates buffer initialization.
		texture_buffer_.BindCopy(
			flat_textures_.size() * sizeof(float),
			flat_textures_.data());

		// Index buffer array.
		index_buffer_.BindCopy(
			flat_indices_.size() * sizeof(unsigned int),
			flat_indices_.data());

		// Get the size of the indices.
		index_size_ = static_cast<GLsizei>(flat_indices_.size());
	}

	const OpenGL::Buffer& Mesh::PointBuffer() const
	{
		return point_buffer_;
	}

	const OpenGL::Buffer& Mesh::NormalBuffer() const
	{
		return normal_buffer_;
	}

	const OpenGL::Buffer& Mesh::TextureBuffer() const
	{
		return texture_buffer_;
	}

	const OpenGL::Buffer& Mesh::IndexBuffer() const
	{
		return index_buffer_;
	}

	void Mesh::SetTexture(std::initializer_list<std::string> values)
	{
		textures_.clear();
		textures_.assign(values.begin(), values.end());
	}

} // End namespace OpenGL