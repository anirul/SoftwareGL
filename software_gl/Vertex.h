#pragma once

#include <utility>
#include "VectorMath.h"

namespace SoftwareGL {

	class Vertex {
	public:
		Vertex() = default;
		Vertex(
			const VectorMath::vector4& position,
			const VectorMath::vector4& color) :
			position_(position), color_(color) {}
		Vertex(const Vertex&) = default;
		Vertex& operator=(const Vertex& v) 
		{
			position_ = v.position_;
			color_ = v.color_;
			return *this;
		}

	public:
		const VectorMath::vector4 GetPosition() const { return position_; }
		const float GetX() const { return position_.x; }
		const float GetY() const { return position_.y; }
		const float GetZ() const { return position_.z; }
		const float GetW() const { return position_.w; }
		const VectorMath::vector4 GetColor() const { return color_;	}
		void SetColor(const VectorMath::vector4& color) { color_ = color; }
		void SetPosition(const VectorMath::vector4& pos) { position_ = pos; }

	protected:
		VectorMath::vector4 position_;
		VectorMath::vector4 color_;
	};

}	// End of namespace SoftwareGL
