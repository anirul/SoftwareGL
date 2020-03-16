#include "Buffer_test.h"

namespace test {

	TEST_F(BufferTest, CreationBufferTest)
	{
		EXPECT_FALSE(buffer_);
		buffer_ = std::make_shared<OpenGL::Buffer>();
		EXPECT_TRUE(buffer_);
	}

} // End namespace test.
