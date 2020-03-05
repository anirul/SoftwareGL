#include "Program.h"

namespace OpenGL {

	Program::Program(const Shader& vertex, const Shader& fragment)
	{
		program_id_ = glCreateProgram();
		glAttachShader(program_id_, vertex.GetId());
		glAttachShader(program_id_, fragment.GetId());
		glLinkProgram(program_id_);
	}

	Program::~Program()
	{
		glDeleteProgram(program_id_);
	}

	void Program::Use() const
	{
		glUseProgram(program_id_);
	}

	void Program::UniformBool(const std::string& name, bool value) const
	{
		glUniform1i(
			glGetUniformLocation(program_id_, name.c_str()), (int)value);
	}

	void Program::UniformInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
	}

	void Program::UniformFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
	}

	void Program::UniformVector2(
		const std::string& name, 
		const VectorMath::vector2& vec2) const
	{
		glUniform2f(
			glGetUniformLocation(program_id_, name.c_str()), vec2.x, vec2.y);
	}

	void Program::UniformVector3(
		const std::string& name, 
		const VectorMath::vector3& vec3) const
	{
		glUniform3f(
			glGetUniformLocation(program_id_, name.c_str()), 
			vec3.x, 
			vec3.y, 
			vec3.z);
	}

	void Program::UniformVector4(
		const std::string& name, 
		const VectorMath::vector4& vec4) const
	{
		glUniform4f(
			glGetUniformLocation(program_id_, name.c_str()),
			vec4.x,
			vec4.y,
			vec4.z,
			vec4.w);
	}

	void Program::UniformMatrix(
		const std::string& name, 
		const VectorMath::matrix& mat) const
	{
		glUniformMatrix4fv(
			glGetUniformLocation(program_id_, name.c_str()), 
			1, 
			GL_TRUE, // CHECKME: Not sure about transpose.
			&mat._11);
	}

} // End namespace OpenGL.