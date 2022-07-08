#include "Mesh.h"

void CMesh::CreateBuffers(std::vector<Vertex>& verts,
                        const std::vector<uint>& indexes) {
    vertexArray_.bind();
    vertexBuffer_ = std::make_unique<VertexBuffer>(verts);
    indexBuffer_ = std::make_unique<IndexBuffer>(indexes);

    constexpr auto vertexStride = 8;
    vertexBuffer_->bind();
    vertexBuffer_->bindAttribute(0, 3, false, vertexCount_, offsetof(Vertex, pos));
    vertexBuffer_->bindAttribute(1, 2, false, vertexCount_, offsetof(Vertex, uv));
    vertexBuffer_->bindAttribute(2, 3, false, vertexCount_, offsetof(Vertex, norm));

    vertexArray_.unbind();
    vertexBuffer_->unbind();
}