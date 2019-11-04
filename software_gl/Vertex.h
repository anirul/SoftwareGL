#pragma once

#include <utility>
#include "VectorMath.h"

namespace SoftwareGL {

	class Vertex {
	public:
		Vertex() = default;
		Vertex(
			const VectorMath::vector2& screen_position,
			const VectorMath::vector4& color) :
			screen_position_(screen_position), color_(color) {}
		Vertex(const Vertex&) = default;
		Vertex& operator=(const Vertex& v) 
		{
			screen_position_ = v.screen_position_;
			color_ = v.color_;
			return *this;
		}

	public:
		const VectorMath::vector2 GetPosition() const 
		{ 
			return screen_position_; 
		}
		const float GetX() const { return screen_position_.x; }
		const float GetY() const { return screen_position_.y; }
		const VectorMath::vector4 GetColor() const { return color_;	}

	protected:
		VectorMath::vector2 screen_position_;
		VectorMath::vector4 color_;
	};

}	// End of namespace SoftwareGL
