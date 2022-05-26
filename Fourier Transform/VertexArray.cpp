#include "VertexArray.h"
#include "Eyad.h"

VertexArray::VertexArray() {
	call(glGenVertexArrays(1, &m_RID));
}

VertexArray::~VertexArray() {
	call(glDeleteVertexArrays(1, &m_RID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		call(glEnableVertexAttribArray(i));
		call(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	call(glBindVertexArray(m_RID));
}

void VertexArray::Unbind() const {
	call(glBindVertexArray(0));
}
