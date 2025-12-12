#pragma once

#include <glm/glm.hpp>

namespace PolyGameX {

enum class ColliderType {
    Sphere,
    Box,
    Capsule
};

class POLYGAMEX_API Collider {
public:
    Collider(ColliderType type) : m_Type(type) {}
    virtual ~Collider() = default;
    
    ColliderType GetType() const { return m_Type; }
    
    // For simple sphere collisions
    void SetRadius(float radius) { m_Radius = radius; }
    float GetRadius() const { return m_Radius; }
    
    // For box collisions
    void SetExtents(const glm::vec3& extents) { m_Extents = extents; }
    glm::vec3 GetExtents() const { return m_Extents; }
    
    // Trigger mode
    void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
    bool IsTrigger() const { return m_IsTrigger; }
    
private:
    ColliderType m_Type;
    float m_Radius = 0.5f;
    glm::vec3 m_Extents = glm::vec3(0.5f);
    bool m_IsTrigger = false;
};

} // namespace PolyGameX
