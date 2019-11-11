#pragma once

#include <utility>
#include "VectorMath.h"

namespace SoftwareGL {

	class Vertex {
	public:
		Vertex() = default;
		Vertex(const VectorMath::vector4& position) : position_(position) {}
		Vertex(
			const VectorMath::vector4& position,
			const VectorMath::vector4& color) :
			position_(position), color_(color) {}
		Vertex(
			const VectorMath::vector4& position,
			const VectorMath::vector4& color,
			const VectorMath::vector4& normal,
			const VectorMath::vector2& texture) :
			position_(position),
			color_(color),
			normal_(normal),
			texture_(texture) {}
		Vertex(const Vertex&) = default;
		Vertex& operator=(const Vertex& v) = default;

	public:
		// Basic accessors.
		const VectorMath::vector4 GetPosition() const { return position_; }
		void SetPosition(const VectorMath::vector4& pos) { position_ = pos; }
		const VectorMath::vector4 GetColor() const { return color_;	}
		void SetColor(const VectorMath::vector4& color) { color_ = color; }
		const VectorMath::vector4 GetNormal() const { return normal_; }
		void SetNormal(const VectorMath::vector4& normal) { normal_ = normal; }
		const VectorMath::vector2 GetTexture() const { return texture_; }
		void SetTexture(const VectorMath::vector2& texture) 
		{ 
			texture_ = texture; 
		}
		// Utility.
		const float GetX() const { return position_.x; }
		const float GetY() const { return position_.y; }
		const float GetZ() const { return position_.z; }
		const float GetW() const { return position_.w; }

	protected:
		VectorMath::vector4 position_ = { 0, 0, 0, 1 };
		VectorMath::vector4 color_ = { .5, .5, .5, 1 };
		VectorMath::vector4 normal_ = { 0, 0, 0, 0 };
		VectorMath::vector2 texture_ = { 0, 0 };
	};

}	// End of namespace SoftwareGL
