#pragma once

#include <string>
#include <vector>
#include "VectorMath.h"

namespace SoftwareGL {

	// technically you don't need y.
	struct ImageData {
		size_t x;
		size_t y;
		std::vector<VectorMath::vector> img;
	};

	struct ImageIOInterface {
		virtual bool Save(
			const ImageData& data,
			const std::string& path) = 0;
		virtual bool Load(
			ImageData& data,
			const std::string& path) = 0;
	};

}	// End of namespace SoftwareGL