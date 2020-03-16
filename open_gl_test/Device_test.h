#pragma once

#include <gtest/gtest.h>
#include "../open_gl/Device.h"

namespace test {

	class DeviceTest : public testing::Test 
	{
	public:
		DeviceTest() = default;

	protected:
		std::shared_ptr<OpenGL::Device> device_ = nullptr;
	};

} // End namespace test.
