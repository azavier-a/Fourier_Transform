#include "IndexBuffer.h"
#include "Eyad.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, unsigned int type) : m_Count(count) {
	ASSERT(sizeof(unsigned int) == sizeof(GLuint))

	call(glGenBuffers(1, &m_RID));
	call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RID));
	call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, type));
}

IndexBuffer::~IndexBuffer() {
	call(glDeleteBuffers(1, &m_RID));
}

void IndexBuffer::Bind() const {
	call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RID));
}

void IndexBuffer::Unbind() const {
	call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}