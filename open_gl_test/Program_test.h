#pragma once

#include <gtest/gtest.h>
#include "OpenGL_test.h"
#include "../open_gl/Program.h"

namespace test {

	class ProgramTest : public OpenGLTest
	{
	public:
		ProgramTest() : OpenGLTest()
		{
			GLContextAndGlewInit();
		}

	protected:
		std::shared_ptr<OpenGL::Program> program_;
	};

} // End namespace test.
