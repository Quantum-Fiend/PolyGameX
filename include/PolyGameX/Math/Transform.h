#pragma once

#include <glm/glm.hpp>

namespace PolyGameX {

/**
 * Transform component for entities.
 * Handles position, rotation, and scale.
 */
class POLYGAMEX_API Transform {
public:
    Transform();
    ~Transform() = default;
    
    // Position
    void SetPosition(const glm::vec3& position) { m_Position = position; m_Dirty = true; }
    const glm::vec3& GetPosition() const { return m_Position; }
    
    // Rotation (Euler angles in degrees)
    void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; m_Dirty = true; }
    const glm::vec3& GetRotation() const { return m_Rotation; }
    
    // Scale
    void SetScale(const glm::vec3& scale) { m_Scale = scale; m_Dirty = true; }
    const glm::vec3& GetScale() const { return m_Scale; }
    
    // Uniform scale
    void SetScale(float scale) { SetScale(glm::vec3(scale)); }
    
    // Transform matrix
    const glm::mat4& GetMatrix();
    glm::mat4 GetInverseMatrix();
    
    // Local space directions
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    
    // Transform operations
    void Translate(const glm::vec3& offset);
    void Rotate(const glm::vec3& eulerAngles);
    void Scale(const glm::vec3& scale);
    
private:
    void UpdateMatrix();
    
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;
    
    glm::mat4 m_Matrix;
    bool m_Dirty;
};

} // namespace PolyGameX
