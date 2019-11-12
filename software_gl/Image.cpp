#include "Image.h"

#include <algorithm>
#include <fstream>
#include <vector>
#include <tuple>
#include <assert.h>

namespace SoftwareGL {

#ifdef WIN32
#	pragma pack(4)
#endif // WIN32

	struct tga_header
	{
		uint8_t length;
		uint8_t color_map_type;
		uint8_t image_type;

		uint16_t color_map_origin;
		uint16_t color_map_length;
		uint8_t color_map_entry_size;

		uint16_t x_origin;
		uint16_t y_origin;
		uint16_t width;
		uint16_t height;
		uint8_t bits;
		uint8_t image_descriptor;
	};

	bool Image::LoadTGA(const std::string& path)
	{
		std::ifstream ifs(path, std::ios::binary);
		if (!ifs.is_open()) return false;

		ifs.seekg(0, std::ios_base::end);
		size_t file_size = ifs.tellg();
		ifs.seekg(0, std::ios_base::beg);

		tga_header header;
		// Fill up the header.
		ifs.read((char*)&header.length, sizeof(uint8_t));
		ifs.read((char*)&header.color_map_type, sizeof(uint8_t));
		ifs.read((char*)&header.image_type, sizeof(uint8_t));
		ifs.read((char*)&header.color_map_origin, sizeof(uint16_t));
		ifs.read((char*)&header.color_map_length, sizeof(uint16_t));
		ifs.read((char*)&header.color_map_entry_size, sizeof(uint8_t));
		ifs.read((char*)&header.x_origin, sizeof(uint16_t));
		ifs.read((char*)&header.y_origin, sizeof(uint16_t));
		ifs.read((char*)&header.width, sizeof(uint16_t));
		ifs.read((char*)&header.height, sizeof(uint16_t));
		ifs.read((char*)&header.bits, sizeof(uint8_t));
		ifs.read((char*)&header.image_descriptor, sizeof(uint8_t));
		// Resize the buffer.
		resize(header.width * header.height);
		switch (header.image_type) 
		{
		case 2:
		{
			if (header.bits == 24)
			{
				for_each(begin(), end(), [&ifs](VectorMath::vector& v) 
				{
					uint8_t r;
					uint8_t g;
					uint8_t b;
					ifs.read((char*)&r, sizeof(uint8_t));
					ifs.read((char*)&g, sizeof(uint8_t));
					ifs.read((char*)&b, sizeof(uint8_t));
					v.x = static_cast<float>(r) / 255.f;
					v.y = static_cast<float>(g) / 255.f;
					v.z = static_cast<float>(b) / 255.f;
					v.w = 1.f;
				});
			}
			else if (header.bits == 32)
			{
				for_each(begin(), end(), [&ifs](VectorMath::vector& v)
				{
					uint8_t r;
					uint8_t g;
					uint8_t b;
					uint8_t a;
					ifs.read((char*)&r, sizeof(uint8_t));
					ifs.read((char*)&g, sizeof(uint8_t));
					ifs.read((char*)&b, sizeof(uint8_t));
					ifs.read((char*)&a, sizeof(uint8_t));
					v.x = static_cast<float>(r) / 255.f;
					v.y = static_cast<float>(g) / 255.f;
					v.z = static_cast<float>(b) / 255.f;
					v.w = static_cast<float>(a) / 255.f;
				});
			}
			else 
			{ 
				return false; 
			}
			break;
		}
		default:
			return false;
		}
		return true;
	}

}	// End namespace SoftwareGL.
