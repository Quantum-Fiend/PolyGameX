#pragma once

#include <glm/glm.hpp>

namespace PolyGameX {

class Collider;

class POLYGAMEX_API RigidBody {
public:
    RigidBody();
    ~RigidBody() = default;
    
    // Transform
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3 GetPosition() const { return m_Position; }
    
    void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
    glm::vec3 GetRotation() const { return m_Rotation; }
    
    // Physics properties
    void SetMass(float mass) { m_Mass = mass; m_InvMass = (mass > 0.0f) ? 1.0f / mass : 0.0f; }
    float GetMass() const { return m_Mass; }
    float GetInvMass() const { return m_InvMass; }
    
    void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }
    glm::vec3 GetVelocity() const { return m_Velocity; }
    
    void SetAngularVelocity(const glm::vec3& angularVelocity) { m_AngularVelocity = angularVelocity; }
    glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
    
    // Forces
    void AddForce(const glm::vec3& force) { m_Force += force; }
    void AddTorque(const glm::vec3& torque) { m_Torque += torque; }
    void ClearForces() { m_Force = glm::vec3(0.0f); m_Torque = glm::vec3(0.0f); }
    
    glm::vec3 GetForce() const { return m_Force; }
    glm::vec3 GetTorque() const { return m_Torque; }
    
    // State
    void SetStatic(bool isStatic) { m_IsStatic = isStatic; }
    bool IsStatic() const { return m_IsStatic; }
    
    void SetKinematic(bool isKinematic) { m_IsKinematic = isKinematic; }
    bool IsKinematic() const { return m_IsKinematic; }
    
    // Collider
    void SetCollider(Collider* collider) { m_Collider = collider; }
    Collider* GetCollider() const { return m_Collider; }
    
private:
    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_Rotation = glm::vec3(0.0f);
    
    glm::vec3 m_Velocity = glm::vec3(0.0f);
    glm::vec3 m_AngularVelocity = glm::vec3(0.0f);
    
    glm::vec3 m_Force = glm::vec3(0.0f);
    glm::vec3 m_Torque = glm::vec3(0.0f);
    
    float m_Mass = 1.0f;
    float m_InvMass = 1.0f;
    
    bool m_IsStatic = false;
    bool m_IsKinematic = false;
    
    Collider* m_Collider = nullptr;
};

} // namespace PolyGameX
