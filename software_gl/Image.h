#pragma once

#include <vector>
#include <string>
#include "VectorMath.h"
#include "Vertex.h"
#include "Triangle.h"

namespace SoftwareGL {

	class Image : public std::vector<VectorMath::vector> 
	{
	public:
		Image() : dx_(0), dy_(0), std::vector<VectorMath::vector>() {}
		Image(size_t dx, size_t dy) : 
			dx_(dx), 
			dy_(dy), 
			std::vector<VectorMath::vector>(dx * dy, {.2f, .0f, .2f, 1.0f}) {}
		Image(const Image& image) = default;
		Image& operator=(const Image& image) = default;

	public:
		bool LoadFromTGA(const std::string& path);

	public:
		const std::pair<size_t, size_t> GetSize() const 
		{ 
			return std::make_pair(dx_, dy_); 
		}
		const float GetWidth() const { return static_cast<float>(dx_); }
		const float GetHeight() const { return static_cast<float>(dy_); }

	private:
		size_t dx_ = 0;
		size_t dy_ = 0;
	};

}	// End of namespace SoftwareGL.
