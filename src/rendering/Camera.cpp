#include "PolyGameX/Rendering/Camera.h"

namespace PolyGameX {

Camera::Camera(CameraType type) : m_Type(type) {
    if (m_Type == CameraType::Perspective) {
        SetPerspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
    }
    UpdateViewMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float nearPlane, float farPlane) {
    m_FOV = fov;
    m_AspectRatio = aspect;
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::SetOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}

void Camera::Move(const glm::vec3& offset) {
    m_Position += offset;
    UpdateViewMatrix();
}

void Camera::Rotate(const glm::vec3& offset) {
    m_Rotation += offset;
    UpdateViewMatrix();
}

void Camera::LookAt(const glm::vec3& target) {
    m_ViewMatrix = glm::lookAt(m_Position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateViewMatrix() {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, -m_Position);
    m_ViewMatrix = transform;
}

glm::vec3 Camera::GetForward() const {
    return glm::normalize(glm::vec3(
        glm::cos(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x)),
        glm::sin(glm::radians(m_Rotation.x)),
        glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x))
    ));
}

glm::vec3 Camera::GetRight() const {
    return glm::normalize(glm::cross(GetForward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Camera::GetUp() const {
    return glm::normalize(glm::cross(GetRight(), GetForward()));
}

} // namespace PolyGameX
