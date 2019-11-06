#pragma once

#include "../software_gl/ImageInterface.h"
#include "../software_gl/VectorMath.h"
#include "../software_gl/Image.h"

class WindowSoftwareGL : public SoftwareGL::WindowInterface
{
public:
	WindowSoftwareGL(size_t width, size_t height) : 
		screen_width_(width), 
		screen_height_(height), 
		current_image_(width, height) {}

	bool Startup(const std::pair<int, int>& gl_version) override
	{
		glGenTextures(1, &texture_id_);
		return true;
	}

	bool RunCompute() override
	{
		//TODO Should clean to the correct ambient light.
		std::fill(
			current_image_.begin(), 
			current_image_.end(), 
			VectorMath::vector4(.2f, .0f, .2f, 1.f));
		static float time = 0;
		time += .01f;
		const float half_height = screen_height_ / 2.0f;
		const float half_width = screen_width_ / 2.0f;
		VectorMath::matrix rotation;
		rotation.IdentityMatrix();
		rotation.RotateZMatrix(time);
		VectorMath::matrix translation_center;
		translation_center.IdentityMatrix();
		translation_center.TranslateMatrix(-half_width, -half_height, 0);
		VectorMath::matrix translation_back;
		translation_back.IdentityMatrix();
		translation_back.TranslateMatrix(half_width, half_height, 0);
		// Create triangles.
		SoftwareGL::Vertex v[3] = { 
			SoftwareGL::Vertex(
				VectorMath::vector4(half_width, 20, 0, 1),
				VectorMath::vector4(1.0, 0.0, 0.0, 1.0)),
			SoftwareGL::Vertex(
				VectorMath::vector4(100, screen_height_ - 20, 0, 1), 
				VectorMath::vector4(0.0, 1.0, 0.0, 1.0)),
			SoftwareGL::Vertex(
				VectorMath::vector4(
					screen_width_ - 100, 
					screen_height_ - 20, 
					0, 
					1), 
				VectorMath::vector4(0.0, 0.0, 1.0, 1.0))
		};
		VectorMath::matrix view = 
			translation_center * rotation * translation_back;
		for (int i = 0; i < 3; ++i)
		{
			auto vec_res = VectorMath::VectorMult(
				VectorMath::vector4(v[i].GetX(), v[i].GetY(), 0, 1),
				view);
			v[i].SetPosition(VectorMath::vector4(vec_res.x, vec_res.y, 0, 1));
				
		}
		// Draw the triangle.
		current_image_.DrawTriangle(v[0], v[1], v[2]);
		// Change color to be white.
		v[0].SetColor(VectorMath::vector4(1, 1, 1, 1));
		v[1].SetColor(VectorMath::vector4(1, 1, 1, 1));
		v[2].SetColor(VectorMath::vector4(1, 1, 1, 1));
		// Draw the outline.
		current_image_.DrawLine(v[0], v[1]);
		current_image_.DrawLine(v[1], v[2]);
		current_image_.DrawLine(v[2], v[0]);
		// Copy the current image into the texture.
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			static_cast<GLsizei>(screen_width_),
			static_cast<GLsizei>(screen_height_),
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

	const std::pair<size_t, size_t> GetWindowSize() const override
	{
		return std::make_pair(screen_width_, screen_height_);
	}

protected:
	size_t screen_width_ = 640;
	size_t screen_height_ = 480;
	GLuint texture_id_ = 0;
	SoftwareGL::Image current_image_{screen_width_, screen_height_};
};
