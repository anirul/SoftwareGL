#pragma once

#include <vector>
#include <string>
#include <memory>
#include <initializer_list>
#include "../open_gl/Buffer.h"
#include "../open_gl/Program.h"
#include "../open_gl/Texture.h"

namespace OpenGL {

	class Mesh
	{
	public:
		Mesh(const std::string& file);
		virtual ~Mesh();

	public:
		const OpenGL::Buffer& PointBuffer() const;
		const OpenGL::Buffer& NormalBuffer() const;
		const OpenGL::Buffer& TextureBuffer() const;
		const OpenGL::Buffer& IndexBuffer() const;
		const size_t IndexSize() const { return index_size_; }
		void SetTexture(std::initializer_list<std::string> values);
		void Draw(
			const OpenGL::Program& program, 
			const OpenGL::TextureManager& texture_manager,
			const VectorMath::matrix& model = {}) const;
		
	private:
		std::vector<std::string> textures_ = {};
		OpenGL::Buffer point_buffer_ = {};
		OpenGL::Buffer normal_buffer_ = {};
		OpenGL::Buffer texture_buffer_ = {};
		OpenGL::Buffer index_buffer_ = 
			{ OpenGL::BufferType::ELEMENT_ARRAY_BUFFER };
		size_t index_size_ = 0;
		unsigned int vertex_array_object_ = 0;
	};

} // End namespace OpenGL