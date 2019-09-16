#pragma once

#include <memory>
#include <utility>

#include <SDL.h>

#include "WindowInterface.h"

class WindowSDL2GL
{
public:
	// Create a window with the interface as an interface.
	WindowSDL2GL(std::shared_ptr<WindowInterface> window_interface);
	// Suppose to call all the cleanups.
	virtual ~WindowSDL2GL();
	// Start the window THIS WILL TAKE THE HAND.
	void Startup();

protected:
	std::shared_ptr<WindowInterface> window_interface_;
	SDL_Window* sdl_window_;
	SDL_GLContext sdl_gl_context_;
};

