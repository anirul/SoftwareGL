#pragma once

#include <string>
#include <vector>
#include <utility>
#include "../software_gl/VectorMath.h"

namespace OpenGL {

	class Texture {
	public:
		Texture(const std::string& file);
		virtual ~Texture();
		void Bind(const unsigned int slot = 0) const;
		void UnBind() const;
		int GetId() const { return texture_id_; }
		std::pair<size_t, size_t> GetSize() const { return size_; }

	private:
		unsigned int texture_id_ = 0;
		std::pair<size_t, size_t> size_ = { 0, 0 };
	};

} // End namespace OpenGL.