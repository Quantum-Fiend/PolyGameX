#include "PolyGameX/Physics/PhysicsEngine.h"
#include "PolyGameX/Physics/RigidBody.h"
#include "PolyGameX/Physics/Collider.h"
#include "PolyGameX/Core/Logger.h"

namespace PolyGameX {

void PhysicsEngine::Initialize() {
    Logger::Info("Initializing Physics Engine...");
    m_RigidBodies.clear();
    m_Collisions.clear();
}

void PhysicsEngine::Update(float deltaTime) {
    // Integrate velocities
    IntegrateVelocities(deltaTime);
    
    // Detect collisions
    DetectCollisions();
    
    // Resolve collisions
    ResolveCollisions();
}

void PhysicsEngine::Shutdown() {
    m_RigidBodies.clear();
    m_Collisions.clear();
}

void PhysicsEngine::AddRigidBody(RigidBody* body) {
    if (body) {
        m_RigidBodies.push_back(body);
    }
}

void PhysicsEngine::RemoveRigidBody(RigidBody* body) {
    auto it = std::find(m_RigidBodies.begin(), m_RigidBodies.end(), body);
    if (it != m_RigidBodies.end()) {
        m_RigidBodies.erase(it);
    }
}

void PhysicsEngine::IntegrateVelocities(float deltaTime) {
    for (auto* body : m_RigidBodies) {
        if (!body->IsStatic()) {
            // Apply gravity
            body->AddForce(m_Gravity * body->GetMass());
            
            // Update velocity
            glm::vec3 acceleration = body->GetForce() / body->GetMass();
            body->SetVelocity(body->GetVelocity() + acceleration * deltaTime);
            
            // Update position
            body->SetPosition(body->GetPosition() + body->GetVelocity() * deltaTime);
            
            // Apply damping
            body->SetVelocity(body->GetVelocity() * 0.99f);
            
            // Clear forces
            body->ClearForces();
        }
    }
}

void PhysicsEngine::DetectCollisions() {
    m_Collisions.clear();
    
    // Broad phase: check all pairs (naive O(n²) - could be optimized with spatial partitioning)
    for (size_t i = 0; i < m_RigidBodies.size(); ++i) {
        for (size_t j = i + 1; j < m_RigidBodies.size(); ++j) {
            RigidBody* bodyA = m_RigidBodies[i];
            RigidBody* bodyB = m_RigidBodies[j];
            
            if (bodyA->IsStatic() && bodyB->IsStatic()) continue;
            
            Collider* colliderA = bodyA->GetCollider();
            Collider* colliderB = bodyB->GetCollider();
            
            if (!colliderA || !colliderB) continue;
            
            // Simple sphere-sphere collision for now
            glm::vec3 delta = bodyB->GetPosition() - bodyA->GetPosition();
            float distance = glm::length(delta);
            float radiusSum = colliderA->GetRadius() + colliderB->GetRadius();
            
            if (distance < radiusSum) {
                CollisionPair collision;
                collision.bodyA = bodyA;
                collision.bodyB = bodyB;
                collision.normal = glm::normalize(delta);
                collision.penetration = radiusSum - distance;
                m_Collisions.push_back(collision);
            }
        }
    }
}

void PhysicsEngine::ResolveCollisions() {
    for (const auto& collision : m_Collisions) {
        RigidBody* bodyA = collision.bodyA;
        RigidBody* bodyB = collision.bodyB;
        
        // Separate bodies
        float totalMass = bodyA->GetMass() + bodyB->GetMass();
        float ratioA = bodyB->GetMass() / totalMass;
        float ratioB = bodyA->GetMass() / totalMass;
        
        if (!bodyA->IsStatic()) {
            bodyA->SetPosition(bodyA->GetPosition() - collision.normal * collision.penetration * ratioA);
        }
        if (!bodyB->IsStatic()) {
            bodyB->SetPosition(bodyB->GetPosition() + collision.normal * collision.penetration * ratioB);
        }
        
        // Apply impulse
        glm::vec3 relativeVelocity = bodyB->GetVelocity() - bodyA->GetVelocity();
        float velocityAlongNormal = glm::dot(relativeVelocity, collision.normal);
        
        if (velocityAlongNormal < 0) {
            float restitution = 0.5f; // Bounciness
            float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
            impulseScalar /= (1.0f / bodyA->GetMass() + 1.0f / bodyB->GetMass());
            
            glm::vec3 impulse = impulseScalar * collision.normal;
            
            if (!bodyA->IsStatic()) {
                bodyA->SetVelocity(bodyA->GetVelocity() - impulse / bodyA->GetMass());
            }
            if (!bodyB->IsStatic()) {
                bodyB->SetVelocity(bodyB->GetVelocity() + impulse / bodyB->GetMass());
            }
        }
    }
}

bool PhysicsEngine::Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit& hit) {
    // Simple raycast implementation
    float closestDistance = maxDistance;
    bool hitFound = false;
    
    for (auto* body : m_RigidBodies) {
        Collider* collider = body->GetCollider();
        if (!collider) continue;
        
        // Ray-sphere intersection
        glm::vec3 oc = origin - body->GetPosition();
        float a = glm::dot(direction, direction);
        float b = 2.0f * glm::dot(oc, direction);
        float c = glm::dot(oc, oc) - collider->GetRadius() * collider->GetRadius();
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant >= 0) {
            float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
            if (t > 0 && t < closestDistance) {
                closestDistance = t;
                hit.body = body;
                hit.point = origin + direction * t;
                hit.normal = glm::normalize(hit.point - body->GetPosition());
                hit.distance = t;
                hitFound = true;
            }
        }
    }
    
    return hitFound;
}

} // namespace PolyGameX
