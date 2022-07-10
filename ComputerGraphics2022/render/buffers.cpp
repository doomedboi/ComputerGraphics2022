#include "buffers.h"

VertexArrayObject::VertexArrayObject() { glGenVertexArrays(1, &id_.id_); }

void VertexArrayObject::bind() const { glBindVertexArray(id_.id_); }
void VertexArrayObject::unbind() const { glBindVertexArray(0); }
ID VertexArrayObject::getId() const { return id_; }


VertexBuffer::VertexBuffer(const std::vector<Vertex>& data, GLenum drawType) {
    glGenBuffers(1, &id_.id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_.id_);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), data.data(), drawType);
}

void VertexBuffer::bind() const { glBindVertexArray(id_.id_); }
void VertexBuffer::unbind() const { glBindVertexArray(0); }
void VertexBuffer::bindAttribute(size_t layout, size_t argumentCount,
    bool needNormalize, size_t vertexSize, size_t offsetFromStart) {
    glVertexAttribPointer(layout, argumentCount,
        GL_FLOAT, needNormalize, vertexSize, (void*)offsetFromStart);
    glEnableVertexAttribArray(layout);
}
void VertexBuffer::unbindAttribute(size_t layout) { glDisableVertexAttribArray(layout); }
ID VertexBuffer::getId() const { return id_; }