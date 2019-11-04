#include <algorithm>
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
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x),
						static_cast<float>(y),
						0, 
						1),
					v1_.GetColor());
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
				// TODO(dubouchet): Interpolate color.
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x),
						static_cast<float>(y),
						0,
						1),
					v1_.GetColor());
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

	// Draw triangle using barycentric coordinate.
	// Using barycentric coordinate to interpolate colors.
	// Also doesn't account for triangle order, and no Z-buffer yet.
	void Image::DrawTriangle(
		const Vertex& v1, 
		const Vertex& v2, 
		const Vertex& v3)
	{
		// Get the bounding box.
		// Should swap with array but having trouble with VC++.
		const std::vector<float> vec_x{ v1.GetX(), v2.GetX(), v3.GetX() };
		const std::vector<float> vec_y{ v1.GetY(), v2.GetY(), v3.GetY() };
		int min_x = 
			static_cast<int>(*std::min_element(vec_x.begin(), vec_x.end()));
		int min_y = 
			static_cast<int>(*std::min_element(vec_y.begin(), vec_y.end()));
		int max_x =
			static_cast<int>(*std::max_element(vec_x.begin(), vec_x.end()));
		int max_y =
			static_cast<int>(*std::max_element(vec_y.begin(), vec_y.end()));
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
		for (int x = min_x; x < max_x; ++x)
		{
			if ((x < 0.0f) || (x > dx_ - 1)) continue;
			for (int y = min_y; y < max_y; ++y)
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

}	// End namespace SoftwareGL.
