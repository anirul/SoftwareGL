#include "Device_test.h"

namespace test {

	TEST_F(DeviceTest, CreateDeviceTest)
	{
		EXPECT_FALSE(device_);
		device_ = std::make_shared<OpenGL::Device>(nullptr);
		EXPECT_TRUE(device_);
	}

} // End namespace test.
