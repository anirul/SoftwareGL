#pragma once

#include <gtest/gtest.h>
#include "OpenGL_test.h"
#include "../open_gl/Device.h"

namespace test {

	class DeviceTest : public OpenGLTest
	{
	public:
		DeviceTest() : OpenGLTest() {}

	protected:
		std::shared_ptr<OpenGL::Device> device_ = nullptr;
	};

} // End namespace test.
