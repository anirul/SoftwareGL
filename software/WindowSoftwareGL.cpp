#include "WindowSoftwareGL.h"

#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <chrono>
#include <ctime>

bool WindowSoftwareGL::Startup(const std::pair<int, int>& gl_version)
{
	const float aspect = 
		renderer_.GetImage().GetWidth() / renderer_.GetImage().GetHeight();
	// Create a projection matrix with 90 opening angle.
	projection_ = VectorMath::Projection(
		65.0f * static_cast<float>(M_PI) / 180.0f,
		aspect,
		0.1f,
		1000.0f);
	look_at_ = cam_.LookAt();
	look_at_.Inverse();
 	if (!mesh_.LoadFromObj(R"(../../asset/CubeUVNormal.obj)")) assert(false);
	//	if (!mesh_.LoadFromObj(".\\TorusUVNormal.obj")) assert(false);
	//	if (!mesh_.LoadFromFile(".\\Torus.obj")) assert(false);
	SoftwareGL::Image texture(1, 1);
	if (!texture.LoadFromTGA(R"(../../asset/Texture.tga)")) assert(false);
	renderer_.SetTexture(texture);
	return true;
}

bool WindowSoftwareGL::RunCompute()
{
	// Cleanup of buffers.
	renderer_.ClearFrame({ .2f, 0.f, .2f, 1.f }, z_max_);
	// Timing counter.
	static auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	VectorMath::matrix rotation;
	{
		VectorMath::matrix r_x;
		VectorMath::matrix r_y;
		VectorMath::matrix r_z;
		r_x.RotateXMatrix(time.count() * 0.7f);
		r_y.RotateYMatrix(time.count() * 0.5f);
		r_z.RotateZMatrix(time.count());
		rotation = r_x * r_y * r_z;
	}
	// Make a copy of mesh to make the view transformations.
	SoftwareGL::Mesh mesh = mesh_;
	mesh.AllPositionMatrixMult(rotation);
	mesh.AllNormalMatrixMult(rotation);
	mesh.AllPositionMatrixMult(look_at_);
	mesh.AllPositionMatrixMult(projection_);
	// mesh.AllTextureDivideByZ();
	mesh.AllPositionDivideByW();
	mesh.AllPositionAdd(1.0f);
	mesh.AllPositionMult(0.5f);
	mesh.AllPositionMult(
		VectorMath::vector(
			static_cast<float>(width_),
			static_cast<float>(height_),
			1,
			1));
	// Compute for every triangle.
	for (const SoftwareGL::Triangle& triangle_in : mesh)
	{
		renderer_.DrawTriangle(triangle_in);
	}
	return true;
}

bool WindowSoftwareGL::RunEvent(const SDL_Event& event)
{
	if (event.type == SDL_QUIT)
	{
		return false;
	}
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return false;
		}
	}
	return true;
}

const std::pair<size_t, size_t> WindowSoftwareGL::GetWindowSize() const
{
	return std::make_pair(width_, height_);
}
