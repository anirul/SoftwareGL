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
#include <functional>
#include <imgui.h>
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <SDL.h>

#include "Shader.h"
#include "WindowSDL2GL.h"

namespace SoftwareGL {

	void GLAPIENTRY WindowSDL2GL::ErrorMessageHandler(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) 
	{
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
		std::ostringstream oss;
		oss << "message\t: " << message << std::endl;
		oss << "type\t: ";
		switch (type) 
		{
		case GL_DEBUG_TYPE_ERROR:
			oss << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			oss << "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			oss << "PORABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			oss << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			oss << "OTHER";
			break;
		}
		oss << std::endl;
		oss << "id\t: " << id << std::endl;
		oss << "severity\t: ";
		switch (severity) 
		{
		case GL_DEBUG_SEVERITY_LOW:
			oss << "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			oss << "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			oss << "HIGH";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			oss << "NOTIFICATION";
			break;
		}
		oss << std::endl;
#if defined(_WIN32) || defined(_WIN64)
		MessageBox(NULL, oss.str().c_str(),	"OpenGL Error",	0);
#else
		std::cout << "OpenGL Error: " << oss.str() << std::endl;
#endif
	}

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}

	bool WindowSDL2GL::Startup()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(NULL, "Couldn't initialize SDL.", "Software GL", 0);
#else
			std::cout << "Couldn't initialize SDL." << std::endl;
#endif
			return false;
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
			return false;
		}
		// GL context.
		sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
		SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
			std::cerr << "Couldn't start GLEW." << std::endl;
#endif
			return false;
		}
#if _DEBUG
		// Enable error message.
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(WindowSDL2GL::ErrorMessageHandler, nullptr);
#endif

		// Points and color initialization.
		float points[] = {
			0.0f, 0.5f, 0.0f,
			0.5f, -.5f, 0.0f,
			-.5f, -.5f, 0.0f,
		};
		float colors[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};
		unsigned int indices[] = {
			0, 2, 1
		};

		// Position buffer initialization.
		GLuint point_buffer_object = 0;
		glGenBuffers(1, &point_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, point_buffer_object);
		glBufferData(
			GL_ARRAY_BUFFER,
			9 * sizeof(float),
			points,
			GL_STATIC_DRAW);

		// Color buffer initialization.
		GLuint color_buffer_object = 0;
		glGenBuffers(1, &color_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object);
		glBufferData(
			GL_ARRAY_BUFFER,
			9 * sizeof(float),
			colors,
			GL_STATIC_DRAW);

		// Index buffer array.
		glGenBuffers(1, &index_buffer_object_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			3 * sizeof(unsigned int), 
			indices, 
			GL_STATIC_DRAW);

		// Vertex attribute initialization.
		GLuint vertex_attribute_object = 0;
		glGenVertexArrays(1, &vertex_attribute_object);
		glBindVertexArray(vertex_attribute_object);
		glBindBuffer(GL_ARRAY_BUFFER, point_buffer_object);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		// Enable vertex attrib array.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		// Vertex Shader program.
		OpenGL::Shader vertex_shader(GL_VERTEX_SHADER);
		if (!vertex_shader.LoadFromFile("../asset/vertex.glsl"))
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(
				NULL, 
				vertex_shader.GetErrorMessage().c_str(), 
				"Vertex shader Error", 
				0);
#else
			std::cout 
				<< "Vertex shader Error: " 
				<< vertex_shader.GetErrorMessage() 
				<< std::endl;
#endif
			return false;
		}

		// Fragment Shader program.
		OpenGL::Shader fragment_shader(GL_FRAGMENT_SHADER);
		if (!fragment_shader.LoadFromFile("../asset/fragment.glsl"))
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(
				NULL,
				fragment_shader.GetErrorMessage().c_str(),
				"Fragment shader Error",
				0);
#else
			std::cout
				<< "Fragment shader Error: "
				<< fragment_shader.GetErrorMessage()
				<< std::endl;
#endif
			return false;
		}

		// Create the program.
		shader_program_ = glCreateProgram();
		glAttachShader(shader_program_, fragment_shader.GetId());
		glAttachShader(shader_program_, vertex_shader.GetId());
		glLinkProgram(shader_program_);
		glUseProgram(shader_program_);

		// Start the user part of the window.
		// FIXME(anirul): This should be done before.
		if (!window_interface_->Startup({ major_version_, minor_version_ })) {
			std::string error = "Error version is too low (" +
				std::to_string(major_version_) + ", " +
				std::to_string(minor_version_) + ")";
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(NULL, error.c_str(), "Fragment shader Error", 0);
#else
			std::cout << "Fragment shader Error: " << error << std::endl;
#endif
			return false;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(
			sdl_window_,
			sdl_gl_context_);
		char* glsl_version = "#version 430";
		ImGui_ImplOpenGL3_Init(glsl_version);

		return true;
	}

	void WindowSDL2GL::Run()
	{
		// While Run return true continue.
		bool loop = true;
		double previous_count = 0.0f;
		// Timing counter.
		static auto start = std::chrono::system_clock::now();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		do 
		{
			// Compute the time difference from previous frame.
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> time = end - start;
			// Process events
			SDL_Event event;
			if (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				if (!window_interface_->RunEvent(event))
				{
					loop = false;
					continue;
				}
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(sdl_window_);
			ImGui::NewFrame();

			if (!window_interface_->RunCompute(time.count() - previous_count))
			{
				loop = false;
				continue;
			}
			PostRunCompute();

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");
				ImGui::Text("This is some useful text.");

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				if (ImGui::Button("Button"))
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text(
					"Application average %.3f ms/frame (%.1f FPS)", 
					1000.0f / ImGui::GetIO().Framerate, 
					ImGui::GetIO().Framerate);
				ImGui::Text("Application is %f, %f", display_width, display_height);
				ImGui::End();
			}

			previous_count = time.count();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SDL_GL_SwapWindow(sdl_window_);
		} 
		while (loop);
		window_interface_->Cleanup();
		// Cleanup.
		SDL_GL_DeleteContext(sdl_gl_context_);
		SDL_DestroyWindow(sdl_window_);
		SDL_Quit();
	}

}	// End of namespace SoftwareGL.
