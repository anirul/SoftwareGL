#pragma once

#include <gtest/gtest.h>
#include "../software_gl/Scene.h"

namespace test {
	
	class SceneTest : public testing::Test
	{
	public:
		SceneTest() {}
		// Populate the tree with:
		//	    Matrix (contain identity)
		//	    \--- Mesh   (contain CubeUVNormal.obj)
		//	    \--- Matrix (contain displacement matrix)
		//	         \--- Mesh   (contain TorusUVNormal.obj)
		void PopulateTree()
		{
			VectorMath::matrix identity =
			{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1,
			};
			auto matrix_scene = 
				std::make_shared<SoftwareGL::SceneMatrix>(identity);
			scene_tree_->AddNode(matrix_scene);
			{
				auto mesh = std::make_shared<OpenGL::Mesh>(
					"../asset/CubeUVNormal.obj");
				scene_tree_->AddNode(
					std::make_shared<SoftwareGL::SceneMesh>(mesh),
					matrix_scene);
			}
			{
				VectorMath::matrix disp =
				{
					1, 0, 0, 10,
					0, 1, 0, 10,
					0, 0, 1, 10,
					0, 0, 0, 1,
				};
				auto disp_scene = 
					std::make_shared<SoftwareGL::SceneMatrix>(disp);
				scene_tree_->AddNode(disp_scene, matrix_scene);
				{
					auto mesh = std::make_shared<OpenGL::Mesh>(
						"../asset/TorusUVNormal.obj");
					scene_tree_->AddNode(
						std::make_shared<SoftwareGL::SceneMesh>(mesh),
						disp_scene);
				}
			}
		}

	protected:
		std::shared_ptr<SoftwareGL::Scene> scene_ = nullptr;
		std::shared_ptr<SoftwareGL::SceneTree> scene_tree_ = nullptr;
	};

} // End namespace test.