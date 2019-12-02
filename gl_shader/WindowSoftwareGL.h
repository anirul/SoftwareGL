#pragma once

#include "WindowInterface.h"

class WindowSoftwareGL : public SoftwareGL::WindowInterface
{
public:
	WindowSoftwareGL(size_t width, size_t height) : 
		width_(width), 
		height_(height) {}
	bool Startup(const std::pair<int, int>& gl_version) override;
	bool RunCompute(const double delta_time) override;
	bool RunEvent(const SDL_Event& event) override;
	void Cleanup() override {}
	const std::pair<size_t, size_t> GetWindowSize() const override;

protected:
	size_t width_ = 640;
	size_t height_ = 480;
	float z_min_ = 0.1f;
	float z_max_ = 10000.0f;
};
