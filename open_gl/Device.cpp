#include <exception>
#include <stdexcept>
#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <iostream>
#endif
#include "Device.h"

namespace OpenGL {

	Device::Device(SDL_Window* sdl_window) 
	{
		// GL context.
		sdl_gl_context_ = SDL_GL_CreateContext(sdl_window);
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
			throw std::runtime_error("couldn't initialize GLEW");
		}

		// Enable blending to 1 - source alpha.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable Z buffer.
		glEnable(GL_DEPTH_TEST);

		// Create a program.
		program_ = std::make_shared<Program>();
	}

	Device::~Device() {}

	std::optional<std::string> Device::Startup(std::pair<int, int> size)
	{
		// Vertex Shader program.
		OpenGL::Shader vertex_shader(GL_VERTEX_SHADER);
		if (!vertex_shader.LoadFromFile("../asset/vertex.glsl"))
		{
			return "Fragment shader Error: " + vertex_shader.GetErrorMessage();
		}

		// Fragment Shader program.
		OpenGL::Shader fragment_shader(GL_FRAGMENT_SHADER);
		if (!fragment_shader.LoadFromFile("../asset/fragment.glsl"))
		{
			return "Fragment shader Error: " + 
				fragment_shader.GetErrorMessage();
		}

		// Create the program.
		program_->AddShader(vertex_shader);
		program_->AddShader(fragment_shader);
		program_->LinkShader();
		program_->Use();

		// Set the perspective matrix.
		const float aspect =
			static_cast<float>(size.first) / static_cast<float>(size.second);
		VectorMath::matrix perspective = VectorMath::Perspective(
			65.0f * static_cast<float>(M_PI) / 180.0f,
			aspect,
			0.1f,
			1000.0f);
		program_->UniformMatrix("projection", perspective);

		// Set the camera.
		VectorMath::matrix view = camera_.LookAt();
		program_->UniformMatrix("view", view);

		// Set the model matrix (identity for now).
		VectorMath::matrix model = {};
		program_->UniformMatrix("model", model);

		return std::nullopt;
	}

	void Device::Draw()
	{
		// Clear the screen.
		glClearColor(.2f, 0.f, .2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (const std::shared_ptr<SoftwareGL::Scene>& scene : scene_tree_)
		{
			const auto& value = scene->GetLocalModelAndMesh();
			if (!value.second)
			{
				continue;
			}

			// Draw the mesh.
			value.second->Draw(*program_, texture_manager_, value.first);
		}
	}

	void Device::SetSceneTree(const SoftwareGL::SceneTree& scene_tree)
	{
		scene_tree_ = scene_tree;
	}

	void Device::SetTextureManager(
		const OpenGL::TextureManager& texture_manager)
	{
		texture_manager_ = texture_manager;
	}

	void Device::SetCamera(const SoftwareGL::Camera& camera)
	{
		camera_ = camera;
	}

	std::pair<int, int> Device::GetGLVersion() const
	{
		return std::make_pair(major_version_, minor_version_);
	}

} // End namespace OpenGL.