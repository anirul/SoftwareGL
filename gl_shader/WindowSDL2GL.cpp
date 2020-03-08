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
#include <set>
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <sdl2/SDL.h>
#include <GL/glew.h>
#if defined(_WIN32) || defined(_WIN64)
	#include <sdl2/SDL_syswm.h>
#endif
#include "Shader.h"
#include "WindowSDL2GL.h"
#include "Texture.h"

namespace SoftwareGL {

#if !defined(__APPLE__)
	void GLAPIENTRY WindowSDL2GL::ErrorMessageHandler(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam) 
	{
		// Remove notifications.
 		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) 
 			return;
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
		MessageBox(nullptr, oss.str().c_str(), "OpenGL Error", 0);
#else
		std::cerr << "OpenGL Error: " << oss.str() << std::endl;
#endif
	}
#endif

	WindowSDL2GL::WindowSDL2GL(
		std::shared_ptr<WindowInterface> window_interface) :
		window_interface_(window_interface) 
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(nullptr, "Couldn't initialize SDL2.", "Software GL", 0);
#else
			std::cerr << "Couldn't initialize SDL2." << std::endl;
#endif
			throw std::runtime_error("Couldn't initialize SDL2.");
		}
		const auto p_size = window_interface_->GetWindowSize();
		sdl_window_ = SDL_CreateWindow(
			"Shader OpenGL",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			static_cast<int>(p_size.first),
			static_cast<int>(p_size.second),
			SDL_WINDOW_OPENGL);
		if (!sdl_window_)
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(
				nullptr,
				"Couldn't start a window in SDL2.",
				"Software GL",
				0);
#else
			std::cerr << "Couldn't start a window in SDL2." << std::endl;
#endif
			throw std::runtime_error("Couldn't start a window in SDL2.");
		}
#if defined(_WIN32) || defined(_WIN64)
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(sdl_window_, &wmInfo);
		hwnd_ = wmInfo.info.win.window;
#endif
	}

	WindowSDL2GL::~WindowSDL2GL()
	{
		SDL_Quit();
	}

	void WindowSDL2GL::PostRunCompute()
	{
		// Update time.
		static auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float> time = end - start;
		// Update the model matrix.
		{
			VectorMath::matrix r_x;
			VectorMath::matrix r_y;
			VectorMath::matrix r_z;
			r_x.RotateXMatrix(time.count() * 0.7f);
			r_y.RotateYMatrix(time.count() * 0.5f);
			r_z.RotateZMatrix(time.count());
			model_ = r_x * r_y * r_z;
		}
//		program_->UniformMatrix("model", model_);
		// Clear the screen.
		glClearColor(.2f, 0.f, .2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Bind textures.
		texture1_->Bind(0);
		// Draw the new view.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
		glDrawElements(
			GL_TRIANGLES, 
			static_cast<GLsizei>(mesh_->GetIndices().size() * 3), 
			GL_UNSIGNED_INT, 
			nullptr);
	}

	bool WindowSDL2GL::Startup()
	{
		// GL context.
		sdl_gl_context_ = SDL_GL_CreateContext(sdl_window_);
		SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
#if defined(__APPLE__)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
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
				hwnd_,
				"Couldn't initialize GLEW.",
				"Shader GL Error",
				0);
#else
			std::cerr << "Couldn't start GLEW." << std::endl;
#endif
			return false;
		}
#if _DEBUG && !defined(__APPLE__)
		// Enable error message.
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(WindowSDL2GL::ErrorMessageHandler, nullptr);
#endif
		// Mesh creation.
		mesh_ = std::make_shared<SoftwareGL::Mesh>();
		mesh_->LoadFromObj("../asset/TorusUVNormal.obj");
		mesh_->ComputeFlat();

		// Position buffer initialization.
		GLuint point_buffer_object = 0;
		glGenBuffers(1, &point_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, point_buffer_object);
		glBufferData(
			GL_ARRAY_BUFFER,
			mesh_->GetFlatPositions().size() * sizeof(float),
			mesh_->GetFlatPositions().data(),
			GL_STATIC_DRAW);

		// Color buffer initialization.
		GLuint normal_buffer_object = 0;
		glGenBuffers(1, &normal_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object);
		glBufferData(
			GL_ARRAY_BUFFER,
			mesh_->GetFlatNormals().size() * sizeof(float),
			mesh_->GetFlatNormals().data(),
			GL_STATIC_DRAW);

		// Texture coordinates buffer initialization.
		GLuint texcoor_buffer_object = 0;
		glGenBuffers(1, &texcoor_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, texcoor_buffer_object);
		glBufferData(
			GL_ARRAY_BUFFER,
			mesh_->GetFlatTextures().size() * sizeof(float),
			mesh_->GetFlatTextures().data(),
			GL_STATIC_DRAW);

		// Index buffer array.
		glGenBuffers(1, &index_buffer_object_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			mesh_->GetFlatIndices().size() * sizeof(unsigned int),
			mesh_->GetFlatIndices().data(),
			GL_STATIC_DRAW);

		// Vertex attribute initialization.
		GLuint vertex_attribute_object = 0;
		glGenVertexArrays(1, &vertex_attribute_object);
		glBindVertexArray(vertex_attribute_object);
		glBindBuffer(GL_ARRAY_BUFFER, point_buffer_object);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, texcoor_buffer_object);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Enable vertex attrib array.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Vertex Shader program.
		OpenGL::Shader vertex_shader(GL_VERTEX_SHADER);
		if (!vertex_shader.LoadFromFile("../asset/vertex.glsl"))
		{
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(
				hwnd_, 
				vertex_shader.GetErrorMessage().c_str(), 
				"Vertex shader Error", 
				0);
#else
			std::cerr
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
				hwnd_,
				fragment_shader.GetErrorMessage().c_str(),
				"Fragment shader Error",
				0);
