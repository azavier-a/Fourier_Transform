#pragma once

class IndexBuffer {
private:
	unsigned int m_RID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count, unsigned int type);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};