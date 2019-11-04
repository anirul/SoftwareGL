#pragma once

#include <vector>
#include "VectorMath.h"
#include "Vertex.h"

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
		const VectorMath::vector4 GetBorderTriangle(
			const Vertex& v1,
			const Vertex& v2,
			const Vertex& v3) const;
		// Pass 2 lines defined by 2 points (in vector4) and return a vector4
		// that contain the point of intersection in x, y and a condition in z.
		const VectorMath::vector4 Intersection(
			const VectorMath::vector4& l1,
			const VectorMath::vector4& l2) const;
		const float GetBarycentricS(
			const Vertex& v1, 
			const Vertex& v2, 
			const Vertex& v3, 
			const VectorMath::vector2& pos) const;
		const float GetBarycentricT(
			const Vertex& v1,
			const Vertex& v2,
			const Vertex& v3,
			const VectorMath::vector2& pos) const;
		const std::tuple<bool, VectorMath::vector4> IntersectionTriangle(
			const Vertex& v1,
			const Vertex& v2,
			const Vertex& v3,
			const VectorMath::vector4& l1,
			const VectorMath::vector4& border) const;
		const bool IntersectionWithinBorder(
			const VectorMath::vector4& i,
			const VectorMath::vector4& border) const;
		const VectorMath::vector4 GetLineFromVertex(
			const Vertex& v1,
			const Vertex& v2) const;
		void DrawTriangleBarycentric(
			const Vertex& v1, 
			const Vertex& v2, 
			const Vertex& v3);
		void DrawTriangleScanLine(
			const Vertex& v1,
			const Vertex& v2,
			const Vertex& v3);

	private:
		const size_t dx_ = 0;
		const size_t dy_ = 0;
	};

}	// End of namespace SoftwareGL