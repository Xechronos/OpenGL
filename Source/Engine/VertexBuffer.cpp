#include "EnginePCH.h"
#include "VertexBuffer.h"

namespace neu {
	VertexBuffer::VertexBuffer()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_ibo) glDeleteBuffers(1, &m_ibo);
		if (m_vbo) glDeleteBuffers(1, &m_vbo);
		if (m_ibo) glDeleteVertexArrays(1, &m_vao);
	}

	void VertexBuffer::Draw(GLenum primitiveType)
	{
	}

	void VertexBuffer::CreateVertexBuffer(GLsizei size, GLsizei count, GLvoid* data)
	{
	}

	void VertexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei count, GLvoid* data)
	{
	}

	void VertexBuffer::SetAttribute(int index, GLint size, GLsizei stride, GLuint offset)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	}
}