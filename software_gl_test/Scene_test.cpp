#include "Scene_test.h"

namespace test {

	TEST_F(SceneTest, CheckConstructorMatrixTest)
	{
		EXPECT_FALSE(scene_);
		VectorMath::matrix test;
		scene_ = std::make_shared<SoftwareGL::SceneMatrix>(test);
		EXPECT_TRUE(scene_);
	}

	TEST_F(SceneTest, CheckConstructorMeshTest)
	{
		EXPECT_FALSE(scene_);
		auto mesh = std::make_shared<OpenGL::Mesh>(
			"../asset/CubeUVNormal.obj");
		scene_ = std::make_shared<SoftwareGL::SceneMesh>(mesh);
		EXPECT_TRUE(scene_);
	}

	// Simple test scene with populate tree.
	TEST_F(SceneTest, CheckTreeConstructTest)
	{
		EXPECT_FALSE(scene_tree_);
		scene_tree_ = std::make_shared<SoftwareGL::SceneTree>();
		EXPECT_TRUE(scene_tree_);
		PopulateTree();
		unsigned int count_mesh = 0;
		unsigned int count_matrix = 0;
		for (const auto& scene : *scene_tree_)
		{
			const auto& value = scene->GetLocalModelAndMesh();
			if (value.second)
			{
				count_mesh++;
			}
			else
			{
				count_matrix++;
			}
		}
		EXPECT_EQ(2, count_mesh);
		EXPECT_EQ(2, count_matrix);
		EXPECT_EQ(4, scene_tree_->size());
	}

} // End namespace test.