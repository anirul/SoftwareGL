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

	bool Device::Startup(std::pair<int, int> size)
	{
		// Vertex Shader program.
		OpenGL::Shader vertex_shader(GL_VERTEX_SHADER);
		if (!vertex_shader.LoadFromFile("../asset/vertex.glsl"))
		{
			if (func_)
			{
				func_(
					"Fragment shader Error: " +
					vertex_shader.GetErrorMessage());
			}
			return false;
		}

		// Fragment Shader program.
		OpenGL::Shader fragment_shader(GL_FRAGMENT_SHADER);
		if (!fragment_shader.LoadFromFile("../asset/fragment.glsl"))
		{
			if (func_)
			{
				func_(
					"Fragment shader Error: " + 
					fragment_shader.GetErrorMessage());
			}
			return false;
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

		// Vertex attribute initialization.
		GLuint vertex_attribute_object = 0;
		glGenVertexArrays(1, &vertex_attribute_object);
		glBindVertexArray(vertex_attribute_object);

		return true;
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

			// Set uniform matrix.
			program_->UniformMatrix("model", value.first);
			
			glBindBuffer(GL_ARRAY_BUFFER, value.second->PointObject());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, value.second->NormalObject());
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, value.second->TextureObject());
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			// Enable vertex attrib array.
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			// Bind textures.
			for (const std::string& texture : value.second->GetTextures())
			{
				EnableTexture(texture);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value.second->IndexObject());
			glDrawElements(
				GL_TRIANGLES,
				static_cast<GLsizei>(value.second->IndexSize()),
				GL_UNSIGNED_INT,
				nullptr);

			// Unbind no more used textures.
			for (const std::string& texture : value.second->GetTextures())
			{
				DisableTexture(texture);
			}
		}
	}

	void Device::SetSceneTree(const SoftwareGL::SceneTree& scene_tree)
	{
		scene_tree_ = scene_tree;
	}

	void Device::AddShader(const Shader& shader)
	{
		program_->AddShader(shader);
	}

	void Device::LinkShader()
	{
		program_->LinkShader();
	}

	bool Device::AddTexture(
		const std::string& name, 
		std::shared_ptr<Texture> texture)
	{
		auto ret = name_texture_map_.insert({ name, texture });
		return ret.second;
	}

	bool Device::RemoveTexture(const std::string& name)
	{
		auto it = name_texture_map_.find(name);
		if (it == name_texture_map_.end())
		{
			return false;
		}
		name_texture_map_.erase(it);
		return true;
	}

	void Device::EnableTexture(const std::string& name)
	{
		auto it1 = name_texture_map_.find(name);
		if (it1 == name_texture_map_.end())
		{
			throw std::runtime_error("try to enable a texture: " + name);
		}
		for (int i = 0; i < name_array_.size(); ++i)
		{
			if (name_array_[i].empty())
			{
				name_array_[i] = name;
				it1->second->Bind(i);
				return;
			}
		}
		throw std::runtime_error("No free slots!");
	}

	void Device::DisableTexture(const std::string& name)
	{
		auto it1 = name_texture_map_.find(name);
		if (it1 == name_texture_map_.end())
		{
			throw std::runtime_error("no texture named: " + name);
		}
		auto it2 = std::find_if(
			name_array_.begin(),
			name_array_.end(),
			[name](const std::string& value)
		{
			return value == name;
		});
		if (it2 != name_array_.end())
		{
			*it2 = "";
			it1->second->UnBind();
		}
		else
		{
			throw std::runtime_error("No slot bind to: " + name);
		}
	}

	void Device::SetProjection(const VectorMath::matrix& projection)
	{
		program_->UniformMatrix("projection", projection);
	}

	void Device::SetView(const VectorMath::matrix& view)
	{
		program_->UniformMatrix("view", view);
	}

	void Device::SetModel(const VectorMath::matrix& model)
	{
		program_->UniformMatrix("model", model);
	}

	void Device::SetCamera(const SoftwareGL::Camera& camera)
	{
		camera_ = camera;
	}

	std::pair<int, int> Device::GetGLVersion() const
	{
		return std::make_pair(major_version_, minor_version_);
	}

	void Device::SetCallbackError(
		std::function<void(const std::string&)> func)
	{
		func_ = func;
	}

} // End namespace OpenGL.