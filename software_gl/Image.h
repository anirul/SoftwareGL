#pragma once

#include <vector>
#include "VectorMath.h"
#include "Vertex.h"
#include "Triangle.h"

namespace SoftwareGL {

	class Image : public std::vector<VectorMath::vector> {
	public:
		Image() {}
		Image(size_t dx, size_t dy) : 
			dx_(dx), 
			dy_(dy), 
			std::vector<VectorMath::vector>(dx * dy, {.2f, .0f, .2f, 1.0f}) {}

	public:
		const std::pair<size_t, size_t> GetSize() const 
		{ 
			return std::make_pair(dx_, dy_); 
		}
		void DrawPixel(
			const Vertex& v,
			std::vector<float>& z_buffer = std::vector<float>());
		void DrawLine(
			const Vertex& v1, 
			const Vertex& v2, 
			const VectorMath::vector& normal = { 0, 0, 0, 0 },
			std::vector<float>& z_buffer = std::vector<float>());
		void DrawTriangle(
			const Triangle& tri, 
			const VectorMath::vector& normal = { 0, 0, 0, 0 },
			std::vector<float>& z_buffer = std::vector<float>());
		const float GetWidth() const { return static_cast<float>(dx_); }
		const float GetHeight() const { return static_cast<float>(dy_); }

	protected:
		void DrawTriangleBarycentric(
			const Triangle& tri,
			const VectorMath::vector& normal,
			std::vector<float>& z_buffer);
		void DrawTriangleScanLine(
			const Triangle& tri,
			const VectorMath::vector& normal,
			std::vector<float>& z_buffer);

	private:
		const size_t dx_ = 0;
		const size_t dy_ = 0;
	};

}	// End of namespace SoftwareGL