#pragma once

#include "VectorMath.h"

namespace SoftwareGL {

	class Camera {
	public:
		Camera(
			const VectorMath::vector3& pos,
			const VectorMath::vector3& to,
			const VectorMath::vector3& up) : 
			pos_(pos), to_(to), up_(up) {}
		const VectorMath::matrix LookAt() const;
		const VectorMath::vector Direction() const;
		const VectorMath::vector Position() const;

	protected:
		VectorMath::vector3 pos_ = { 0, 0, 0 };
		VectorMath::vector3 to_ = { 0, 0, 1 };
		VectorMath::vector3 up_ = { 0, 1, 0 };
	};

}	// End namespace SoftwareGL