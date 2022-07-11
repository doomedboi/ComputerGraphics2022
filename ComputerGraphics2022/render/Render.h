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
    void setMenuState(bool);
    bool getMenuState();
    
    
    std::shared_ptr<sf::RenderWindow> getWnd();
    void ProcessIvents(sf::Event);
private:
    class Menu {
    public:
        void BeginFrame();
        void EndFrame();
        void DrawMainMenu();
        void DrawObjectsMenu(Scene&);
        void DrawDetailObjectsMenu(Scene&);
        void DrawCreateObjectMenu(Scene&);
        void DrawWorldMenu(Scene&, Renderer&);
        void DrawSettingsMenu(Renderer& render);

    private:
        sf::Clock deltaClock;
        float color[3] = { 0.f, 0.f, 0.f };
        char windowTitle[255] = "ImGui + SFML = <3";
        ImGuiStyle* style;
        bool openModal = true;
        
        //static int nowSelectedDetail;
    };
private:
    class Menu;
    void InitOpenGL();
    void InitImgui();
    void DrawMenu(Scene&);
private:
    std::shared_ptr<RenderBehavior> renderStrategy;
    std::shared_ptr<sf::RenderWindow> window_;
    sf::ContextSettings windowSettings_;
    bool menuOpen = true; // maybe move it to the menu class?
    float bgColor[3];
    Menu menu;

};
