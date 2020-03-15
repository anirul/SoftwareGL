#include "Scene.h"
#include <stdexcept>

namespace SoftwareGL {

	void SceneMatrix::SetParent(const std::shared_ptr<Scene>& parent)
	{
		parent_ = parent;
	}

	const model_mesh SceneMatrix::GetLocalModelAndMesh() const
	{
		if (parent_)
		{
			// CHECKME: multiplication is in right order?
			return { parent_->GetLocalModelAndMesh().first * matrix_, nullptr };
		}
		else
		{
			return { matrix_, nullptr };
		}
	}

	void SceneMesh::SetParent(const std::shared_ptr<Scene>& parent)
	{
		parent_ = parent;
	}

	const model_mesh SceneMesh::GetLocalModelAndMesh() const
	{
		VectorMath::matrix ret = {};
		if (parent_)
		{
			ret = parent_->GetLocalModelAndMesh().first;
		}
		return { ret, mesh_ };
	}

	bool Scene::operator!=(std::shared_ptr<Scene> scene) const
	{
		return scene.get() != this;
	}

	void SceneTree::AddNode(
		const std::shared_ptr<Scene>& node, 
		const std::shared_ptr<Scene>& parent /*= nullptr*/)
	{
		node->SetParent(parent);
		push_back(node);
	}

} // End namespace SoftwareGL.