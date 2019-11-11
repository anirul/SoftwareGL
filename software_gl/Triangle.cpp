#include <limits>
#include "Triangle.h"

namespace SoftwareGL {

	Triangle& Triangle::operator=(const Triangle& triangle)
	{
		v1_ = triangle.v1_;
		v2_ = triangle.v2_;
		v3_ = triangle.v3_;
		SetVertexConst();
		return *this;
	}

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

	Triangle Triangle::AllPositionDivideByW() const
	{
		Triangle out = *this;
		auto v1 = v1_.GetPosition();
		if (v1.w) 
		{
			v1.x /= v1.w;
			v1.y /= v1.w;
			v1.z /= v1.w;
			out.v1_.SetPosition(v1);
		}
		auto v2 = v2_.GetPosition();
		if (v2.w)
		{
			v2.x /= v2.w;
			v2.y /= v2.w;
			v2.z /= v2.w;
			out.v2_.SetPosition(v2);
		}
		auto v3 = v3_.GetPosition();
		if (v3.w)
		{
			v3.x /= v3.w;
			v3.y /= v3.w;
			v3.z /= v3.w;
			out.v3_.SetPosition(v3);
		}
		return out;
	}

	Triangle Triangle::AllPositionAdd(const float f) const
	{
		auto out = *this;
		out.v1_.SetPosition(v1_.GetPosition() + f);
		out.v2_.SetPosition(v2_.GetPosition() + f);
		out.v3_.SetPosition(v3_.GetPosition() + f);
		return out;
	}

	Triangle Triangle::AllPositionMult(const float f) const
	{
		auto out = *this;
		out.v1_.SetPosition(v1_.GetPosition() * f);
		out.v2_.SetPosition(v2_.GetPosition() * f);
		out.v3_.SetPosition(v3_.GetPosition() * f);
		return out;
	}

	Triangle Triangle::AllPositionMult(const VectorMath::vector4& v) const
	{
		auto out = *this;
		out.v1_.SetPosition(v1_.GetPosition() | v);
		out.v2_.SetPosition(v2_.GetPosition() | v);
		out.v3_.SetPosition(v3_.GetPosition() | v);
		return out;
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

	void Triangle::SetVertexConst()
	{
		area_ = 0.5f *
			(-v2_.GetY() * v3_.GetX() +
				v1_.GetY() * (-v2_.GetX() + v3_.GetX()) +
				v1_.GetX() * (v2_.GetY() - v3_.GetY()) +
				v2_.GetX() * v3_.GetY());
		den_ = 1.f / ((v2_.GetY() - v3_.GetY()) *
			(v1_.GetX() - v3_.GetX()) + (v3_.GetX() - v2_.GetX()) *
			(v1_.GetY() - v3_.GetY()));
		// Could't get the constexpr version of min and max to work.
		border_ = VectorMath::vector(
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
			((v2_.GetY() > v3_.GetY()) ? v2_.GetY() : v3_.GetY()));
	}

	Triangle Triangle::MatrixMult(const VectorMath::matrix& matrix) const
	{
		Triangle out = *this;
		out.v1_.SetPosition(
			VectorMath::VectorMult(out.GetV1().GetPosition(), matrix));
		out.v2_.SetPosition(
			VectorMath::VectorMult(out.GetV2().GetPosition(), matrix));
		out.v3_.SetPosition(
			VectorMath::VectorMult(out.GetV3().GetPosition(), matrix));
		if (is_normal_fixed_)
		{
			return out;
		}
		out.v1_.SetNormal(
			VectorMath::VectorMult(out.GetV1().GetNormal(), matrix));
		out.v2_.SetNormal(
			VectorMath::VectorMult(out.GetV2().GetNormal(), matrix));
		out.v3_.SetNormal(
			VectorMath::VectorMult(out.GetV3().GetNormal(), matrix));
		return out;
	}

	void Triangle::NormalFixed(bool is_fixed)
	{
		is_normal_fixed_ = is_fixed;
		// Check if set to true.
		if (is_normal_fixed_) {
			// Check if there is not any normals.
			if ((!v1_.GetNormal().LengthSquared()) ||
				(!v2_.GetNormal().LengthSquared()) ||
				(!v3_.GetNormal().LengthSquared()))
			{
				// Compute normal as a perpendicular to the surface.
				VectorMath::vector normal = 
					((v2_.GetPosition() - v1_.GetPosition()) % 
					(v3_.GetPosition() - v1_.GetPosition())).Normalize();
				v1_.SetNormal(normal);
				v2_.SetNormal(normal);
				v3_.SetNormal(normal);
			}
		}
	}

}	// End namespace SoftwareGL.
