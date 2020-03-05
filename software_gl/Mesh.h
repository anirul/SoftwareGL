#pragma once

#include <string>
#include <vector>
#include <array>
#include "../software_gl/Vertex.h"
#include "../software_gl/Triangle.h"

namespace SoftwareGL {

	class Mesh {
	public:
		Mesh() = default;
		Mesh(const Mesh& mesh) = default;
		Mesh& operator=(const Mesh& mesh) = default;

	public:
		bool LoadFromObj(const std::string& path);
		const std::vector<VectorMath::vector4>& GetPositions() const;
		const std::vector<VectorMath::vector4>& GetNormals() const;
		const std::vector<VectorMath::vector3>& GetTextures() const;
		const std::vector<std::array<int, 3>>& GetIndices() const;

	public:
		// Operation on matrices.
		// Multiply a triangle with a matrix (do this for all vectors).
		void AllPositionMatrixMult(const VectorMath::matrix& matrix);
		void AllNormalMatrixMult(const VectorMath::matrix& matrix);
		// Re normalizing.
		void AllTextureDivideByZ();
		void AllPositionDivideByW();
		// Function to change all positions.
		void AllPositionAdd(const float f);
		void AllPositionMult(const float f);
		void AllPositionMult(const VectorMath::vector4& v);

	public:
		class ConstIterator {
		public:
			ConstIterator(int position, const Mesh& mesh);
			const Triangle& operator*() const { return *triangle_; }
			ConstIterator& operator++();
			bool operator!=(const ConstIterator& it) const;
		
		protected:
			void SetLocalTriangle();

		private:
			std::unique_ptr<Triangle> triangle_;
			int position_;
			const Mesh& mesh_;
		};
		bool operator!=(const Mesh& mesh) const;
		ConstIterator begin() const { return { 0, *this }; }
		ConstIterator end() const 
		{ 
			return { static_cast<int>(this->indices_.size()), *this };
		}

	private:
		std::vector<VectorMath::vector4> positions_ = {};
		std::vector<VectorMath::vector4> normals_ = {};
		std::vector<VectorMath::vector3> textures_ = {};
		std::vector<std::array<int, 3>> indices_ = {};
	};

}	// End namespace SoftwareGL.
