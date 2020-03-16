#pragma once

#include <gtest/gtest.h>
#include "../open_gl/Buffer.h"

namespace test {

	class BufferTest : public testing::Test
	{
	public:
		BufferTest() = default;

	protected:
		std::shared_ptr<OpenGL::Buffer> buffer_ = nullptr;
	};

} // End namespace test.
