#include "Renderer.h"
#include <algorithm>
#include <vector>
#include <tuple>
#include <assert.h>
#include "Image.h"

namespace SoftwareGL {

	void Renderer::ClearFrame(
		const VectorMath::vector& color, 
		const float z_max)
	{
		z_buffer_.resize(image_.size());
		std::fill(image_.begin(), image_.end(), color);
		std::fill(z_buffer_.begin(), z_buffer_.end(), z_max);
	}

	void Renderer::DrawPixel(const Vertex& v)
	{
		// Check boundaries.
		if (v.GetX() < 0) return;
		if (v.GetX() > (image_.GetWidth() - 1)) return;
		if (v.GetY() < 0) return;
		if (v.GetY() > (image_.GetHeight() - 1)) return;
		const size_t index  =
			static_cast<size_t>(v.GetX()) +
			static_cast<size_t>(v.GetY()) * 
			static_cast<size_t>(image_.GetWidth());
		assert(index < z_buffer_.size());
		if (z_buffer_.size() != 0)
		{
			if (z_buffer_[index] <= v.GetZ())
			{
				return;
			}
			z_buffer_[index] = v.GetZ();
		}
		// Draw the pixel
		image_[index] = v.GetColor();
	}

	void Renderer::DrawLine(const Vertex& v1, const Vertex& v2)
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
			const float z = v1_.GetZ();
			for (; x <= v2_.GetX(); ++x)
			{
				const float s = (x - v1_.GetX()) / dx;
				const float z = v1_.GetZ() * s + v2_.GetZ() * (1 - s);
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x),
						static_cast<float>(y),
						z,
						0),
					v1_.GetColor()* s + v2_.GetColor() * (1 - s));
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
					v1_.GetColor()* s + v2_.GetColor() * (1 - s));
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
	void Renderer::DrawTriangle(const Triangle& tri)
	{
		static const VectorMath::vector4 light = { 0, 0, -1, 0 };
		// Get the bounding box.
		VectorMath::vector4 border = tri.GetBorder();
		// Get if current point is in triangle using barycentric coordinates.
		for (int x = static_cast<int>(border.x); x < border.z; ++x)
		{
			if ((x < 0.0f) || (x > image_.GetWidth() - 1)) continue;
			for (int y = static_cast<int>(border.y); y < border.w; ++y)
			{
				if ((y < 0.0f) || (y > image_.GetHeight() - 1)) continue;
				// Compute barycentric coordinates.
				const float s = tri.GetBarycentricS(VectorMath::vector2(
					static_cast<float>(x),
					static_cast<float>(y)));
				if ((s < 0.0f) || (s > 1.0f)) continue;
				const float t = tri.GetBarycentricT(VectorMath::vector2(
					static_cast<float>(x),
					static_cast<float>(y)));
				if ((t < 0.0f) || (t > 1.0f)) continue;
				const float u = 1.f - (s + t);
				if ((u < 0.0f) || (u > 1.0f)) continue;
				assert((1 - (s + t + u)) < epsilon);
				// Compute z using barycentric coordinates.
				const float z =
					tri.GetV1().GetZ() * s +
					tri.GetV2().GetZ() * t +
					tri.GetV3().GetZ() * u;
				// Interpolate color using s & t.
				float shade = 1.0f;
				const bool are_normal_different =
					abs((tri.GetV1().GetNormal() -
						tri.GetV2().GetNormal()).LengthSquared()) < epsilon &&
					abs((tri.GetV1().GetNormal() -
						tri.GetV2().GetNormal()).LengthSquared()) < epsilon;
				VectorMath::vector4 normal;
				if (are_normal_different)
				{
					normal =
						tri.GetV1().GetNormal() * s +
						tri.GetV2().GetNormal() * t +
						tri.GetV3().GetNormal() * u;
				}
				else
				{
					normal = tri.GetV1().GetNormal();
				}
				shade = normal * light;
				VectorMath::vector4 color =
					(tri.GetV1().GetColor() * s +
						tri.GetV2().GetColor() * t +
						tri.GetV3().GetColor() * u) * shade;
				VectorMath::vector2 uv = {
					tri.GetV1().GetTexture().x * s +
					tri.GetV2().GetTexture().x * t +
					tri.GetV3().GetTexture().x * u,
					tri.GetV1().GetTexture().y * s +
					tri.GetV2().GetTexture().y * t +
					tri.GetV3().GetTexture().y * u
				};
				const Vertex v(
					VectorMath::vector4(
						static_cast<float>(x),
						static_cast<float>(y),
						z,
						1),
					color,
					normal,
					uv);
				DrawPixel(v);
			}
		}
	}

}	// End of namespace SoftwareGL.