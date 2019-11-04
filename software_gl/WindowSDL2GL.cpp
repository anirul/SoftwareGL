#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <iostream>
#endif

#include "WindowSDL2GL.h"

namespace SoftwareGL {

	WindowSDL2GL::WindowSDL2GL(
		std::shared_ptr<WindowInterface> window_interface) :
		window_interface_(window_interface) {}

	WindowSDL2GL::~WindowSDL2GL()
	{
		SDL_Quit();
	}

	void WindowSDL2GL::Startup()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(NULL, "Couldn't initialize SDL.", "Software GL", 0);
#else
			std::cout << "Couldn't initialize SDL." << std::endl;
#endif
			return;
		}
		const auto p_size = window_interface_->GetWindowSize();
		sdl_window_ = SDL_CreateWindow(
			"Software GL",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			p_size.first,
			p_size.second,
			SDL_WINDOW_OPENGL);
		if (!sdl_window_)
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(
				NULL, 
				"Couldn't start a window in SDL.", 
				"Software GL", 
				0);
#else
			std::cout << "Couldn't start a window in SDL." << std::endl;
#endif
			return;
		}
		// GL context.
		sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
		SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		int value1;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value1);
		int value2;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value2);
		// While Run return true continue.
		if (window_interface_->Startup({ value1, value2 })) {
			bool loop = true;
			do {
				// Process events
				SDL_Event event;
				if (SDL_PollEvent(&event))
				{
					if (!window_interface_->RunEvent(event))
					{
						loop = false;
					}
				}
				if (!window_interface_->RunCompute())
				{
					loop = false;
				}
				SDL_GL_SwapWindow(sdl_window_);
			} while (loop);
			window_interface_->Cleanup();
		}
		// Cleanup.
		SDL_GL_DeleteContext(sdl_gl_context_);
		SDL_DestroyWindow(sdl_window_);
	}

}	// End of namespace SoftwareGL.
