#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement {
	unsigned int count, type;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type)
		{
		case GL_DOUBLE: 
		}
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {}

	template<typename T>
	void Push(int count) {
		static_assert(false);
	}

	template<>
	void Push<double>(int count) {
		m_Elements.push_back({GL_DOUBLE, count, GL_FALSE });
		m_Stride += sizeof(GLdouble);
	}

	template<>
	void Push<unsigned int>(int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += sizeof(GLuint);
	}

	template<>
	void Push<unsigned char>(int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += sizeof(GLubyte);
	}

	inline unsigned int GetStride() const { return m_Stride; }
	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
};