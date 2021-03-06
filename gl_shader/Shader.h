#pragma once

#include <string>
#include <GL/glew.h>

namespace OpenGL {

	class Shader {
	public:
		Shader(unsigned int type) : type_(type) {}
		virtual ~Shader();
		bool LoadFromSource(const std::string& source);
		bool LoadFromFile(const std::string& path);
		unsigned int GetId() const { return id_; }
		const std::string GetErrorMessage() const { return error_message_; }

	private:
		bool created_ = false;
		unsigned int id_ = 0;
		unsigned int type_ = GL_VERTEX_SHADER;
		std::string error_message_;
	};

}	// End namespace OpenGL.
