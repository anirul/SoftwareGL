#pragma once

#include <gtest/gtest.h>
#include "../open_gl/Mesh.h"

namespace test {

	class MeshTest : public testing::Test 
	{
	public:
		MeshTest() {}

	protected:
		std::shared_ptr<OpenGL::Mesh> mesh_ = nullptr;
	};

} // End namespace test.
