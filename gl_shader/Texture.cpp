#include "Texture.h"
#include <assert.h>
#include <GL/glew.h>
#include "../software_gl/Image.h"

namespace OpenGL {

	Texture::Texture(const std::string& file)
	{
		SoftwareGL::Image img{};
		img.LoadFromTGA(file);
		size_ = img.GetSize();
		assert(img.size() % size_.first == 0);
		glGenTextures(1, &texture_id_);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			static_cast<GLsizei>(size_.first),
			static_cast<GLsizei>(size_.second),
			0,
			GL_RGBA,
			GL_FLOAT,
			img.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture::~Texture() 
	{
		glDeleteTextures(1, &texture_id_);
	}

	void Texture::Bind(const unsigned int slot /*= 0*/) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
	}

	void Texture::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

} // End namespace OpenGL.