#pragma once

#include <cstddef>                      // std::size_t

namespace snbl {

class VertexBuffer {
public:
    VertexBuffer(std::size_t, const void *);
    ~VertexBuffer();

    void Bind();
    void Unbind();

private:
    unsigned int m_RendererID;
};

} // namespace snbl