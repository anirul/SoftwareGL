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
#include "../open_gl/Shader.h"
#include "../open_gl/Device.h"
#include "WindowSDL2GL.h"
#include "../open_gl/Texture.h"

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

	void WindowSDL2GL::ErrorMessageDisplay(const std::string& error)
	{
#if defined(_WIN32) || defined(_WIN64)
		MessageBox(hwnd_, error.c_str(), "OpenGL Error", 0);
#else
		std::cerr << "OpenGL Error: " << error << std::endl;
#endif
	}

#endif

	WindowSDL2GL::WindowSDL2GL(
		std::shared_ptr<WindowInterface> window_interface) :
		window_interface_(window_interface) 
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			ErrorMessageDisplay("Couldn't initialize SDL2.");
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
			ErrorMessageDisplay("Couldn't start a window in SDL2.");
			throw std::runtime_error("Couldn't start a window in SDL2.");
		}
#if defined(_WIN32) || defined(_WIN64)
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(sdl_window_, &wmInfo);
		hwnd_ = wmInfo.info.win.window;
#endif
		// Create a new device.
		device_ = std::make_shared<OpenGL::Device>(sdl_window_);
		device_->SetCallbackError([this](const std::string& error)
		{
			ErrorMessageDisplay(error);
		});
	}

	WindowSDL2GL::~WindowSDL2GL()
	{
		SDL_Quit();
	}

	void WindowSDL2GL::PostRunCompute()
	{
		// Draw the new view.
		device_->Draw();
	}

	bool WindowSDL2GL::Startup()
	{
#if _DEBUG && !defined(__APPLE__)
		// Enable error message.
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(WindowSDL2GL::ErrorMessageHandler, nullptr);
#endif
		// Start the user part of the window.
		if (!window_interface_->Startup(device_->GetGLVersion()))
		{
			auto p = device_->GetGLVersion();
			std::string error = "Error version is too low (" +
				std::to_string(p.first) + ", " +
				std::to_string(p.second) + ")";
			ErrorMessageDisplay(error);
			return false;
		}

		// Device Startup call.
		device_->Startup(window_interface_->GetWindowSize());

		// Mesh creation.
		device_->AddTexture(
			"texture1",
			std::make_shared<OpenGL::Texture>("../asset/Texture.tga"));
		auto gl_mesh = 
			std::make_shared<OpenGL::Mesh>("../asset/TorusUVNormal.obj");
		gl_mesh->SetTexture({ "texture1" });

		// Pack it into a Scene object.
		SceneTree scene_tree{};
		scene_tree.AddNode(std::make_shared<SoftwareGL::SceneMesh>(gl_mesh));
		device_->SetSceneTree(scene_tree);

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
