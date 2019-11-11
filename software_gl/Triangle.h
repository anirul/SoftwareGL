#pragma once

#include <algorithm>
#include <vector>
#include "Vertex.h"

namespace SoftwareGL {

	class Triangle
	{
	public:
		Triangle() = default;
		Triangle(const Triangle& triangle) : 
			v1_(triangle.v1_), v2_(triangle.v2_), v3_(triangle.v3_) 
		{
			SetVertexConst();
		}
		Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) :
			v1_(v1), v2_(v2), v3_(v3) 
		{
			SetVertexConst();
		}
		Triangle& operator=(const Triangle& triangle);

	public:
		float GetArea() const;
		VectorMath::vector4 GetBorder() const;
		float GetBarycentricS(const VectorMath::vector2& pos) const;
		float GetBarycentricT(const VectorMath::vector2& pos) const;

	public:
		std::vector<VectorMath::vector2> IntersectWithinBorder(
			const VectorMath::vector4& l) const;
		// Setter and getter for vertices.
		const Vertex GetV1() const { return v1_; }
		const Vertex GetV2() const { return v2_; }
		const Vertex GetV3() const { return v3_; }
		void SetV1(const Vertex& v)
		{
			v1_ = v;
			SetVertexConst();
		}
		void SetV2(const Vertex& v)
		{
			v2_ = v;
			SetVertexConst();
		}
		void SetV3(const Vertex& v)
		{
			v3_ = v;
			SetVertexConst();
		}

	public:
		// Mutliply a triangle with a matrix (do this for all vectors).
		Triangle AllPositionMatrixMult(const VectorMath::matrix& matrix) const;
		Triangle AllNormalMatrixMult(const VectorMath::matrix& matrix) const;
		// Renormalizing.
		Triangle AllPositionDivideByW() const;
		// Function to change all positions.
		Triangle AllPositionAdd(const float f) const;
		Triangle AllPositionMult(const float f) const;
		Triangle AllPositionMult(const VectorMath::vector4& v) const;

	protected:
		const VectorMath::vector2 IntersectionLineLine(
			const VectorMath::vector4& l1,
			const VectorMath::vector4& l2) const;
		bool IsWithinBorder(const VectorMath::vector2& pos) const;
		void SetVertexConst();

	private:
		// A Triangle is made out of 3 vertices.
		Vertex v1_ = {};
		Vertex v2_ = {};
		Vertex v3_ = {};
		// Some constants that are fixed for barycentric calculations.
		float area_;
		float den_;
		// Could't get the constexpr version of min and max to work.
		VectorMath::vector4 border_;
	};

}	// End namespace SoftwareGL