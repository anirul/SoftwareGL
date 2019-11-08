#pragma once

#include <algorithm>
#include <vector>
#include "Vertex.h"

namespace SoftwareGL {

	class Triangle
	{
	public:
		Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) :
			v1_(v1), v2_(v2), v3_(v3) {}
		float GetArea() const;
		VectorMath::vector4 GetBorder() const;
		float GetBarycentricS(const VectorMath::vector2& pos) const;
		float GetBarycentricT(const VectorMath::vector2& pos) const;
		std::vector<VectorMath::vector2> IntersectWithinBorder(
			const VectorMath::vector4& l) const;
		const Vertex GetV1() const { return v1_; }
		const Vertex GetV2() const { return v2_; }
		const Vertex GetV3() const { return v3_; }

	protected:
		const VectorMath::vector2 IntersectionLineLine(
			const VectorMath::vector4& l1,
			const VectorMath::vector4& l2) const;
		bool IsWithinBorder(const VectorMath::vector2& pos) const;

	private:
		const Vertex v1_;
		const Vertex v2_;
		const Vertex v3_;
		const float area_ = 0.5f *
			(-v2_.GetY() * v3_.GetX() +
				v1_.GetY() * (-v2_.GetX() + v3_.GetX()) +
				v1_.GetX() * (v2_.GetY() - v3_.GetY()) +
				v2_.GetX() * v3_.GetY());
		const float den_ = 1.f / ((v2_.GetY() - v3_.GetY()) * 
			(v1_.GetX() - v3_.GetX()) + (v3_.GetX() - v2_.GetX()) * 
			(v1_.GetY() - v3_.GetY()));
		// Could't get the constexpr version of min and max to work.
		const VectorMath::vector4 border_ = {
			(v1_.GetX() < v2_.GetX()) ?
			((v1_.GetX() < v3_.GetX()) ? v1_.GetX() : v3_.GetX()) :
			((v2_.GetX() < v3_.GetX()) ? v2_.GetX() : v3_.GetX()),
			(v1_.GetY() < v2_.GetY()) ?
			((v1_.GetY() < v3_.GetY()) ? v1_.GetY() : v3_.GetY()) :
			((v2_.GetY() < v3_.GetY()) ? v2_.GetY() : v3_.GetY()),
			(v1_.GetX() > v2_.GetX()) ?
			((v1_.GetX() > v3_.GetX()) ? v1_.GetX() : v3_.GetX()) :
			((v2_.GetX() > v3_.GetX()) ? v2_.GetX() : v3_.GetX()),
			(v1_.GetY() > v2_.GetY()) ?
			((v1_.GetY() > v3_.GetY()) ? v1_.GetY() : v3_.GetY()) :
			((v2_.GetY() > v3_.GetY()) ? v2_.GetY() : v3_.GetY()) 
		};

	};

}	// End namespace SoftwareGL