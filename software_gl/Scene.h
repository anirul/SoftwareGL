#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "../software_gl/VectorMath.h"
#include "../open_gl/Mesh.h"

namespace SoftwareGL {

	using model_mesh = 
		std::pair<
			const VectorMath::matrix, 
			const std::shared_ptr<OpenGL::Mesh>>;

	struct Scene 
	{
		virtual void SetParent(const std::shared_ptr<Scene>& parent) = 0;
		virtual const model_mesh GetLocalModelAndMesh() const = 0;
		virtual bool IsLeaf() const = 0;
		virtual bool IsRoot() const = 0;
		bool operator!=(std::shared_ptr<Scene> scene) const;
	};

	class SceneMatrix : public Scene 
	{
	public:
		SceneMatrix(const VectorMath::matrix& matrix) : matrix_(matrix) {}
		void SetParent(const std::shared_ptr<Scene>& parent) override;
		const model_mesh GetLocalModelAndMesh() const override;
		bool IsLeaf() const override { return false; }
		bool IsRoot() const override { return !parent_; }

	private:
		VectorMath::matrix matrix_;
		std::shared_ptr<Scene> parent_ = nullptr;
		std::shared_ptr<Scene> next_ = nullptr;
	};

	class SceneMesh : public Scene
	{
	public:
		SceneMesh(std::shared_ptr<OpenGL::Mesh> mesh) : mesh_(mesh) {}
		void SetParent(const std::shared_ptr<Scene>& parent) override;
		const model_mesh GetLocalModelAndMesh() const override;
		bool IsLeaf() const override { return true; }
		bool IsRoot() const override { return !parent_; }

	private:
		std::shared_ptr<OpenGL::Mesh> mesh_;
		std::shared_ptr<Scene> parent_ = nullptr;
		std::shared_ptr<Scene> next_ = nullptr;
	};

	class SceneTree : public std::vector<std::shared_ptr<Scene>>
	{
	public:
		void AddNode(
			const std::shared_ptr<Scene>& node, 
			const std::shared_ptr<Scene>& parent = nullptr);
		std::shared_ptr<Scene> Root() { return *begin(); }
	};

} // End namespace SoftwareGL.