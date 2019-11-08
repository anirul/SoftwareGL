#include <limits>
#include "Triangle.h"

namespace SoftwareGL {

	float SoftwareGL::Triangle::GetArea() const
	{
		return area_;
	}

	VectorMath::vector4 Triangle::GetBorder() const
	{
		return border_;
	}

	float Triangle::GetBarycentricS(const VectorMath::vector2& pos) const
	{
		return ((v2_.GetY() - v3_.GetY()) * (pos.x - v3_.GetX()) + 
			(v3_.GetX() - v2_.GetX()) * (pos.y - v3_.GetY())) * den_;
	}

	float Triangle::GetBarycentricT(const VectorMath::vector2& pos) const
	{
		return ((v3_.GetY() - v1_.GetY()) * (pos.x - v3_.GetX()) + 
			(v1_.GetX() - v3_.GetX()) * (pos.y - v3_.GetY())) * den_;
	}

	std::vector<VectorMath::vector2> Triangle::IntersectWithinBorder(
		const VectorMath::vector4& l) const
	{
		std::vector<VectorMath::vector2> points = {};
		// Get line from triangle.
		const auto lab = 
			VectorMath::vector4(
				v1_.GetX(), v1_.GetY(), v2_.GetX(), v2_.GetY());
		const auto lbc = 
			VectorMath::vector4(
				v2_.GetX(), v2_.GetY(), v3_.GetX(), v3_.GetY());
		const auto lca = 
			VectorMath::vector4(
				v3_.GetX(), v3_.GetY(), v1_.GetX(), v1_.GetY());
		// Get intersection from line to scanline.
		const auto i1 = IntersectionLineLine(l, lab);
		const auto i2 = IntersectionLineLine(l, lbc);
		const auto i3 = IntersectionLineLine(l, lca);
		if (IsWithinBorder(i1))
		{
			points.push_back(i1);
		}
		if (IsWithinBorder(i2))
		{
			points.push_back(i2);
		}
		if (IsWithinBorder(i3))
		{
			points.push_back(i3);
		}
		return points;
	}

	const VectorMath::vector2 Triangle::IntersectionLineLine(
		const VectorMath::vector4& l1, 
		const VectorMath::vector4& l2) const
	{
		// Line AB represented as a1x + b1y = c1
		float a1 = l1.w - l1.y;
		float b1 = l1.x - l1.z;
		float c1 = a1 * l1.x + b1 * l1.y;
		// Line CD represented as a2x + b2y = c2
		float a2 = l2.w - l2.y;
		float b2 = l2.x - l2.z;
		float c2 = a2 * l2.x + b2 * l2.y;
		// Compute the determinant.
		float det = a1 * b2 - a2 * b1;
		if (det == 0.0f)
		{
			return VectorMath::vector2(
				std::numeric_limits<float>::infinity(),
				std::numeric_limits<float>::infinity());
		}
		float x = (b2 * c1 - b1 * c2) / det;
		float y = (a1 * c2 - a2 * c1) / det;
		return VectorMath::vector2(x, y);
	}

	bool Triangle::IsWithinBorder(const VectorMath::vector2& pos) const
	{
		if ((pos.x < border_.x) || (pos.x > border_.z)) return false;
		if ((pos.y < border_.y) || (pos.y > border_.w)) return false;
		return true;
	}

}	// End namespace SoftwareGL.
