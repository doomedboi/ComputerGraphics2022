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
    VertexBuffer(const std::vector<Vertex>& data, GLenum drawType = GL_STATIC_DRAW);

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


class IndexBuffer {
public:
    IndexBuffer(const std::vector<unsigned>& indeces, GLenum drawType = GL_STATIC_DRAW) {
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(unsigned int), indeces.data(), GL_STATIC_DRAW);
        bind();

    }
    void bind() {  }
    void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
    ID getId() { return id_.id_; }
private:
    GLuint elementbuffer;
    ID id_;
};