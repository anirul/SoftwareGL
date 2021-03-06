#pragma once

#include "Image.h"
#include <memory>
#include "Camera.h"
#include "Mesh.h"
#include "Triangle.h"
#include "VectorMath.h"
#include "Vertex.h"

namespace SoftwareGL {

	class Renderer {
	public:
		Renderer(Image image) : texture_(1, 1), image_(image) {}

	public:
		void ClearFrame(const VectorMath::vector& color, const float z_max);
		void DrawPixel(const Vertex& v);
		void DrawLine(const Vertex& v1, const Vertex& v2);
		void DrawTriangle(const Triangle& tri);
		const Image& GetImage() const { return image_; }
		void SetTexture(const Image& texture) { texture_ = texture; }

	private:
		Image texture_;
		std::vector<float> z_buffer_;
		std::vector<Mesh> meshes_;
		Image image_;
	};

}	// End of namespace SoftwareGL.
