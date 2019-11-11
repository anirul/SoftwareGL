#include "WindowSoftwareGL.h"

#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <chrono>
#include <ctime>

bool WindowSoftwareGL::Startup(const std::pair<int, int>& gl_version)
{
	glGenTextures(1, &texture_id_);
	const float aspect = 
		renderer_.GetImage().GetWidth() / renderer_.GetImage().GetHeight();
	// Create a projection matrix with 90 opening angle.
	projection_ = VectorMath::Projection(
		90.0f * static_cast<float>(M_PI) / 180.0f,
		aspect,
		0.1f,
		1000.0f);
	look_at_ = cam_.LookAt();
	look_at_.Inverse();
	if (!mesh_.LoadFromFile(".\\CubeUVNormal.obj")) assert(false);
//	if (!mesh_.LoadFromFile(".\\TorusUVNormal.obj")) assert(false);
//	if (!mesh_.LoadFromFile(".\\Torus.obj")) assert(false);
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
		r_x.RotateXMatrix(time.count() * 0.3f);
		r_y.RotateYMatrix(time.count() * 0.5f);
		r_z.RotateZMatrix(time.count());
		rotation = r_x * r_y * r_z;
	}
	// Compute for every triangle.
	for (const SoftwareGL::Triangle& triangle_in : mesh_)
	{
		// Create a triangle
		SoftwareGL::Triangle triangle(triangle_in);
		triangle = triangle.AllPositionMatrixMult(rotation);
		triangle = triangle.AllNormalMatrixMult(rotation);
		triangle = triangle.AllPositionMatrixMult(look_at_);
		triangle = triangle.AllPositionMatrixMult(projection_);
		triangle = triangle.AllPositionDivideByW();
		triangle = triangle.AllPositionAdd(1.0f);
		triangle = triangle.AllPositionMult(0.5f);
		triangle = triangle.AllPositionMult(
			VectorMath::vector(
				static_cast<float>(width_), 
				static_cast<float>(height_), 
				1, 
				1));
		renderer_.DrawTriangle(triangle);
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

