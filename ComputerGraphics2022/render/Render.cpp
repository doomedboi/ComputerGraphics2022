#include "Render.h"




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

    window_->pushGLStates();
    // beginning of the menu    

    ImGui::SFML::Update(*window_, deltaClock.restart());
    //////////////////////////////////////////////////////
    
    if (flag)
        Menu(sc);
    
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



std::shared_ptr<sf::RenderWindow> Renderer::getWnd() { return window_; }

// imgui::sfml wrapper
void Renderer::ProcessIvents(sf::Event event) {
    ImGui::SFML::ProcessEvent(event);
}

void Renderer::InitOpenGL() {
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        _ASSERT("GLEW ain't inited");
    }
}

void Renderer::InitImgui() {
    ImGui::SFML::Init(*window_);
}


void Renderer::Menu(Scene& sc)
{

    auto objects = sc.getObjects();

    
    sf::Clock deltaClock;
    float color[3] = { 0.f, 0.f, 0.f };
    char windowTitle[255] = "ImGui + SFML = <3";

    ImGuiStyle* style = &ImGui::GetStyle();


    style->WindowPadding = ImVec2(6, 6);
    ImGui::SetNextWindowSize(ImVec2(660.f, 170.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.6f, 0.6f, 0.6f, 0.2));


    ImGui::Begin("Sample window"); // создаём окно

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

    bool f = false;
    static bool select[255];
    static int nowSelectedDetail;

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

    
    static float translation[3];
    static std::string modelPath, modelName;
    bool opendetails = true;
    bool openworld = true;
    ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
    if (ImGui::BeginPopupModal("Object Details", &opendetails)) {

        static char name[255] = "Type here to change name";
        auto coor = objects[nowSelectedDetail]->getPos();
        static char texturePath[300];
        ImGui::Text("Name: "); ImGui::SameLine(50.f);
        ImGui::Text(objects[nowSelectedDetail]->getName().c_str());
        ImGui::InputText("##", name, 255, 0); 
        if (ImGui::SliderFloat3("Position", translation, -1000.f, 1000.f)) {
            objects[nowSelectedDetail]->setPose(math::vec3(translation[0], translation[1], translation[2]));
        }
        if (ImGui::Button("Change Name")) {
            objects[nowSelectedDetail]->setName(name);
        }
        
        if (ImGui::Button("Change Model")) {
            ImGuiFileDialog::Instance()->OpenModal("ChooseModel", "Choose File", "Model files (*.fbx *.obj){.fbx,.obj}", ".\\res\\");
        }

        
        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("ChooseModel")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                modelPath = ImGuiFileDialog::Instance()->GetFilePathName();
                modelName = ImGuiFileDialog::Instance()->GetCurrentFileName();
            }
            objects[nowSelectedDetail]->loadModel(modelPath);
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Change texture")) {
            ImGuiFileDialog::Instance()->OpenModal("Choose new texture", "Choose File", "Texture files (*.png *.jpg){.png,.jpg}", ".\\res\\");
        }
        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGuiFileDialog::Instance()->Display("Choose new texture")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                strncpy_s(texturePath, ImGuiFileDialog::Instance()->GetFilePathName().c_str(), sizeof(texturePath));
            }
            objects[nowSelectedDetail]->loadTexture(texturePath);
            ImGuiFileDialog::Instance()->Close();
        }
        if (ImGui::Button("Delete this object")) {
            sc.deleteEntity(objects[nowSelectedDetail]);
            ImGui::CloseCurrentPopup();
        }


        ImGui::EndPopup();
    }

    bool open1 = true;


    ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
    if (ImGui::BeginPopupModal("World", &openworld)) {

        ImGui::EndPopup();
    }
    static char selectedItem[255];

    bool open = true;
    if (ImGui::BeginPopupModal("Objects", &open))
    {
        ImGui::SameLine(30);
        if (ImGui::Button("Create new object", ImVec2(600, 50))) {
            ImGui::OpenPopup("New object creation");
        }  ImGui::NewLine();

        ImGui::SetNextWindowSize(ImVec2(660.f, 400.f));
        if (ImGui::BeginPopupModal("New object creation", &open1)) {

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
                //model path
                //texture path
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

    

    ImGui::SetNextWindowSize(ImVec2(300.f, 300.f));
    if (ImGui::BeginPopupModal("Settings", &open)) {
        const char* res_listbox_items[] = {"3840x2160","2560x1440", "1920x1200", "1920x1080", 
            "1600x900", "1366x768"};
        std::vector<sf::Vector2u> res_listbox_items_ = {
            {3840, 2160}, {2560, 1440}, {1920, 1200}, {1920, 1080}, {1600, 900}, {1366, 768}
        };

        static int res_listbox_item_current = 3;
        ImGui::Combo("Resolution", &res_listbox_item_current, res_listbox_items, IM_ARRAYSIZE(res_listbox_items));

        static char title[255] = "Type new title here";
        ImGui::InputText("Window title", title, sizeof(title));

        if (ImGui::Button("Change")) {
            window_->setSize(res_listbox_items_[res_listbox_item_current]);
            window_->setTitle(title);
        }
        
        ImGui::EndPopup();
    }
    processSelected();

    ImGui::PopStyleColor();
    ImGui::End(); // end window
    // end of the menu
}
