#include "Scene.h"

namespace SoftwareGL {

	void SceneMatrix::AddChild(std::shared_ptr<Scene> child)
	{
		children_.push_back(child);
	}

	const model_mesh SceneMatrix::GetLocalModelAndMesh() const
	{
		// CHECKME: multiplication is in right order?
		return { parent_->GetLocalModelAndMesh().first * matrix_, nullptr };
	}

	std::shared_ptr<const Scene> SceneMatrix::Next(
		std::shared_ptr<const Scene> self /*= nullptr*/) const
	{
		if (self == nullptr)
		{
			const std::shared_ptr<const SceneMatrix> ptr(this);
			return parent_->Next(ptr);
		}
		auto it = std::find(children_.begin(), children_.end(), self);
		if (it != children_.end())
		{
			++it;
			if (it != children_.end())
			{
				return *it;
			}
		}
		if (parent_)
		{
			std::shared_ptr<const SceneMatrix> ptr(this);
			return parent_->Next(ptr);
		}
		return nullptr;
	}

	Scene::ConstIterator SceneMatrix::begin() const
	{
		std::shared_ptr<const SceneMatrix> ptr(this);
		return Scene::ConstIterator(ptr);
	}

	const model_mesh SceneMesh::GetLocalModelAndMesh() const
	{
		return { parent_->GetLocalModelAndMesh().first, mesh_ };
	}

	std::shared_ptr<const Scene> SceneMesh::Next(
		std::shared_ptr<const Scene> self /*= nullptr*/) const
	{
		std::shared_ptr<const SceneMesh> ptr(this);
		return parent_->Next(ptr);
	}

	SoftwareGL::Scene::ConstIterator SceneMesh::begin() const
	{
		std::shared_ptr<const SceneMesh> ptr(this);
		return Scene::ConstIterator(ptr);
	}

	Scene::ConstIterator::ConstIterator(std::shared_ptr<const Scene> scene) :
		scene_(scene) {}

	const model_mesh Scene::ConstIterator::operator*() const
	{
		return scene_->GetLocalModelAndMesh();
	}

	Scene::ConstIterator Scene::ConstIterator::operator++()
	{
		auto next_scene = scene_->Next(scene_);
		while (!next_scene->IsLeaf())
		{
			next_scene = next_scene->Next(scene_);
		}
		return ConstIterator(next_scene);
	}

	bool Scene::ConstIterator::operator!=(const ConstIterator& it) const
	{
		return scene_ != it.scene_;
	}

	bool Scene::operator!=(std::shared_ptr<Scene> scene) const
	{
		return scene.get() != this;
	}

	SoftwareGL::Scene::ConstIterator Scene::end() const
	{
		return ConstIterator(nullptr);
	}

} // End namespace SoftwareGL.