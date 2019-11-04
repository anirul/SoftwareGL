#include <algorithm>
#include <vector>
#include <tuple>
#include <assert.h>
#include "Image.h"

namespace SoftwareGL {

	void Image::DrawPixel(const Vertex& v)
	{
		// Check boundaries.
		if (v.GetX() < 0) return;
		if (v.GetX() > (dx_ - 1)) return;
		if (v.GetY() < 0) return;
		if (v.GetY() > (dy_ - 1)) return;
		// Draw the pixel
		operator[](
			static_cast<int>(v.GetX()) + 
			static_cast<int>(v.GetY()) * dx_) =
			v.GetColor();
	}

	void Image::DrawLine(const Vertex& v1, const Vertex& v2)
	{
		// Sign operation.
		auto sign = [](float val)->float {
			if (val > 0) return 1.0f;
			if (val < 0) return -1.0f;
			return 0.0f;
		};
		// Temporary variables make it inter changeable by swap.
		Vertex v1_ = v1;
		Vertex v2_ = v2;
		// Check that v1 is before v2 else swap them.
		if (v1_.GetX() > v2_.GetX())
		{
			std::swap(v1_, v2_);
		}
		float dx = v2_.GetX() - v1_.GetX();
		float dy = v2_.GetY() - v1_.GetY();
		// Check the line is not horizontal.
		if (abs(dx) > abs(dy))
		{
			const float deltaerr = abs(dy / dx);
			float error = 0.0f;
			int x = static_cast<int>(v1_.GetX());
			int y = static_cast<int>(v1_.GetY());
			for (; x <= v2_.GetX(); ++x)
			{
				const float s = (x - v1_.GetX()) / dx;
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x),
						static_cast<float>(y),
						0, 
						1),
					v1_.GetColor() * s + v2_.GetColor() * (1 - s));
				DrawPixel(v);
				error += deltaerr;
				if (error >= 0.5f)
				{
					y += static_cast<int>(sign(dy));
					error -= 1.0f;
				}
			}
		}
		else 
		{
			// Check that the v1_ y is before the v2_ y else swap them.
			if (v1_.GetY() > v2_.GetY())
			{
				std::swap(v1_, v2_);
				dx = v2_.GetX() - v1_.GetX();
				dy = v2_.GetY() - v1_.GetY();
			}
			const float deltaerr = abs(dx / dy);
			float error = 0.0f;
			int x = static_cast<int>(v1_.GetX());
			int y = static_cast<int>(v1_.GetY());
			for (; y <= v2_.GetY(); ++y) 
			{
				const float s = (y - v1_.GetY()) / dy;
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x),
						static_cast<float>(y),
						0,
						1),
					v1_.GetColor() * s + v2_.GetColor() * (1 - s));
				DrawPixel(v);
				error += deltaerr;
				if (error >= 0.5f)
				{
					x += static_cast<int>(sign(dx));
					error -= 1.0f;
				}
			}
		}
	}

	void Image::DrawTriangle(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3)
	{
		// DrawTriangleBarycentric(v1, v2, v3);
		DrawTriangleScanLine(v1, v2, v3);
	}

	// Get the border of the zone to be drawn.
	const VectorMath::vector4 Image::GetBorderTriangle(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3) const
	{
		float min_x = std::min(v1.GetX(), std::min(v2.GetX(), v3.GetX()));
		float min_y = std::min(v1.GetY(), std::min(v2.GetY(), v3.GetY()));
		float max_x = std::max(v1.GetX(), std::max(v2.GetX(), v3.GetX()));
		float max_y = std::max(v1.GetY(), std::max(v2.GetY(), v3.GetY()));
		return VectorMath::vector4(min_x, min_y, max_x, max_y);
	}

	const VectorMath::vector4 Image::Intersection(
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
			return VectorMath::vector(.0f, .0f, .0f, 1.f);
		}
		float x = (b2 * c1 - b1 * c2) / det;
		float y = (a1 * c2 - a2 * c1) / det;
		return VectorMath::vector(x, y, 1.f, 1.f);
	}

	const float Image::GetBarycentricS(const Vertex& v1, const Vertex& v2, const Vertex& v3, const VectorMath::vector2& pos) const
	{
		// Compute area.
		float area = 0.5f *
			(-v2.GetY() * v3.GetX() +
				v1.GetY() * (-v2.GetX() + v3.GetX()) +
				v1.GetX() * (v2.GetY() - v3.GetY()) +
				v2.GetX() * v3.GetY());
		const float A = 1.0f / (2.0f * area);
		const float Bs = v1.GetY() * v3.GetX() - v1.GetX() * v3.GetY();
		const float Cs = v3.GetY() - v1.GetY();
		const float Ds = v1.GetX() - v3.GetX();
		return A * (Bs + Cs * pos.x + Ds * pos.y);
	}

	const float Image::GetBarycentricT(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3, 
		const VectorMath::vector2& pos) const
	{
		// Compute area.
		float area = 0.5f *
			(-v2.GetY() * v3.GetX() +
				v1.GetY() * (-v2.GetX() + v3.GetX()) +
				v1.GetX() * (v2.GetY() - v3.GetY()) +
				v2.GetX() * v3.GetY());
		const float A = 1.0f / (2.0f * area);
		const float Bt = v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX();
		const float Ct = v1.GetY() - v2.GetY();
		const float Dt = v2.GetX() - v1.GetX();
		return A * (Bt + Ct * pos.x + Dt * pos.y);
	}

	const std::tuple<bool, VectorMath::vector4> Image::IntersectionTriangle(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3, 
		const VectorMath::vector4& l1,
		const VectorMath::vector4& border) const
	{
		// Get line from triangle.
		const auto lab = GetLineFromVertex(v1, v2);
		const auto lbc = GetLineFromVertex(v2, v3);
		const auto lca = GetLineFromVertex(v3, v1);
		// Get intersection from line to scanline.
		const auto i1 = Intersection(l1, lab);
		const auto i2 = Intersection(l1, lbc);
		const auto i3 = Intersection(l1, lca);
		std::vector<VectorMath::vector4> points = {};
		if (IntersectionWithinBorder(i1, border)) 
		{
			points.push_back(i1);
		}
		if (IntersectionWithinBorder(i2, border)) 
		{
			points.push_back(i2);
		}
		if (IntersectionWithinBorder(i3, border))
		{
			points.push_back(i3);
		}
		if (points.size() == 2) 
		{
			return std::make_tuple(
				true, 
				VectorMath::vector4(
					points[0].x,
					points[0].y,
					points[1].x,
					points[1].y));
		}
		return std::make_tuple(false, VectorMath::vector4());
	}

	const bool Image::IntersectionWithinBorder(
		const VectorMath::vector4& i, 
		const VectorMath::vector4& border) const
	{
		if (i.z == 0.0f) return false;
		if ((i.x < border.x) || (i.x > border.z)) return false;
		if ((i.y < border.y) || (i.y > border.w)) return false;
		return true;
	}

	const VectorMath::vector4 Image::GetLineFromVertex(
		const Vertex& v1, 
		const Vertex& v2) const
	{
		return VectorMath::vector4(v1.GetX(), v1.GetY(), v2.GetX(), v2.GetY());
	}

	// Draw triangle using barycentric coordinate.
	// Using barycentric coordinate to interpolate colors.
	// Also doesn't account for triangle order, and no Z-buffer yet.
	void Image::DrawTriangleBarycentric(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3)
	{
		// Get the bounding box.
		VectorMath::vector4 border = GetBorderTriangle(v1, v2, v3);
		// Compute area.
		float area = 0.5f * 
			(-v2.GetY() * v3.GetX() + 
			v1.GetY() * (-v2.GetX() + v3.GetX()) + 
			v1.GetX() * (v2.GetY() - v3.GetY()) + 
			v2.GetX() * v3.GetY());
		const float A = 1.0f / (2.0f * area);
		const float Bs = v1.GetY() * v3.GetX() - v1.GetX() * v3.GetY();
		const float Cs = v3.GetY() - v1.GetY();
		const float Ds = v1.GetX() - v3.GetX();
		const float Bt = v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX();
		const float Ct = v1.GetY() - v2.GetY();
		const float Dt = v2.GetX() - v1.GetX();
		// Get if current point is in triangle using barycentric coordinates.
		for (int x = static_cast<int>(border.x); x < border.z; ++x)
		{
			if ((x < 0.0f) || (x > dx_ - 1)) continue;
			for (int y = static_cast<int>(border.y); y < border.w; ++y)
			{
				if ((y < 0.0f) || (y > dy_ - 1)) continue;
				// Compute barycentric coordinates.
				const float s = A * (Bs + Cs * x + Ds * y);
				if ((s < 0.0f) || (s > 1.0f)) continue;
				const float t = A * (Bt + Ct * x + Dt * y);
				if ((t < 0.0f) || (t > 1.0f)) continue;
				// Check if correct.
				const float u = s + t;
				if ((u < 0.0f) || (u > 1.0f)) continue;
				// Interpolate color using s & t.
				VectorMath::vector4 color = 
					v1.GetColor() * s + 
					v2.GetColor() * t + 
					v3.GetColor() * (1.0f - u);
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x), 
						static_cast<float>(y),
						0,
						1),
					color);
				DrawPixel(v);				
			}
		}
	}

	void Image::DrawTriangleScanLine(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3)
	{
		VectorMath::vector4 border = GetBorderTriangle(v1, v2, v3);
		// Get if current point is in triangle using barycentric coordinates.
		for (int y = static_cast<int>(border.y); y < border.w; ++y)
		{
			if ((y < 0.0f) || (y > dy_ - 1)) continue;
			VectorMath::vector4 scan_line(border.x, y, border.z, y);
			auto bool_vec = IntersectionTriangle(
				v1, 
				v2, 
				v3, 
				scan_line, 
				border);
			if (std::get<0>(bool_vec)) {
				const float s1 = GetBarycentricS(
					v1, 
					v2, 
					v3, 
					VectorMath::vector2(std::get<1>(bool_vec).x, y));
				const float t1 = GetBarycentricT(
					v1,
					v2,
					v3,
					VectorMath::vector2(std::get<1>(bool_vec).x, y));
				const float u1 = s1 + t1;
				const float s2 = GetBarycentricS(
					v1,
					v2,
					v3,
					VectorMath::vector2(std::get<1>(bool_vec).z, y));
				const float t2 = GetBarycentricT(
					v1,
					v2,
					v3,
					VectorMath::vector2(std::get<1>(bool_vec).z, y));
				const float u2 = s2 + t2;
				Vertex l1(
					VectorMath::vector4(std::get<1>(bool_vec).x, y, 0, 1), 
					v1.GetColor() * s1 + 
					v2.GetColor() * t1 + 
					v3.GetColor() * (1 - u1));
				Vertex l2(
					VectorMath::vector4(std::get<1>(bool_vec).z, y, 0, 1), 
					v1.GetColor() * s2 +
					v2.GetColor() * t2 +
					v3.GetColor() * (1 - u2));
				DrawLine(l1, l2);
			}
		}
	}

}	// End namespace SoftwareGL.
