#pragma once

#include "../software_gl/ImageInterface.h"
#include "../software_gl/WindowInterface.h"
#include "../software_gl/VectorMath.h"
#include "../software_gl/Image.h"
#include "../software_gl/Camera.h"
#include "../software_gl/Mesh.h"

class WindowSoftwareGL : public SoftwareGL::WindowInterface
{
public:
	WindowSoftwareGL(size_t width, size_t height) : 
		cam_({ 0, 0, -5 }, { 0, 0, -1 }, { 0, 1, 0 }),
		width_(width), 
		height_(height), 
		image_(width, height) {}
	bool Startup(const std::pair<int, int>& gl_version) override;
	bool RunCompute() override;
	bool RunEvent(const SDL_Event& event) override;
	void Cleanup() override {}
	const std::pair<size_t, size_t> GetWindowSize() const override;
	const SoftwareGL::Image& GetWindowImage() const override 
	{ 
		return image_; 
	}

protected:
	VectorMath::matrix projection_;
	VectorMath::matrix look_at_;
	VectorMath::matrix translation_;
	VectorMath::matrix rotate_;
	VectorMath::matrix scale_;
	SoftwareGL::Mesh mesh_;
	SoftwareGL::Camera cam_;
	size_t width_ = 640;
	size_t height_ = 480;
	unsigned int texture_id_ = 0;
	std::vector<SoftwareGL::Vertex> vertex_;
	SoftwareGL::Image image_{width_, height_};
	std::vector<float> z_buffer_;
	float z_min_ = 0.1f;
	float z_max_ = 10000.0f;
};
