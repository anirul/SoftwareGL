#pragma once

#include <vector>
#include <string>
#include <initializer_list>

namespace OpenGL {

	class Mesh
	{
	public:
		Mesh(const std::string& file, bool test = false);

	public:
		unsigned int PointObject() const { return point_buffer_object_; }
		unsigned int NormalObject() const { return normal_buffer_object_; }
		unsigned int TextureObject() const { return texture_buffer_object_; }
		unsigned int IndexObject() const { return index_buffer_object_; }
		size_t IndexSize() const { return index_size_; }
		const std::vector<std::string> GetTextures() const { return textures_; }
		void SetTexture(std::initializer_list<std::string> values);

	private:
		std::vector<std::string> textures_;
		unsigned int point_buffer_object_;
		unsigned int normal_buffer_object_;
		unsigned int texture_buffer_object_;
		unsigned int index_buffer_object_;
		size_t index_size_ = 0;
	};

} // End namespace OpenGL