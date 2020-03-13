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
			throw std::runtime_error("couldn't initialize GLEW");
		}
	}

	Device::~Device() {}

	bool Device::Startup()
	{

		// Move this to the mesh class, and use the scene_ pointer to move
		// around the scene and initialize it.

		

		// Post this should be moved to the Draw method.

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
	}

	void Device::Draw()
	{
		for (auto value : *scene_)
		{
			
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
		}
	}

	void Device::AddShader(const Shader& shader)
	{
		program_.AddShader(shader);
	}

	bool Device::LinkShader()
	{
		program_.LinkShader();
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
		program_.UniformMatrix("projection", projection);
	}

	void Device::SetView(const VectorMath::matrix& view)
	{
		program_.UniformMatrix("view", view);
	}

	void Device::SetModel(const VectorMath::matrix& model)
	{
		program_.UniformMatrix("model", model);
	}

	std::pair<int, int> Device::GetGLVersion() const
	{
		return std::make_pair(major_version_, minor_version_);
	}

} // End namespace OpenGL.