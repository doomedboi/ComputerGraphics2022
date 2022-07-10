#pragma once
//#include "Render.h"
#include "../render/Vertex.h"
#include "buffers.h"

#include <vector>
#include <memory>


class CMesh {
public:
    using uint = unsigned;
    CMesh(std::vector<Vertex>& verts,
        const std::vector<uint>& indexes, uint matIndex) :
        vertices_(verts), indexes_(indexes) ,vertexCount_(static_cast<unsigned>(verts.size())),
        indexesCount_(static_cast<unsigned>(indexes.size())) ,materialIndex_(materialIndex_) {
        CreateBuffers(verts, indexes);
    }
    
    void Bind();
    void Unbind();
    uint GetVertexCount();
    uint GetIndexesCount();

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