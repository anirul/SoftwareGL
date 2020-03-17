#pragma once

#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <gl/glew.h>
#include "OpenGL_test.h"
#include "../open_gl/Buffer.h"

namespace test {

	class BufferTest : public OpenGLTest
	{
	public:
		BufferTest() : OpenGLTest() 
		{
			GLContextAndGlewInit();
		}

	protected:
		std::shared_ptr<OpenGL::Buffer> buffer_ = nullptr;
	};

} // End namespace test.
