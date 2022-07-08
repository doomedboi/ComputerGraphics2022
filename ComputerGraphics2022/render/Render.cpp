#include "Render.h"


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

Renderer::Renderer(int w, int h, int bitsPerPix, const std::string& title) :
    window_( std::make_shared<sf::RenderWindow>(sf::VideoMode(w, h, bitsPerPix), title.c_str(),
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

void Renderer::DrawMenu() {
    sf::Clock deltaClock;
    float color[3] = { 0.f, 0.f, 0.f };
    char windowTitle[255] = "ImGui + SFML = <3";
    window_->pushGLStates();
    // beginning of the menu    

    ImGui::SFML::Update(*window_, deltaClock.restart());
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(6, 6);
    ImGui::SetNextWindowSize(ImVec2(660.f, 560.f));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.6f, 0.6f, 0.6f, 0.2));
    ImGui::Begin("Sample window"); // создаём окно

    ImGui::BeginChild("Complete Border", ImVec2(648.f, 548.f), false); {

    } ImGui::EndChild();

    ImGui::InputText("Window title", windowTitle, 255);

    
    ImGui::PopStyleColor();
    ImGui::End(); // end window
    // end of the menu
    ImGui::SFML::Render(*window_);
    window_->popGLStates();

    window_->display();
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
