#pragma once

namespace snbl {

class IndexBuffer {
public:
    IndexBuffer(unsigned int, const unsigned int *);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }

private:
    unsigned int m_RendererID;
    unsigned int m_Count;
};

} // namespace snbl