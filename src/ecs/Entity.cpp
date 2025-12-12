#include "PolyGameX/ECS/Entity.h"
#include "PolyGameX/Rendering/Mesh.h"
#include "PolyGameX/Rendering/Renderer.h"
#include "PolyGameX/Physics/RigidBody.h"

namespace PolyGameX {

Entity::Entity(const std::string& name) : m_Name(name) {
}

Entity::~Entity() {
    // Note: Mesh and RigidBody are not owned by Entity, so we don't delete them
}

glm::mat4 Entity::GetTransformMatrix() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_Position);
    transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, m_Scale);
    return transform;
}

void Entity::Update(float deltaTime) {
    if (!m_Active) return;
    
    // Sync transform with rigid body if present
    if (m_RigidBody) {
        m_Position = m_RigidBody->GetPosition();
        m_Rotation = m_RigidBody->GetRotation();
    }
}

void Entity::Render(Renderer* renderer) {
    if (!m_Active || !m_Mesh) return;
    
    renderer->DrawMesh(m_Mesh, GetTransformMatrix(), nullptr);
}

} // namespace PolyGameX
