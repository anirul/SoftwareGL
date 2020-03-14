#pragma once

#include <memory>
#include <utility>
#include <SDL.h>
#include "../software_gl/Mesh.h"
#include "../software_gl/Camera.h"
#include "../open_gl/Device.h"
#include "../open_gl/Program.h"
#include "../open_gl/Texture.h"
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
		bool Startup();
		// Run the window THIS WILL TAKE THE HAND.
		void Run();

	protected:
		void DrawImGui();
		void PostRunCompute();
#if !defined(__APPLE__)
		static void GLAPIENTRY ErrorMessageHandler(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam);
#endif

	private:
		std::shared_ptr<WindowInterface> window_interface_;
		std::shared_ptr<OpenGL::Device> device_;
		VectorMath::matrix model_ = {};
		SDL_Window* sdl_window_ = nullptr;
		SDL_GLContext sdl_gl_context_ = nullptr;
#if defined(_WIN32) || defined(_WIN64)
		HWND hwnd_;
#endif
	};

}	// End of namespace SoftwareGL.
