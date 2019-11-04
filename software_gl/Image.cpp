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
			static_cast<size_t>(v.GetX()) + 
			static_cast<size_t>(v.GetY()) * dx_) =
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
		if (v1_.GetX() > v2_.GetX()) std::swap(v1_, v2_);
		const float dx = 
			static_cast<float>(v2_.GetX()) - static_cast<float>(v1_.GetX());
		const float dy = 
			static_cast<float>(v2_.GetY()) - static_cast<float>(v1_.GetY());
		// Check the line is not horizontal.
		if (dx > dy) 
		{
			const float deltaerr = abs(dy / dx);
			float error = 0.0f;
			size_t x = static_cast<size_t>(v1_.GetX());
			size_t y = static_cast<size_t>(v1_.GetY());
			for (; x <= v2_.GetX(); ++x)
			{
				const Vertex v(
					VectorMath::vector2(
						static_cast<float>(x),
						static_cast<float>(y)),
					v1_.GetColor());
				DrawPixel(v);
				error += deltaerr;
				if (error >= 0.5f)
				{
					y += static_cast<size_t>(sign(dy));
					error -= 1.0f;
				}
			}
		}
		else 
		{
			// Check that the v1_ y is before the v2_ y else swap them.
			if (v1_.GetY() > v2_.GetY()) std::swap(v1_, v2_);
			const float deltaerr = abs(dx / dy);
			float error = 0.0f;
			size_t x = static_cast<size_t>(v1_.GetX());
			size_t y = static_cast<size_t>(v1_.GetY());
			for (; y <= v2_.GetY(); ++y) 
			{
				// TODO(dubouchet): Interpolate color.
				const Vertex v(
					VectorMath::vector2(
						static_cast<float>(x),
						static_cast<float>(y)),
					v1_.GetColor());
				DrawPixel(v);
				error += deltaerr;
				if (error >= 0.5f)
				{
					x += static_cast<size_t>(sign(dx));
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
		size_t min_x = 
			static_cast<size_t>(*std::min_element(vec_x.begin(), vec_x.end()));
		size_t min_y = 
			static_cast<size_t>(*std::min_element(vec_y.begin(), vec_y.end()));
		size_t max_x =
			static_cast<size_t>(*std::max_element(vec_x.begin(), vec_x.end()));
		size_t max_y =
			static_cast<size_t>(*std::max_element(vec_y.begin(), vec_y.end()));
		// Compute area.
		float area = 0.5f * 
			(-v2.GetY() * v3.GetX() + 
			v1.GetY() * (-v2.GetX() + v3.GetX()) + 
			v1.GetX() * (v2.GetY() - v3.GetY()) + 
			v2.GetX() * v3.GetY());
		// Get if current point is in triangle using barycentric coordinates.
		for (size_t x = min_x; x < max_x; ++x)
		{
			for (size_t y = min_y; y < max_y; ++y)
			{
				// Compute barycentric coordinates.
				const float s = 1.0f / (2.0f * area) * 
					(v1.GetY() * v3.GetX() - v1.GetX() * v3.GetY() + 
					(v3.GetY() - v1.GetY()) * x + (v1.GetX() - v3.GetX()) * y);
				const float t = 1.0f / (2.0f * area) * 
					(v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX() + 
					(v1.GetY() - v2.GetY()) * x + (v2.GetX() - v1.GetX()) * y);
				// Check if correct.
				const float u = s + t;
				if ((s >= 0.0f) && (s <= 1.0f) &&
					(t >= 0.0f) && (t <= 1.0f) &&
					(u >= 0.0f) && (u <= 1.0f))
				{
					// Interpolate color using s & t.
					VectorMath::vector4 color = 
						v1.GetColor() * s + 
						v2.GetColor() * t + 
						v3.GetColor() * (1.0f - u);
					const Vertex v(
						VectorMath::vector2 (
							static_cast<float>(x), 
							static_cast<float>(y)), 
						color);
					DrawPixel(v);
				}
			}
		}
	}

}	// End namespace SoftwareGL.
