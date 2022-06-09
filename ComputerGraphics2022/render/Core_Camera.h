#pragma once
#include "../render/common.h"
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"

constexpr float default_yaw = -90.f;
constexpr float default_pitch = 0.f;

class CCamera final {
public:
    CCamera(const math::vec3& pos, const math::vec3& front = math::vec3(0, 0, 0),
            const math::vec3& up = math::vec3(0, 1, 0), const float fov = 45.f,
            const float yaw = default_yaw, const float pitch = default_pitch);
    void SetMatrix(const math::mat4&);
    void SetAngles(float xOffset, float yOffset, bool debugMode);
    math::mat4 GetViewMatrix() const;
    math::vec3 GetPosition() const;
    math::vec3& GetPosition();
    void SetPosition(const math::vec3&);
    math::vec3 GetUp() const;
    math::vec3 GetFront() const;
    // render or core should do that
    math::mat4 GetRenderProjectionMatrix() const;
    float GetFov() const;
    void SetRenderProjectMatrix(math::mat4& mat);
    void SetRenderProjectMatrix(const float fov, const float ratio,
        const float zNear, const float zFar);
private:
    math::vec3 position_;
    math::vec3 up_;
    math::vec3 front_;
    math::vec3 right_;
    math::vec3 worldUp_;
    float yaw_;
    float pitch_;
    float fov_ = 45;
    math::mat4 projection_;

    void recalcCameraVectors();
};