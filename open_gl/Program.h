#pragma once

#include <optional>
#include "../open_gl/Shader.h"
#include "../open_gl/Buffer.h"
#include "../software_gl/VectorMath.h"

namespace OpenGL {

	class Program {
	public:
		// Create the program.
		Program();
		// Destructor
		virtual ~Program();
		// Attach shader to a program.
		void AddShader(const Shader& shader);
		// Link shaders to a program.
		void LinkShader();
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
			const VectorMath::matrix& mat,
			const bool flip = false) const;

	private:
		int program_id_ = 0;
	};

} // End namespace OpenGL.