#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <iostream>
#endif

#include <chrono>
#include <ctime>
#include <sstream>
#include <array>
#include <algorithm>
#include <numeric>
#include <GL/glew.h>
#include <SDL.h>

#include "WindowSDL2GL.h"
#include "../software_gl/Image.h"

namespace SoftwareGL {

	WindowSDL2GL::WindowSDL2GL(
		std::shared_ptr<WindowInterface> window_interface) :
		window_interface_(window_interface) {}

	WindowSDL2GL::~WindowSDL2GL()
	{
		SDL_Quit();
	}

	void WindowSDL2GL::PostRunCompute()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program_);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
			static_cast<int>(p_size.first),
			static_cast<int>(p_size.second),
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
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version_);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version_);
		// Vsync off.
		SDL_GL_SetSwapInterval(0);

		// Initialize GLEW.
		if (GLEW_OK != glewInit())
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(
				NULL,
				"Couldn't initialize GLEW.",
				"Shader GL Error",
				0);
#else
			std::cout << "Couldn't start GLEW." << std::endl;
#endif
		}

		// Before the main loop initialization
		float points[] = {
			0.0f,  0.5f,  0.0f,
			0.5f, -0.5f,  0.0f,
		   -0.5f, -0.5f,  0.0f
		};

		// Vertex buffer initialization.
		GLuint vertex_buffer_object = 0;
		glGenBuffers(1, &vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
		glBufferData(
			GL_ARRAY_BUFFER,
			9 * sizeof(float),
			points,
			GL_STATIC_DRAW);
		// Vertex attribute initialization.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		// Shader program.
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		const char* vertex_buffer =
			window_interface_->GetVertexShader().c_str();
		glShaderSource(vs, 1, &vertex_buffer, NULL);
		glCompileShader(vs);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragment_shader =
			window_interface_->GetFragmentShader().c_str();
		glShaderSource(fs, 1, &fragment_shader, NULL);
		glCompileShader(fs);
		glAttachShader(shader_program_, fs);
		glAttachShader(shader_program_, vs);
		glLinkProgram(shader_program_);
	}

	void WindowSDL2GL::Run()
	{
		// While Run return true continue.
		if (window_interface_->Startup({ major_version_, minor_version_ })) 
		{
			bool loop = true;
			float previous_count = 0.0f;
			float redraw_window_title = 0.0f;
			const float redraw_window_delta = 0.01f;
			float frame_count = 0.f;
			const size_t array_length = 10;
			std::array<float, array_length> fps_stats = { 0.f };

			// Timing counter.
			static auto start = std::chrono::system_clock::now();

			do 
			{
				// Compute the time difference from previous frame.
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<float> time = end - start;
				// Process events
				SDL_Event event;
				if (SDL_PollEvent(&event))
				{
					if (!window_interface_->RunEvent(event))
					{
						loop = false;
					}
				}
				if (!window_interface_->RunCompute(
					time.count() - previous_count))
				{
					loop = false;
				}
				PostRunCompute();
				SDL_GL_SwapWindow(sdl_window_);
				const float window_delta = time.count() - redraw_window_title;
				++frame_count;
				if (window_delta > redraw_window_delta)
				{
					static size_t counter = 0;
					// Compute the approximate fps at instant t.
					fps_stats[++counter % array_length] =
						frame_count * (1.f / window_delta);
					// Make an average on a number of sub values.
					float fps_value = std::accumulate(
						fps_stats.begin(), 
						fps_stats.end(), 
						0.0f) / fps_stats.size();
					std::ostringstream oss;
					oss << "Shader GL - "
						<< fps_value;
					SDL_SetWindowTitle(sdl_window_, oss.str().c_str());
					redraw_window_title = time.count();
					frame_count = 0;
				}
				previous_count = time.count();
			} 
			while (loop);
			window_interface_->Cleanup();
		}
		// Cleanup.
		SDL_GL_DeleteContext(sdl_gl_context_);
		SDL_DestroyWindow(sdl_window_);
	}

}	// End of namespace SoftwareGL.
