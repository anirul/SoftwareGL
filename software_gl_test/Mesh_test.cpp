#include "Mesh_test.h"

namespace test {

	TEST_F(MeshTest, CreateCubeMeshTest)
	{
		EXPECT_FALSE(mesh_);
		mesh_ = std::make_shared<OpenGL::Mesh>(
			"../asset/CubeUVNormal.obj", 
			true);
		EXPECT_TRUE(mesh_);
	}

	TEST_F(MeshTest, CreateTorusMeshTest)
	{
		EXPECT_FALSE(mesh_);
		mesh_ = std::make_shared<OpenGL::Mesh>(
			"../asset/TorusUVNormal.obj",
			true);
		EXPECT_TRUE(mesh_);
	}

} // End namespace test.