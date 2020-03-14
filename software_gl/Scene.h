#pragma once

#include <memory>
#include <optional>
#include "../software_gl/VectorMath.h"
#include "../open_gl/Mesh.h"

namespace SoftwareGL {

	using model_mesh = 
		std::pair<
			const VectorMath::matrix, 
			const std::shared_ptr<OpenGL::Mesh>>;
	class ConstSceneIterator;

	struct Scene 
	{
		class ConstIterator
		{
		public:
			ConstIterator(std::shared_ptr<const Scene> scene);
			const model_mesh operator*() const;
			ConstIterator operator++();
			bool operator!=(const ConstIterator& it) const;
		private:
			std::shared_ptr<const Scene> scene_;
		};

		virtual const model_mesh GetLocalModelAndMesh() const = 0;
		virtual std::shared_ptr<const Scene> Next(
			std::shared_ptr<const Scene> self = nullptr) const = 0;
		virtual bool IsLeaf() const = 0;
		virtual ConstIterator begin() const = 0;
		ConstIterator end() const;
		bool operator!=(std::shared_ptr<Scene> scene) const;
	};

	class SceneMatrix : public Scene 
	{
	public:
		SceneMatrix(const VectorMath::matrix& matrix) : matrix_(matrix) {}
		void AddChild(std::shared_ptr<Scene> child);
		const model_mesh GetLocalModelAndMesh() const override;
		std::shared_ptr<const Scene> Next(
			std::shared_ptr<const Scene> self = nullptr) const override;
		bool IsLeaf() const override { return false; }
		ConstIterator begin() const override;

	private:
		std::vector<std::shared_ptr<Scene>> children_;
		VectorMath::matrix matrix_;
		std::shared_ptr<Scene> parent_ = nullptr;
	};

	class SceneMesh : public Scene
	{
	public:
		SceneMesh(std::shared_ptr<OpenGL::Mesh> mesh) : mesh_(mesh) {}
		const model_mesh GetLocalModelAndMesh() const override;
		std::shared_ptr<const Scene> Next(
			std::shared_ptr<const Scene> self = nullptr) const override;
		bool IsLeaf() const override { return true; }
		ConstIterator begin() const override;

	private:
		std::shared_ptr<OpenGL::Mesh> mesh_;
		std::shared_ptr<Scene> parent_ = nullptr;
	};

} // End namespace SoftwareGL.