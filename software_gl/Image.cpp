#include "Image.h"

#include <algorithm>
#include <fstream>
#include <vector>
#include <tuple>
#include <assert.h>

namespace SoftwareGL {

	struct tga_header {
		char  idlength;
		char  colourmaptype;
		char  datatypecode;
		short int colourmaporigin;
		short int colourmaplength;
		char  colourmapdepth;
		short int x_origin;
		short int y_origin;
		short width;
		short height;
		char  bitsperpixel;
		char  imagedescriptor;
	};

	bool Image::LoadTGA(const std::string& path)
	{
		assert(sizeof(tga_header) == 18);
		std::ifstream ifs(path, std::ios::binary);
		if (!ifs.is_open()) return false;
		return true;
	}

}	// End namespace SoftwareGL.
