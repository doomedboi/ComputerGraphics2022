#pragma once
#include <vector>
#include "Vertex.h"
#include "Render.h"
#include <memory>


class CMesh {
public:
    using uint = unsigned;
    CMesh(std::vector<Vertex>& verts,
        const std::vector<uint>& indexes, uint matIndex) :
        vertices_(verts), indexes_(indexes) ,vertexCount_(verts.size()),
        indexesCount_(indexes.size()) ,materialIndex_(materialIndex_) {
        CreateBuffers(verts, indexes);
    }
    
    void Bind() { vertexArray_.bind(); }
    void Unbind() { vertexArray_.unbind(); }
    auto GetVertexCount() { return vertexCount_; }
    auto GetIndexesCount() { return indexesCount_; }

    std::unique_ptr<IndexBuffer> indexBuffer_;

private:
    void CreateBuffers(std::vector<Vertex>& verts, const std::vector<uint>& indexes);

private:
    std::vector<Vertex> vertices_;
    std::vector<uint> indexes_;
    const uint vertexCount_;
    const uint indexesCount_;
    const uint materialIndex_;
    std::unique_ptr<VertexBuffer> vertexBuffer_;
    VertexArrayObject vertexArray_;
    

};