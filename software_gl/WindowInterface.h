#pragma once

#include <SDL.h>

class WindowInterface
{
public:
	// Startup of the window (with GL version).
	virtual bool Startup(const std::pair<int, int>& gl_version) = 0;
	// Compute run (called after the event run).
	virtual bool RunCompute() = 0;
	// Event run will give the event.
	virtual bool RunEvent(const SDL_Event& event) = 0;
	// Cleanup after the window has been closed.
	virtual void Cleanup() = 0;
};
