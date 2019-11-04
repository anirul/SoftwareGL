#pragma once

#include <string>
#include <vector>
#include "Image.h"
#include "VectorMath.h"

namespace SoftwareGL {

	struct ImageIOInterface {
		virtual bool Save(
			const Image& data,
			const std::string& path) = 0;
		virtual bool Load(
			Image& data,
			const std::string& path) = 0;
	};

}	// End of namespace SoftwareGL