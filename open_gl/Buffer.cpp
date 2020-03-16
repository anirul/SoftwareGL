#include "Buffer.h"
#include <exception>
#include <stdexcept>

namespace OpenGL {


	Buffer::Buffer(
		const BufferType buffer_type /*= BufferType::ARRAY_BUFFER*/, 
		const BufferUsage buffer_usage /*= BufferUsage::STATIC_DRAW*/) :
		buffer_type_(buffer_type),
		buffer_usage_(buffer_usage)
	{
		glGenBuffers(1, &buffer_object_);
	}

	void Buffer::Copy(const size_t size, const void* data /*= nullptr*/) const
	{
		Bind();
		glBufferData(
			static_cast<GLenum>(buffer_type_), 
			size, 
			data, 
			static_cast<GLenum>(buffer_usage_));
		UnBind();
	}

	void Buffer::Bind() const
	{
		glBindBuffer(static_cast<GLenum>(buffer_type_), buffer_object_);
	}

	void Buffer::UnBind() const
	{
		glBindBuffer(static_cast<GLenum>(buffer_type_), 0);
	}

	void* Buffer::Map(const size_t size)
	{
		if (buffer_usage_ == BufferUsage::STATIC_DRAW)
		{
			throw std::runtime_error("Static draw object cannot be mapped!");
		}
		return glMapBufferRange(
			static_cast<GLenum>(buffer_type_),
			0,
			size,
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	}

	void Buffer::UnMap() const
	{
		if (buffer_usage_ == BufferUsage::STATIC_DRAW)
		{
			throw std::runtime_error("Static draw object cannot be unmapped!");
		}
		glUnmapBuffer(static_cast<GLenum>(buffer_type_));
	}

} // End namespace OpenGL.