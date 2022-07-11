#include "Render.h"
#include "../core/Core.h"




Renderer::Renderer(int w, int h, int bitsPerPix, const std::string& title) :
    window_(std::make_shared<sf::RenderWindow>(sf::VideoMode(w, h, bitsPerPix), title.c_str(),
        sf::Style::Titlebar | sf::Style::Close)) {
   
    // in the future take it from user and move
    // to constructor to set in time of creation render window
    windowSettings_.depthBits = 24; 
    windowSettings_.stencilBits = 8; 
    windowSettings_.majorVersion = 4;
    windowSettings_.minorVersion = 3;

    InitOpenGL();
    InitImgui();
}

void Renderer::ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    glClearColor(red, green, blue, alpha);    
}

static int tab = -1;

void Renderer::DrawMenu(bool flag, Scene& sc) {
    sf::Clock deltaClock;
    float color[3] = { 0.f, 0.f, 0.f };
    char windowTitle[255] = "ImGui + SFML = <3";
    glClearColor(bgColor[0], bgColor[1], bgColor[2],1.f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window_->pushGLStates();
    // beginning of the menu    

    ImGui::SFML::Update(*window_, deltaClock.restart());
    //////////////////////////////////////////////////////
    
    if (flag)
        DrawMenu(sc);
                     
    
    ImGui::SFML::Render(*window_);
    window_->popGLStates();

    window_->display();
}

void Renderer::BeginFrame() {

}

int Renderer::GetHeight() {
    return window_->getSize().y;
}

int Renderer::GetWidth() {
    return window_->getSize().x;
}

void Renderer::DrawArrays(GLenum mode, int from, int to) {
    glDrawArrays(mode, from, to);
}

void Renderer::setMenuState(bool state) {
    menuOpen = state;
}

bool Renderer::getMenuState()
{
    return menuOpen;
}



std::shared_ptr<sf::RenderWindow> Renderer::getWnd() { return window_; }

// imgui::sfml wrapper
void Renderer::ProcessIvents(sf::Event event) {
    ImGui::SFML::ProcessEvent(event);
}

void Renderer::InitOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        _ASSERT("GLEW ain't inited");
    }
}

void Renderer::InitImgui() {
    ImGui::SFML::Init(*window_);
}


void Renderer::DrawMenu(Scene& sc) {
    if (!menuOpen)
        return;

    menu.BeginFrame();
    menu.DrawMainMenu();
    menu.DrawDetailObjectsMenu(sc);
    
    menu.DrawWorldMenu(sc, *this);
    menu.DrawObjectsMenu(sc);
    
    menu.DrawSettingsMenu(*this);
    
    menu.EndFrame();
}

void Renderer::Menu::BeginFrame() {
    ImGui::SetNextWindowSize(ImVec2(660.f, 170.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.6f, 0.6f, 0.6f, 0.2));


    ImGui::Begin("Sample window"); // создаём окно
}

void Renderer::Menu::EndFrame() {
    ImGui::PopStyleColor();
    ImGui::End(); // end window
    // end of the menu
}

void Renderer::Menu::DrawMainMenu() {
    ImGui::SameLine(50.f);
    if (ImGui::Button("Objects", ImVec2(180, 100))) {
        ImGui::OpenPopup("Objects");
    }  ImGui::SameLine();
    if (ImGui::Button("World", ImVec2(180, 100))) {
        ImGui::OpenPopup("World");
    }   ImGui::SameLine();
    if (ImGui::Button("Settings", ImVec2(180, 100))) {
        ImGui::OpenPopup("Settings");
    }

}

static int nowSelectedDetail;
static std::string modelPath, modelName;

