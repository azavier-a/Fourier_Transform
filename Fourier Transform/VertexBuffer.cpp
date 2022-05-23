#include "VertexBuffer.h"
#include "Eyad.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int type) {
	call(glGenBuffers(1, &m_RID));
	call(glBindBuffer(GL_ARRAY_BUFFER, m_RID));
	call(glBufferData(GL_ARRAY_BUFFER, size, data, type));
	call(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer() {
	call(glDeleteBuffers(1, &m_RID));
}

void VertexBuffer::Bind() const {
	call(glBindBuffer(GL_ARRAY_BUFFER, m_RID));
}

void VertexBuffer::Unbind() const {
	call(glBindBuffer(GL_ARRAY_BUFFER, 0));
}