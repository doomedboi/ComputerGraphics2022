#include "Core_Camera.h"

CCamera::CCamera(const math::vec3& pos, const math::vec3& front, const math::vec3& up,
    const float fov, const float yaw, const float pitch) : position_(pos), front_(front),
    up_(up), fov_(fov), yaw_(yaw), pitch_(pitch) {
    recalcCameraVectors();
}

void CCamera::SetAngles(float xOffset, float yOffset, bool debugMode) {
    // TODO: if no debug mode we have to stand on the XZ 
    // normalize for sensetive
    xOffset *= 0.05f, yOffset *= 0.05f;
    yaw_ += xOffset;
    pitch_ += yOffset;

    if (pitch_ > 89)
        pitch_ = 89;
    if (pitch_ < -89)
        pitch_ = -89;

    recalcCameraVectors();
}

math::mat4 CCamera::GetViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);    
}

math::vec3 CCamera::GetPosition() const {
    return position_;
}

math::vec3& CCamera::GetPosition() {
    return position_;
}

void CCamera::SetPosition(const math::vec3& pos) {
    position_ = pos;
    recalcCameraVectors();
}

math::vec3 CCamera::GetUp() const
{
    return up_;
}

math::vec3 CCamera::GetFront() const
{
    return front_;
}

GLfloat CCamera::getCameraSpeed() const
{
    return cameraSpeed;
}

void CCamera::setCameraSpeed(GLfloat speed) {
    cameraSpeed = speed;
}

math::mat4 CCamera::GetRenderProjectionMatrix() const {
    return projection_;
}

float CCamera::GetFov() const
{
    return fov_;
}

void CCamera::SetRenderProjectMatrix(math::mat4& mat) {
    projection_ = mat;  // fov???
}

void CCamera::SetRenderProjectMatrix(const float fov, const float ratio,
    const float zNear, const float zFar) {
    projection_ = glm::perspective(fov, ratio, zNear, zFar);
    fov_ = fov;
}



void CCamera::recalcCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(this->pitch_));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(front_, up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up_ = glm::normalize(glm::cross(right_, front_));
}
