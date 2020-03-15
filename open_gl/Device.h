#pragma once

#include <memory>
#include <map>
#include <array>
#include <functional>
#include <SDL2/SDL.h>
#include "Program.h"
#include "Texture.h"
#include "../software_gl/VectorMath.h"
#include "../software_gl/Mesh.h"
#include "../software_gl/Scene.h"
#include "../software_gl/Camera.h"

namespace OpenGL {

	class Device
	{
	public:
		Device(SDL_Window* sdl_window);
		virtual ~Device();

	public:
		// Startup the scene.
		bool Startup(std::pair<int, int> size);
		// Draw what is on the scene.
		void Draw();
		// Scene management.
		void SetSceneTree(const SoftwareGL::SceneTree& scene_tree);
		// Shader management.
		void AddShader(const Shader& shader);
		void LinkShader();
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
		// Set the camera.
		void SetCamera(const SoftwareGL::Camera& camera);
		// Get the GL version.
		std::pair<int, int> GetGLVersion() const;
		// Set a function to allow error to be displayed.
		void SetCallbackError(std::function<void(const std::string&)> func);

	private:
		std::shared_ptr<Program> program_;
		std::vector<Shader> shaders_ = {};
		SoftwareGL::SceneTree scene_tree_ = {};
		std::map<std::string, std::shared_ptr<Texture>> name_texture_map_;
		std::array<std::string, 32> name_array_;
		std::function<void(const std::string&)> func_ = nullptr;
		SDL_GLContext sdl_gl_context_ = nullptr;
		int major_version_ = 0;
		int minor_version_ = 0;
		SoftwareGL::Camera camera_ = SoftwareGL::Camera({ 0, 0, -4 });
	};

} // End namespace OpenGL.