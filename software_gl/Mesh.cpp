#include "Mesh.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace SoftwareGL {

	bool SoftwareGL::Mesh::LoadFromFile(const std::string& path)
	{
		std::ifstream ifs;
		ifs.open(path, std::ifstream::in);
		if (!ifs.is_open())
		{
			return false;
		}
		while (!ifs.eof()) {
			std::string line = "";
			if (!std::getline(ifs, line)) break;
			if (line == "") continue;
			std::istringstream iss(line);
			std::string dump;
			if (!(iss >> dump))	return false;
			if (dump.size() != 1) return false;
			switch (line[0]) {
			case 'v':
			{
				if (dump != "v") return false;
				VectorMath::vector4 v(0, 0, 0, 1);
				if (!(iss >> v.x)) return false;
				if (!(iss >> v.y)) return false;
				if (!(iss >> v.z)) return false;
				Vertex ver(v, { .5, .5, .5, 1 });
				points_.push_back(ver);
			}
				break;
			case 'f':
			{
				if (dump != "f") return false;
				std::array<int, 3> vi = { 0, 0, 0 };
				for (int& ii : vi)
				{
					if (!(iss >> ii)) return false;
				}
				for (const int i : vi)
				{
					indices_.push_back(i - 1);
				}
			}
				break;
			default:
				break;
			}
		}
		if (indices_.size() % 3 != 0) return false;
		return true;
	}

}	// End namespace SoftwareGL.
