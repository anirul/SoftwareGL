#include <string>
#include <vector>

struct Pixel {
	unsigned char r, g, b, a;
};

// technically you don't need y.
struct ImageData {
	size_t x;
	size_t y;
	std::vector<Pixel> img;
};

struct ImageIOInterface {
	virtual bool Save(
		const ImageData& data,
		const std::string& path) = 0;
	virtual bool Load(
		ImageData& data,
		const std::string& path) = 0;
};