void Renderer::Menu::DrawObjectsMenu(Scene& sc) {
    bool f = false;
    static bool select[255];
    

    bool open = true;
    static char selectedItem[255];
    static float translation[3];
    //static std::string modelPath, modelName;
    bool opendetails = true;
    //bool openworld = true;
    auto processSelected = []() {
        for (int i = 0; i < 255; i++) {
            if (select[i] == true) {
                ImGui::OpenPopup("Object Details");
                nowSelectedDetail = i;
                select[i] = false;
                break;
            }
        }
    };

    if (ImGui::BeginPopupModal("Objects", &open))
    {
        ImGui::SameLine(30);
        if (ImGui::Button("Create new object", ImVec2(600, 50))) {
            ImGui::OpenPopup("New object creation");
        }  ImGui::NewLine();

        DrawCreateObjectMenu(sc);
        
        
        if (ImGui::BeginTable("split", 2)) {
            int incolumn = 0;
            auto objects = sc.getObjects();
            for (int i = 0; i < objects.size(); i++) {
                ImGui::TableNextColumn();

                ImGui::Selectable(objects[i]->getName().c_str(), &select[i], 0, ImVec2(300, 50));

                incolumn++;
                if (incolumn > 5) {
                    incolumn = 0;
                    ImGui::TableNextRow();
                }
            }
            ImGui::EndTable();
            ImGui::EndPopup();
        }
    }
    processSelected();
}



void Renderer::Menu::DrawDetailObjectsMenu(Scene& sc) {
    static float translation[3];
    static std::string modelPath, modelName;
    bool opendetails = true;
    bool openworld = true;
    ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
    auto standartButtonSz = ImVec2(430, 50);
    

    if (ImGui::BeginPopupModal("Object Details", &opendetails)) {

        static char name[255] = "Type here to change name";
        auto coor = sc.getObjects()[nowSelectedDetail]->getPos();
        static char texturePath[300];
        static float scale[3];
        ImGui::Text("Name: "); ImGui::SameLine(50.f);
        ImGui::Text(sc.getObjects()[nowSelectedDetail]->getName().c_str());
        ImGui::InputText("##", name, 255, 0);
        if (ImGui::SliderFloat3("Position", translation, -1000.f, 1000.f)) {
            sc.getObjects()[nowSelectedDetail]->setPose(math::vec3(translation[0], translation[1], translation[2]));
        }
        if (ImGui::SliderFloat3("Scale", scale, 0.f, 0.3f)) {
            sc.getObjects()[nowSelectedDetail]->setScale(math::vec3(scale[0], scale[1], scale[2]));
        }
        if (ImGui::Button("Change Name", standartButtonSz)) {
            sc.getObjects()[nowSelectedDetail]->setName(name);
        }

        if (ImGui::Button("Change Model", standartButtonSz)) {
            ImGuiFileDialog::Instance()->OpenModal("ChooseModel", "Choose File", "Model files (*.fbx *.obj){.fbx,.obj}", ".\\res\\");
        }


        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("ChooseModel")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                modelPath = ImGuiFileDialog::Instance()->GetFilePathName();
                modelName = ImGuiFileDialog::Instance()->GetCurrentFileName();
            }
            sc.getObjects()[nowSelectedDetail]->loadModel(modelPath);
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Change texture", standartButtonSz)) {
            ImGuiFileDialog::Instance()->OpenModal("Choose new texture", "Choose File", "Texture files (*.png *.jpg){.png,.jpg}", ".\\res\\");
        }
        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("Choose new texture")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                strncpy_s(texturePath, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), sizeof(texturePath));
            }
            sc.getObjects()[nowSelectedDetail]->loadTexture(texturePath);
            ImGuiFileDialog::Instance()->Close();
        }
        if (ImGui::Button("Change normal texture", standartButtonSz)) {

        }
        if (ImGui::Button("Delete this object", standartButtonSz)) {
            sc.deleteEntity(sc.getObjects()[nowSelectedDetail]);
            ImGui::CloseCurrentPopup();
        }


        ImGui::EndPopup();
    }

}

