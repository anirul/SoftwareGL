#pragma once

#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <gl/glew.h>
#include "OpenGL_test.h"
#include "../open_gl/Mesh.h"

namespace test {

	class MeshTest : public OpenGLTest
	{
	public:
		MeshTest() : OpenGLTest() 
		{
			GLContextAndGlewInit();
		}

	protected:
		std::shared_ptr<OpenGL::Mesh> mesh_ = nullptr;
	};

} // End namespace test.
