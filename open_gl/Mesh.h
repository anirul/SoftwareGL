#pragma once

#include <vector>
#include <string>
#include <memory>
#include <initializer_list>
#include "../open_gl/Buffer.h"

namespace OpenGL {

	class Mesh
	{
	public:
		Mesh(const std::string& file);
		virtual ~Mesh() = default;

	public:
		const OpenGL::Buffer& PointBuffer() const;
		const OpenGL::Buffer& NormalBuffer() const;
		const OpenGL::Buffer& TextureBuffer() const;
		const OpenGL::Buffer& IndexBuffer() const;
		const size_t IndexSize() const { return index_size_; }
		const std::vector<std::string> GetTextures() const { return textures_; }
		void SetTexture(std::initializer_list<std::string> values);

	private:
		std::vector<std::string> textures_ = {};
		OpenGL::Buffer point_buffer_ = {};
		OpenGL::Buffer normal_buffer_ = {};
		OpenGL::Buffer texture_buffer_ = {};
		OpenGL::Buffer index_buffer_ = 
			{ OpenGL::BufferType::ELEMENT_ARRAY_BUFFER };
		size_t index_size_ = 0;
	};

} // End namespace OpenGL