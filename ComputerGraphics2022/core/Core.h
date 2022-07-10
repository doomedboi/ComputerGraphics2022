#pragma once
#include "../render/common.h"
#include "../render/Render.h"
#include "../render/Core_Camera.h"
#include <memory>
#include "../core/InputManager.h"


class Core {
public:
    static Core& Singleton() { static Core instance; return instance; }
    void SceneUpdate();
    void ScenePostUpdate(); // draw is here
    /* members getters */
    CCamera& GetViewCamera() { return camera_; }
    void SetViewCamera(CCamera& camera) { camera_ = camera; }
    void InitRender(int w, int h, int bitsPerPix, const std::string& title) {
        render_ = std::make_unique<Renderer>(w, h, bitsPerPix, title);
    }
    void InitCamera(const math::vec3& pos, const math::vec3& front = math::vec3(0,0,0),
        const math::vec3& up = math::vec3(0, 1, 0), const float fov = 45.f, 
        const float yaw = default_yaw, const float pitch = default_pitch) {
        camera_ = CCamera(pos, front, up, fov, yaw, pitch);
        camera_.SetRenderProjectMatrix(fov,
            render_->getWnd()->getSize().x / render_->getWnd()->getSize().y, 0.1f, 100.f);
    }
    Core() : render_(nullptr), camera_(math::vec3(0, 0, 3.f)) {}
    Renderer& GetRender() { return *render_; }
    Core(const Core&&) = delete;
    Core(Core&&) = delete;
    void operator=(const Core&) = delete;
private:
    std::unique_ptr<Renderer> render_; // draw stuff getted from scene manager
    CCamera camera_;
    InputManager inputManager_;
};