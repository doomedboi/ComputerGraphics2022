#pragma once
                   

#include "common.h"
#include "Vertex.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"


#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "./Model.h"


#include "../core/Scene.h"
#include "../render/ShaderLoader.h"
#include "../render/Shader.h"
#include "../scene/SceneObject.h"


struct RenderBehavior {
    virtual void draw() = 0;
};

struct Cube : public RenderBehavior {
    virtual void draw() override {

    }
};

class Renderer {
public:
    friend class Scene;
    //addListener???
    Renderer(int w, int h, int bitsPerPix, const std::string& title);
    void ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void DrawMenu(bool flag, Scene&);
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
    void Menu(Scene&);
private:
    std::shared_ptr<RenderBehavior> renderStrategy;
    std::shared_ptr<sf::RenderWindow> window_;
    sf::ContextSettings windowSettings_;
    bool menuOpen = true;
    float bgColor[3];
};
