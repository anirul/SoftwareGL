#pragma once

#include "../gl_shader/Shader.h"
#include "../software_gl/VectorMath.h"

namespace OpenGL {

	class Program {
	public:
		// Create and attach the shader to the program.
		Program(const Shader& vertex, const Shader& fragment);
		// Destructor
		virtual ~Program();
		// Use the program.
		void Use() const;
		// Create a uniform from a string and a bool.
		void UniformBool(const std::string& name, bool value) const;
		// Create a uniform from a string and an int.
		void UniformInt(const std::string& name, int value) const;
		// Create a uniform from a string and a float.
		void UniformFloat(const std::string& name, float value) const;
		// Create a uniform from a string and a vector2.
		void UniformVector2(
			const std::string& name, 
			const VectorMath::vector2& vec2) const;
		// Create a uniform from a string and a vector3.
		void UniformVector3(
			const std::string& name,
			const VectorMath::vector3& vec3) const;
		// Create a uniform from a string and a vector4.
		void UniformVector4(
			const std::string& name,
			const VectorMath::vector4& vec4) const;
		// Create a uniform from a string and a matrix.
		void UniformMatrix(
			const std::string& name,
			const VectorMath::matrix& mat) const;

	private:
		unsigned int program_id_ = 0;
		unsigned int vertex_id_ = 0;
		unsigned int fragment_id_ = 0;
	};

} // End namespace OpenGL.