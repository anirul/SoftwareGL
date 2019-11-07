#pragma once

#include <string>
#include <vector>
#include "Vertex.h"

namespace SoftwareGL {

	class Mesh {
	public:
		Mesh() : points_(), indices_() {}
		bool LoadFromFile(const std::string& path);
		const std::vector<Vertex>& GetPoints() const { return points_; }
		const std::vector<int>& GetIndices() const { return indices_; }

	private:
		std::vector<Vertex> points_;
		std::vector<int> indices_;
	};

}	// End namespace SoftwareGL.