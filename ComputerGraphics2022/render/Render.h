#pragma once
#include "common.h"
#include "Vertex.h"
#include <vector>

// VBO wrapper used to megrate data from ram to gpu
class VertexBuffer {
private:
    ID id_;

public:
    // constructor takes vertex array
    VertexBuffer(const std::vector<Vertex>& data, GLenum drawType);

    void bind() const;

    void unbind() const;

    void bindAttribute(size_t layout, size_t argumentCount,
        bool needNormalize, size_t vertexSize, size_t offsetFromStart);

    void unbindAttribute(size_t layout);

    ID getId() const;

};

// VAO wrapper
class VertexArrayObject {
public:
    VertexArrayObject();
    void bind() const;
    void unbind() const;
    ID getId() const;
private:
    ID id_;
};

