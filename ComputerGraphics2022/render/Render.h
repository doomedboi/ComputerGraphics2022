#pragma once
#include "common.h"
#include "Vertex.h"
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include "imgui-SFML.h"
#include "imgui.h"

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

struct RenderBehavior {
    virtual void draw() = 0;
};

struct Cube : public RenderBehavior {
    virtual void draw() override {

    }
};

class Renderer {
public:
    //addListener???
    Renderer(int w, int h, int bitsPerPix, const std::string& title);
    void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void DrawMenu();
    void BeginFrame();
    void EndFrame();
    int GetHeight();
    int GetWidth();
    void SetDrawPrimitive(RenderBehavior* drawStrategy);
    void DrawArrays(GLenum mode, int from, int to);
    void DrawPrimitive();
    
    std::shared_ptr<sf::RenderWindow> getWnd();
    void ProcessIvents(sf::Event);
private:
    void InitOpenGL();
    void InitImgui();
private:
    std::shared_ptr<RenderBehavior> renderStrategy;
    std::shared_ptr<sf::RenderWindow> window_;
    sf::ContextSettings windowSettings_;
};
