#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PolyGameX {

enum class CameraType {
    Perspective,
    Orthographic
};

class POLYGAMEX_API Camera {
public:
    Camera(CameraType type = CameraType::Perspective);
    
    // Transform
    void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }
    void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; UpdateViewMatrix(); }
    
    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetRotation() const { return m_Rotation; }
    
    // Projection
    void SetPerspective(float fov, float aspect, float nearPlane, float farPlane);
    void SetOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    // Matrices
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
    
    // Movement
    void Move(const glm::vec3& offset);
    void Rotate(const glm::vec3& offset);
    void LookAt(const glm::vec3& target);
    
    // Vectors
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    
private:
    void UpdateViewMatrix();
    
    CameraType m_Type;
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_Rotation = glm::vec3(0.0f);
    
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    
    // Perspective params
    float m_FOV = 45.0f;
    float m_AspectRatio = 16.0f / 9.0f;
    float m_NearPlane = 0.1f;
    float m_FarPlane = 100.0f;
};

} // namespace PolyGameX
