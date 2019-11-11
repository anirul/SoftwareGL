#pragma once

#include "Image.h"
#include <memory>

namespace SoftwareGL {

	class Renderer {
	public:
		Renderer(Image image) : image_(image) {}

	public:
		void ClearFrame(const VectorMath::vector& color, const float z_max);
		void DrawPixel(const Vertex& v);
		void DrawLine(const Vertex& v1, const Vertex& v2);
		void DrawTriangle(const Triangle& tri);
		const Image& GetImage() const { return image_; }

	private:
		std::vector<float> z_buffer_;
		Image image_;
	};

}	// End of namespace SoftwareGL.