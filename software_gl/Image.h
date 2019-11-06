#pragma once

#include <vector>
#include "VectorMath.h"
#include "Vertex.h"
#include "Triangle.h"

namespace SoftwareGL {

	// technically you don't need y.
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
		void DrawPixel(const Vertex& v);
		void DrawLine(const Vertex& v1, const Vertex& v2);
		void DrawTriangle(
			const Vertex& v1,
			const Vertex& v2,
			const Vertex& v3);

	protected:
		void DrawTriangleBarycentric(const Triangle& tri);
		void DrawTriangleScanLine(const Triangle& tri);

	private:
		const size_t dx_ = 0;
		const size_t dy_ = 0;
	};

}	// End of namespace SoftwareGL