void Renderer::Menu::DrawCreateObjectMenu(Scene&sc) {
    bool open = true;
    if (ImGui::BeginPopupModal("New object creation", &open)) {

        static float pos[3];
        static float scale[3];
        static char name[255];
        spin sp;
        static char modelPath[300] = "model path";
        static char texturePath[300] = "texture path";
        static char textureNPath[300] = "normal texture path";
        ImGui::InputFloat3("position", pos);
        ImGui::InputFloat3("scale", scale);
        ImGui::Text("spin...");
        ImGui::InputText("Object name", name, sizeof(name));
        ImGui::InputText("##", modelPath, sizeof(modelPath)); ImGui::SameLine(440);
        if (ImGui::Button("Load model")) {
            ImGuiFileDialog::Instance()->OpenModal("Choose new Model", "Choose File", "Model files (*.fbx *.obj){.fbx,.obj}", ".\\res\\");
        }

        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("Choose new Model")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                strncpy_s(modelPath, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), sizeof(modelPath));
                modelName = ImGuiFileDialog::Instance()->GetCurrentFileName();
            }

            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::InputText("##", texturePath, sizeof(texturePath)); ImGui::SameLine(440);
        if (ImGui::Button("Load texture")) {
            ImGuiFileDialog::Instance()->OpenModal("Choose new texture", "Choose File", "Texture files (*.png *.jpg){.png,.jpg}", ".\\res\\");
        }
        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("Choose new texture")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                strncpy_s(texturePath, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), sizeof(texturePath));
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Create object")) {
            if (strlen(name) == 0) {
                strncpy_s(name, modelName.c_str(), modelName.size());
            }

            //texture normal


            auto obj = std::make_shared<SceneObject>(math::vec3(pos[0], pos[1], pos[2]),
                math::vec3(scale[0], scale[1], scale[2]), sp);
            obj->setName(name);
            obj->loadModel(modelPath);
            obj->loadTexture(texturePath);
            sc.addEntity(obj);
        }
        ImGui::EndPopup();
    }

}

void Renderer::Menu::DrawWorldMenu(Scene& sc, Renderer& render) {
    static float light[4];
    static float vbgColor[3] = { 0.3, 0.5, 0.5 };
    static float cameraSpeed;
    static char vertexPath[255];
    bool open = true;
    if (ImGui::BeginPopupModal("World", &open)) {
        if (ImGui::SliderFloat3("Light from position", light, -100.f, 100.f)) {
            sc.setLight(math::vec3(light[0], light[1], light[2]));
        }
        if (ImGui::SliderFloat("Light's shine", &light[3], 0.f, 1.f)) {
            sc.setLightShine(light[3]);
        }
        if (ImGui::SliderFloat3("Background Color", vbgColor, 0.f, 1.f)) {
            render.bgColor[0] = vbgColor[0]; render.bgColor[1] = vbgColor[1]; render.bgColor[2] = vbgColor[2];
        }
        if (ImGui::SliderFloat("Camera speed", &cameraSpeed, 0, 1.f)) {
            Core::Singleton().GetViewCamera().setCameraSpeed(cameraSpeed);
        }
        ImGui::InputText("World's vertex shader path", vertexPath, sizeof(vertexPath)); //ImGui::SameLine(440);
        if (ImGui::Button("Choose vertex shader", ImVec2(100.f, 50.f))) {
            ImGuiFileDialog::Instance()->OpenModal("Choose shader", "Choose File", "vertex files (*.vs *.fs *.txt){.vs,.fs,.txt}", ".\\res\\");
        }


        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("Choose fragment shader")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                strncpy_s(vertexPath, ImGuiFileDialog::Instance()->GetCurrentPath().c_str(), sizeof(vertexPath));
            }

            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::SameLine(110.f);
        if (ImGui::Button("Set shaders", ImVec2(100.f, 50.f))) {
            auto shader = std::make_shared<Shader>(ShaderLoader::singleton().load(vertexPath));
            sc.setShader(shader);
        }

        ImGui::EndPopup();
    }
}

void Renderer::Menu::DrawSettingsMenu(Renderer& render) {
    bool open = true;
    ImGui::SetNextWindowSize(ImVec2(300.f, 300.f));
    if (ImGui::BeginPopupModal("Settings", &open)) {
        const char* res_listbox_items[] = { "3840x2160","2560x1440", "1920x1200", "1920x1080",
            "1600x900", "1366x768" };
        std::vector<sf::Vector2u> res_listbox_items_ = {
            {3840, 2160}, {2560, 1440}, {1920, 1200}, {1920, 1080}, {1600, 900}, {1366, 768}
        };

        static int res_listbox_item_current = 3;
        ImGui::Combo("Resolution", &res_listbox_item_current, res_listbox_items, IM_ARRAYSIZE(res_listbox_items));

        static char title[255] = "Type new title here";
        ImGui::InputText("Window title", title, sizeof(title));

        if (ImGui::Button("Change", ImVec2(195, 25))) {
            render.getWnd()->setSize(res_listbox_items_[res_listbox_item_current]);
            render.getWnd()->setTitle(title);
        }

        ImGui::EndPopup();
    }
}
