#pragma once

#include "../software_gl/ImageInterface.h"
#include "../software_gl/VectorMath.h"

class WindowSoftwareGL : public SoftwareGL::WindowInterface
{
public:
	bool Startup(const std::pair<int, int>& gl_version) override
	{
		glGenTextures(1, &texture_id_);
		current_image_.assign(
			screen_width_ * screen_height_, 
			{ 0.2f, 0.0f, 0.2f, 1.0f });
		return true;
	}

	bool RunCompute() override
	{
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
	GLuint screen_width_ = 1280;
	GLuint screen_height_ = 720;
	GLuint texture_id_ = 0;
	std::vector<VectorMath::vector> current_image_;
};
