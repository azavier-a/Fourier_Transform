#pragma once

class VertexBuffer {
private:
	unsigned int m_RID;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int type);
	~VertexBuffer();

	void Bind()  const;
	void Unbind() const;
};