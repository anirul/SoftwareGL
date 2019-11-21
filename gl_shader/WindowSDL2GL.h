#pragma once

#include <memory>
#include <utility>
#include <SDL.h>

#include "WindowInterface.h"

namespace SoftwareGL {

	class WindowSDL2GL
	{
	public:
		// Create a window with the interface as an interface.
		WindowSDL2GL(std::shared_ptr<WindowInterface> window_interface);
		// Suppose to call all the cleanups.
		virtual ~WindowSDL2GL();
		// Start the window.
		void Startup();
		// Run the window THIS WILL TAKE THE HAND.
		void Run();

	protected:
		void PostRunCompute();

	private:
		std::shared_ptr<WindowInterface> window_interface_;
		SDL_Window* sdl_window_ = nullptr;
		SDL_GLContext sdl_gl_context_ = nullptr;
		GLuint vertex_array_object_ = 0;
		GLuint shader_program_ = 0;
		int major_version_ = 0;
		int minor_version_ = 0;
	};

}	// End of namespace SoftwareGL.
