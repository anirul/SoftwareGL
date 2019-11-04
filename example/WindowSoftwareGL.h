#pragma once

#include "../software_gl/ImageInterface.h"
#include "../software_gl/VectorMath.h"
#include "../software_gl/Image.h"

class WindowSoftwareGL : public SoftwareGL::WindowInterface
{
public:
	bool Startup(const std::pair<int, int>& gl_version) override
	{
		glGenTextures(1, &texture_id_);
		return true;
	}

	bool RunCompute() override
	{
		SoftwareGL::Vertex v[3] = { 
			SoftwareGL::Vertex(
				VectorMath::vector2(320, 20), 
				VectorMath::vector4(1.0, 0.0, 0.0, 1.0)),
			SoftwareGL::Vertex(
				VectorMath::vector2(100, 460), 
				VectorMath::vector4(0.0, 1.0, 0.0, 1.0)),
			SoftwareGL::Vertex(
				VectorMath::vector2(540, 460), 
				VectorMath::vector4(0.0, 0.0, 1.0, 1.0))
		};
		current_image_.DrawTriangle(v[0], v[1], v[2]);
		// Copy the current image into the texture.
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			screen_width_,
			screen_height_,
			0,
			GL_RGBA,
			GL_FLOAT,
			&current_image_[0]);
		// Copy it to the 1 quad on the screen.
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		{
			glPushMatrix();
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2f(-1, 1);
				glTexCoord2f(1, 0);
				glVertex2f(1, 1);
				glTexCoord2f(1, 1);
				glVertex2f(1, -1);
				glTexCoord2f(0, 1);
				glVertex2f(-1, -1);
			}
			glEnd();
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
		glFlush();
		return true;
	}

	bool RunEvent(const SDL_Event& event) override
	{
		if (event.type == SDL_QUIT)
		{
			return false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return false;
			}
		}
		return true;
	}

	void Cleanup() override {}

protected:
	GLuint screen_width_ = 640;
	GLuint screen_height_ = 480;
	GLuint texture_id_ = 0;
	SoftwareGL::Image current_image_{screen_width_, screen_height_};
};
