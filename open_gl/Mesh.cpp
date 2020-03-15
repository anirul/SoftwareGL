#include "Mesh.h"
#include <iterator>
#include <gl/glew.h>
#include "../software_gl/Mesh.h"

namespace OpenGL {

	Mesh::Mesh(const std::string& file, bool test /*= false*/)
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
		if (!test)
		{
			// Position buffer initialization.
			glGenBuffers(1, &point_buffer_object_);
			glBindBuffer(GL_ARRAY_BUFFER, point_buffer_object_);
			glBufferData(
				GL_ARRAY_BUFFER,
				flat_positions_.size() * sizeof(float),
				flat_positions_.data(),
				GL_STATIC_DRAW);

			// Normal buffer initialization.
			glGenBuffers(1, &normal_buffer_object_);
			glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object_);
			glBufferData(
				GL_ARRAY_BUFFER,
				flat_normals_.size() * sizeof(float),
				flat_normals_.data(),
				GL_STATIC_DRAW);

			// Texture coordinates buffer initialization.
			glGenBuffers(1, &texture_buffer_object_);
			glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object_);
			glBufferData(
				GL_ARRAY_BUFFER,
				flat_textures_.size() * sizeof(float),
				flat_textures_.data(),
				GL_STATIC_DRAW);

			// Index buffer array.
			glGenBuffers(1, &index_buffer_object_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				flat_indices_.size() * sizeof(unsigned int),
				flat_indices_.data(),
				GL_STATIC_DRAW);
		}
		// Get the size of the indices.
		index_size_ = static_cast<GLsizei>(flat_indices_.size());
	}

	void Mesh::SetTexture(std::initializer_list<std::string> values)
	{
		textures_.clear();
		textures_.assign(values.begin(), values.end());
	}

} // End namespace OpenGL