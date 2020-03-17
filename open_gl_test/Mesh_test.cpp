#include "Mesh_test.h"

namespace test {

	TEST_F(MeshTest, CreateCubeMeshTest)
	{
		EXPECT_FALSE(mesh_);
		mesh_ = std::make_shared<OpenGL::Mesh>("../asset/CubeUVNormal.obj");
		EXPECT_NE(0, mesh_->PointBuffer().GetId());
		EXPECT_NE(0, mesh_->NormalBuffer().GetId());
		EXPECT_NE(0, mesh_->TextureBuffer().GetId());
		EXPECT_NE(0, mesh_->IndexBuffer().GetId());
		EXPECT_EQ(18, mesh_->IndexSize());
		EXPECT_TRUE(mesh_);
	}

	TEST_F(MeshTest, CreateTorusMeshTest)
	{
		EXPECT_FALSE(mesh_);
		mesh_ = std::make_shared<OpenGL::Mesh>("../asset/TorusUVNormal.obj");
		EXPECT_NE(0, mesh_->PointBuffer().GetId());
		EXPECT_NE(0, mesh_->NormalBuffer().GetId());
		EXPECT_NE(0, mesh_->TextureBuffer().GetId());
		EXPECT_NE(0, mesh_->IndexBuffer().GetId());
		EXPECT_EQ(3456, mesh_->IndexSize());
		EXPECT_TRUE(mesh_);
	}

} // End namespace test.
