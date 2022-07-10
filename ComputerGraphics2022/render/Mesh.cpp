#include "Mesh.h"

void CMesh::CreateBuffers(std::vector<Vertex>& verts,
                        const std::vector<uint>& indexes) {
    vertexArray_.bind();
    vertexBuffer_ = std::make_unique<VertexBuffer>(verts);
    indexBuffer_ = std::make_unique<IndexBuffer>(indexes);

    constexpr auto vertexStride = sizeof(Vertex);
    vertexBuffer_->bind();
    indexBuffer_->bind();

    vertexBuffer_->bindAttribute(0, 3, false, vertexStride, offsetof(Vertex, pos));
    vertexBuffer_->bindAttribute(1, 2, false, vertexStride, offsetof(Vertex, uv));
    vertexBuffer_->bindAttribute(2, 3, false, vertexStride, offsetof(Vertex, norm));

    vertexArray_.unbind();
    vertexBuffer_->unbind();
    vertexBuffer_->unbindAttribute(0);
}

void CMesh::Bind() { vertexArray_.bind(); }
void CMesh::Unbind() { vertexArray_.unbind(); }
unsigned CMesh::GetVertexCount() { return vertexCount_; }
unsigned CMesh::GetIndexesCount() { return indexesCount_; }