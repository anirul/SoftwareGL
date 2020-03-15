#pragma once

#include <gtest/gtest.h>
#include "../open_gl/Mesh.h"

namespace test {

	class MeshTest : public testing::Test
	{
	public:
		MeshTest() = default;

	protected:
		std::shared_ptr<OpenGL::Mesh> mesh_;
	};

} // End namespace test.