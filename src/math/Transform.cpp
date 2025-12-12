#include "PolyGameX/Math/Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace PolyGameX {

Transform::Transform()
    : m_Position(0.0f)
    , m_Rotation(0.0f)
    , m_Scale(1.0f)
    , m_Matrix(1.0f)
    , m_Dirty(true)
{
}

const glm::mat4& Transform::GetMatrix() {
    if (m_Dirty) {
        UpdateMatrix();
        m_Dirty = false;
    }
    return m_Matrix;
}

glm::mat4 Transform::GetInverseMatrix() {
    return glm::inverse(GetMatrix());
}

void Transform::UpdateMatrix() {
    m_Matrix = glm::mat4(1.0f);
    
    // Apply transformations in TRS order
    m_Matrix = glm::translate(m_Matrix, m_Position);
    m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    m_Matrix = glm::scale(m_Matrix, m_Scale);
}

glm::vec3 Transform::GetForward() const {
    float yaw = glm::radians(m_Rotation.y);
    float pitch = glm::radians(m_Rotation.x);
    
    return glm::normalize(glm::vec3(
        cos(yaw) * cos(pitch),
        sin(pitch),
        sin(yaw) * cos(pitch)
    ));
}

glm::vec3 Transform::GetRight() const {
    return glm::normalize(glm::cross(GetForward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Transform::GetUp() const {
    return glm::normalize(glm::cross(GetRight(), GetForward()));
}

void Transform::Translate(const glm::vec3& offset) {
    m_Position += offset;
    m_Dirty = true;
}

void Transform::Rotate(const glm::vec3& eulerAngles) {
    m_Rotation += eulerAngles;
    m_Dirty = true;
}

void Transform::Scale(const glm::vec3& scale) {
    m_Scale *= scale;
    m_Dirty = true;
}

} // namespace PolyGameX
