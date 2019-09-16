#include <string>
#include <vector>

struct pixel {
	unsigned char r, g, b, a;
};

// technically you don't need y.
struct image_data {
	size_t x;
	size_t y;
	std::vector<pixel> img;
};

struct image_interface {
	virtual bool save(
		const image_data& data,
		const std::string& path) = 0;
	virtual bool load(
		image_data& data,
		const std::string& path) = 0;
};
