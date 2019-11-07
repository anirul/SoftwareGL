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
		const Triangle& tri,
		const VectorMath::vector& normal,
		std::vector<float>& z_buffer)
	{
		DrawTriangleBarycentric(tri, normal, z_buffer);
		// DrawTriangleScanLine(tri, normal, z_buffer);
	}

	// Draw triangle using barycentric coordinate.
	// Using barycentric coordinate to interpolate colors.
	// Also doesn't account for triangle order, and no Z-buffer yet.
	void Image::DrawTriangleBarycentric(
		const Triangle& tri,
		const VectorMath::vector& normal,
		std::vector<float>& z_buffer)
	{
		static const VectorMath::vector4 light = {0, 0, -1, 0};
		// Get the bounding box.
		VectorMath::vector4 border = tri.GetBorder();
		// Get if current point is in triangle using barycentric coordinates.
		for (int x = static_cast<int>(border.x); x < border.z; ++x)
		{
			if ((x < 0.0f) || (x > dx_ - 1)) continue;
			for (int y = static_cast<int>(border.y); y < border.w; ++y)
			{
				if ((y < 0.0f) || (y > dy_ - 1)) continue;
				// Compute barycentric coordinates.
				const float s = tri.GetBarycentricS(VectorMath::vector2(
					static_cast<float>(x), 
					static_cast<float>(y)));
				if ((s < 0.0f) || (s > 1.0f)) continue;
				const float t = tri.GetBarycentricT(VectorMath::vector2(
					static_cast<float>(x), 
					static_cast<float>(y)));
				if ((t < 0.0f) || (t > 1.0f)) continue;
				// Check if correct.
				const float u = s + t;
				if ((u < 0.0f) || (u > 1.0f)) continue;
				const float z =
					tri.GetV1().GetZ() * s +
					tri.GetV2().GetZ() * t +
					tri.GetV3().GetZ() * (1 - u);
				if (z_buffer[x + y * dx_] > z)
				{
					z_buffer[x + y * dx_] = z;
					// Interpolate color using s & t.
					float shade = 1.0f;
					if (normal.LengthSquared() != 0.0f) {
						shade = normal * light;
					}
					VectorMath::vector4 color;
					color =
						(tri.GetV1().GetColor() * s +
							tri.GetV2().GetColor() * t +
							tri.GetV3().GetColor() * (1 - u)) * shade;
					color = VectorMath::vector(
						z_buffer[x + y * dx_], 
						z_buffer[x + y * dx_] / 2, 
						z_buffer[x + y * dx_] / 4,
						1);
					color = -normal;
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
	}

	void Image::DrawTriangleScanLine(
		const Triangle& tri,
		const VectorMath::vector& normal,
		std::vector<float>& z_buffer)
	{
		VectorMath::vector4 border = tri.GetBorder();
		// Get if current point is in triangle using barycentric coordinates.
		for (int y = static_cast<int>(border.y); y < border.w; ++y)
		{
			if ((y < 0.0f) || (y > dy_ - 1)) continue;
			const float fy = static_cast<float>(y);
			VectorMath::vector4 scan_line(border.x, fy,	border.z, fy);
			auto vec = tri.IntersectWithinBorder(scan_line);
			if (vec.size() == 2) {
				const float s1 = tri.GetBarycentricS(
					VectorMath::vector2(vec[0].x, fy));
				const float t1 = tri.GetBarycentricT(
					VectorMath::vector2(vec[0].x, fy));
				const float u1 = s1 + t1;
				const float s2 = tri.GetBarycentricS(
					VectorMath::vector2(vec[1].x, fy));
				const float t2 = tri.GetBarycentricT(
					VectorMath::vector2(vec[1].x, fy));
				const float u2 = s2 + t2;
				Vertex l1(
					VectorMath::vector4(vec[0].x, fy, 0, 1), 
					tri.GetV1().GetColor() * s1 + 
					tri.GetV2().GetColor() * t1 + 
					tri.GetV3().GetColor() * (1 - u1));
				Vertex l2(
					VectorMath::vector4(vec[1].x, fy, 0, 1), 
					tri.GetV1().GetColor() * s2 +
					tri.GetV2().GetColor() * t2 +
					tri.GetV3().GetColor() * (1 - u2));
				DrawLine(l1, l2);
			}
		}
	}

}	// End namespace SoftwareGL.
