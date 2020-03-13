#pragma once

#include <memory>
#include <map>
#include <array>
#include <SDL2/SDL.h>
#include "Program.h"
#include "Texture.h"
#include "../software_gl/VectorMath.h"
#include "../software_gl/Mesh.h"
#include "../software_gl/Scene.h"

namespace OpenGL {

	class Device
	{
	public:
		Device(SDL_Window* sdl_window);
		virtual ~Device();

	public:
		bool Startup();
		void Draw();
		// Scene management.
		bool SetScene(std::shared_ptr<SoftwareGL::Scene> scene);
		// Mesh management.
		bool AddMesh(
			const std::string& name, 
			std::shared_ptr<SoftwareGL::Mesh> mesh);
		bool RemoveMesh(const std::string& name);
		// Shader management.
		void AddShader(const Shader& shader);
		bool LinkShader();
		// Texture management.
		bool AddTexture(
			const std::string& name, 
			std::shared_ptr<Texture> texture);
		bool RemoveTexture(const std::string& name);
		void EnableTexture(const std::string& name);
		void DisableTexture(const std::string& name);
		// MVP management (set the correct matrix for the scene).
		void SetProjection(const VectorMath::matrix& projection);
		void SetView(const VectorMath::matrix& view);
		void SetModel(const VectorMath::matrix& model);

	private:
		Program program_;
		std::vector<Shader> shaders_;
		std::shared_ptr<SoftwareGL::Scene> scene_;
		std::map<std::string, std::shared_ptr<Texture>> name_texture_map_;
		std::array<std::string, 32> name_array_;
		std::map<std::string, std::shared_ptr<SoftwareGL::Mesh>> name_mesh_map_;
		SDL_Window* sdl_window_ = nullptr;
		SDL_GLContext sdl_gl_context_ = nullptr;
		GLuint index_buffer_object_ = 0;
		int major_version_ = 0;
		int minor_version_ = 0;
	};

} // End namespace OpenGL.