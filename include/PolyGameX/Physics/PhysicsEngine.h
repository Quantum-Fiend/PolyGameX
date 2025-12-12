#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace PolyGameX {

class RigidBody;
class Collider;

class POLYGAMEX_API PhysicsEngine {
public:
    PhysicsEngine() = default;
    ~PhysicsEngine() = default;
    
    void Initialize();
    void Update(float deltaTime);
    void Shutdown();
    
    // Rigid body management
    void AddRigidBody(RigidBody* body);
    void RemoveRigidBody(RigidBody* body);
    
    // Physics settings
    void SetGravity(const glm::vec3& gravity) { m_Gravity = gravity; }
    glm::vec3 GetGravity() const { return m_Gravity; }
    
    // Raycasting
    struct RaycastHit {
        RigidBody* body = nullptr;
        glm::vec3 point;
        glm::vec3 normal;
        float distance = 0.0f;
    };
    
    bool Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit& hit);
    
private:
    void IntegrateVelocities(float deltaTime);
    void DetectCollisions();
    void ResolveCollisions();
    
    std::vector<RigidBody*> m_RigidBodies;
    glm::vec3 m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    
    struct CollisionPair {
        RigidBody* bodyA;
        RigidBody* bodyB;
        glm::vec3 normal;
        float penetration;
    };
    
    std::vector<CollisionPair> m_Collisions;
};

} // namespace PolyGameX
