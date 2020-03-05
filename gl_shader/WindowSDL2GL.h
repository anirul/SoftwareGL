#pragma once

#include <memory>
#include <utility>
#include <SDL.h>
#include "../software_gl/Mesh.h"
#include "../software_gl/Camera.h"
#include "Program.h"
#include "Texture.h"
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
		std::shared_ptr<OpenGL::Program> program_ = nullptr;
		std::shared_ptr<OpenGL::Texture> texture1_ = nullptr;
		std::shared_ptr<SoftwareGL::Mesh> mesh_ = nullptr;
		std::shared_ptr<SoftwareGL::Camera> camera_ = nullptr;
		SDL_Window* sdl_window_ = nullptr;
		SDL_GLContext sdl_gl_context_ = nullptr;
		GLuint vertex_array_object_ = 0;
		GLuint shader_program_ = 0;
		GLuint index_buffer_object_ = 0;
		int major_version_ = 0;
		int minor_version_ = 0;
#if defined(_WIN32) || defined(_WIN64)
		HWND hwnd_;
#endif
	};

}	// End of namespace SoftwareGL.
