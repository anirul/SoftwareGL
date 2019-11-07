#include "WindowSoftwareGL.h"

#include <GL/glew.h>
#include <SDL.h>
#include <fstream>
#include <chrono>
#include <ctime>

bool WindowSoftwareGL::Startup(const std::pair<int, int>& gl_version)
{
	glGenTextures(1, &texture_id_);
	const float aspect = image_.GetWidth() / image_.GetHeight();
	// Create a projection matrix with 90 opening angle.
	projection_ = VectorMath::Projection(
		90.0f * static_cast<float>(M_PI) / 180.0f,
		aspect,
		0.1f,
		1000.0f);
	look_at_ = cam_.LookAt();
	look_at_.Inverse();
	translation_.IdentityMatrix();
  	translation_.TranslateMatrix(
  		aspect, 
  		z_min_, 
  		z_max_);
	scale_.IdentityMatrix();
	scale_.ScaleMatrix(static_cast<float>(height_) / 2);
	if (!mesh_.LoadFromFile(".\\Cube.obj")) assert(false);
	vertex_.resize(mesh_.GetPoints().size());
	z_buffer_.resize(height_ * width_);
	return true;
}

bool WindowSoftwareGL::RunCompute()
{
	// Cleanup of buffers.
	//TODO Should clean to the correct ambient light.
	std::fill(
		image_.begin(), 
		image_.end(),	
		VectorMath::vector4(.2f, .0f, .2f, 1.f));
	std::fill(z_buffer_.begin(), z_buffer_.end(), z_max_);
	// Timing counter.
	static auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<float> time = end - start;
	// Main vertex loop
	const auto& points = mesh_.GetPoints();
	const auto& indices = mesh_.GetIndices();
	vertex_.assign(points.cbegin(), points.cend());
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
	// Do the transformations.
	std::for_each(
		vertex_.begin(), 
		vertex_.end(), 
		[this, rotation](SoftwareGL::Vertex& vertex) {
		vertex.SetPosition(
			VectorMath::VectorMult(vertex.GetPosition(), rotation));
		vertex.SetPosition(
			VectorMath::VectorMult(vertex.GetPosition(), look_at_));
		vertex.SetPosition(
			VectorMath::VectorMult(vertex.GetPosition(), projection_));
		if (vertex.GetW() != 0.0f) 
		{
			vertex.SetPosition(VectorMath::vector(
				vertex.GetX() / vertex.GetW(),
				vertex.GetY() / vertex.GetW(),
				vertex.GetZ() / vertex.GetW()));
		}
		vertex.SetPosition((vertex.GetPosition() + 1.f) * .5f);
		vertex.SetPosition(VectorMath::vector(
			vertex.GetX() * width_,
			vertex.GetY() * height_,
			vertex.GetZ(),
			vertex.GetW()));
	});
	for (int i = 0; i < indices.size(); i += 3)
	{
		int i0 = i;
		int i1 = i + 1;
		int i2 = i + 2;
		// Compute normal to a triangle (before transform).
		VectorMath::vector normal = 
			[&i0, &i1, &i2, &points, &indices, &rotation] {
			VectorMath::vector p0 = points[indices[i0]].GetPosition();
			VectorMath::vector p1 = points[indices[i1]].GetPosition();
			VectorMath::vector p2 = points[indices[i2]].GetPosition();
			p0 = VectorMath::VectorMult(p0, rotation);
			p1 = VectorMath::VectorMult(p1, rotation);
			p2 = VectorMath::VectorMult(p2, rotation);
			return ((p1 - p0) % (p2 - p0)).Normalize();
		}();
		if (normal * cam_.Direction() <= 0.0f) {
			// Create a triangle
			SoftwareGL::Triangle triangle(
				vertex_[indices[i0]],
				vertex_[indices[i1]],
				vertex_[indices[i2]]);
			image_.DrawTriangle(triangle, normal, z_buffer_);
		}
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

