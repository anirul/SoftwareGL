#pragma once

#include <memory>
#include <map>
#include <array>
#include <functional>
#include <optional>
#include <SDL2/SDL.h>
#include "Program.h"
#include "Texture.h"
#include "../open_gl/Buffer.h"
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
		// Return an error string with the current error none if there is no
		// errors.
		std::optional<std::string> Startup(std::pair<int, int> size);
		// Draw what is on the scene.
		void Draw();
		// Scene management.
		void SetSceneTree(const SoftwareGL::SceneTree& scene_tree);
		// Set texture manager.
		void SetTextureManager(const OpenGL::TextureManager& texture_manager);
		// Set the camera.
		void SetCamera(const SoftwareGL::Camera& camera);

	public:
		// Get the GL version.
		std::pair<int, int> GetGLVersion() const;

	private:
		std::shared_ptr<Program> program_;
		std::vector<Shader> shaders_ = {};
		SoftwareGL::SceneTree scene_tree_ = {};
		std::shared_ptr<OpenGL::Buffer> model_buffer_ = nullptr;
		OpenGL::TextureManager texture_manager_ = {};
		SDL_GLContext sdl_gl_context_ = nullptr;
		int major_version_ = 0;
		int minor_version_ = 0;
		SoftwareGL::Camera camera_ = SoftwareGL::Camera({ 0, 0, -4 });
		GLuint vertex_attribute_object_ = 0;
	};

} // End namespace OpenGL.