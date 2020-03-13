#pragma once

#include "../software_gl/Mesh.h"

namespace OpenGL {

	class Mesh
	{
	public:
		Mesh(const SoftwareGL::Mesh& mesh);

	public:
		unsigned int PointObject() const { return point_buffer_object_; }
		unsigned int NormalObject() const { return normal_buffer_object_; }
		unsigned int TextureObject() const { return texture_buffer_object_; }
		unsigned int IndexObject() const { return index_buffer_object_; }

	private:
		unsigned int point_buffer_object_;
		unsigned int normal_buffer_object_;
		unsigned int texture_buffer_object_;
		unsigned int index_buffer_object_;
	};

} // End namespace OpenGL