#else
			std::cerr
				<< "Fragment shader Error: "
				<< fragment_shader.GetErrorMessage()
				<< std::endl;
#endif
			return false;
		}

		// Create the program.
		program_ = std::make_shared<OpenGL::Program>(
			vertex_shader, 
			fragment_shader);
		program_->Use();
		
		// Bind the texture to the shader.
		const unsigned int slot = 0;
		texture1_ = std::make_shared<OpenGL::Texture>(
			"../asset/Texture.tga");
//			"../asset/PaintedMetal05_col.tga");
		texture1_->Bind(slot);
		program_->UniformInt("texture1", slot);
		
		// Enable blending to 1 - source alpha.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable Z buffer.
		glEnable(GL_DEPTH_TEST);

		// Set the perspective matrix.
		const std::pair<size_t, size_t> size =
			window_interface_->GetWindowSize();
		const float aspect = 
			static_cast<float>(size.first) / static_cast<float>(size.second);
		VectorMath::matrix perspective = VectorMath::Perspective(
			65.0f * static_cast<float>(M_PI) / 180.0f,
			aspect,
			0.1f,
			1000.0f);
		program_->UniformMatrix("projection", perspective);

		// Set the camera.
		camera_ = std::make_shared<SoftwareGL::Camera>(
			VectorMath::vector3{ 0.f, 0.f, -4.f });
		VectorMath::matrix view = camera_->LookAt();
		program_->UniformMatrix("view", view);

		// Set the model matrix (identity for now).
		program_->UniformMatrix("model", model_);

		// Start the user part of the window.
		// FIXME(anirul): This should be done before.
		if (!window_interface_->Startup({ major_version_, minor_version_ })) 
		{
			std::string error = "Error version is too low (" +
				std::to_string(major_version_) + ", " +
				std::to_string(minor_version_) + ")";
#if defined(_WIN32) || defined(_WIN64)
			MessageBox(hwnd_, error.c_str(), "Fragment shader Error", 0);
#else
			std::cout << "Fragment shader Error: " << error << std::endl;
#endif
			return false;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(sdl_window_, sdl_gl_context_);
#if defined(__APPLE__)
		const char* glsl_version = "#version 410";
#else
		const char* glsl_version = "#version 430";
#endif
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

			if (!window_interface_->RunCompute(time.count() - previous_count))
			{
				loop = false;
				continue;
			}
			PostRunCompute();
			previous_count = time.count();
			// DrawImGui();
			SDL_GL_SwapWindow(sdl_window_);
		} 
		while (loop);
		window_interface_->Cleanup();
		// Cleanup.
		SDL_GL_DeleteContext(sdl_gl_context_);
		SDL_DestroyWindow(sdl_window_);
		SDL_Quit();
	}

	void WindowSDL2GL::DrawImGui()
	{
		// Create a global io variable.
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(sdl_window_);
		ImGui::NewFrame();

		const ImU32 flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove;

		static float f = 0.0f;
		static int counter = 0;
		static ImVec4 my_color{ .4f, 0.f, .4f, .5f };

		auto display_width = (float)io.DisplaySize.x;
		auto display_height = (float)io.DisplaySize.y;

		ImGui::SetNextWindowPos(
			ImVec2(display_width, display_height),
			ImGuiCond_Always,
			ImVec2(1.f, 1.f));
		ImGui::SetNextWindowSize(ImVec2(340, 120), ImGuiCond_Always);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, my_color);
		ImGui::Begin("Hello, world!", nullptr, flags);
		if (ImGui::CollapsingHeader("test"))
		{
			ImGui::Text("This is some useful text.");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			if (ImGui::Button("Button")) counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);
		}

		ImGui::Text(
			"Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
		ImGui::ColorEdit4("Color", &my_color.x);
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}	// End namespace SoftwareGL.
