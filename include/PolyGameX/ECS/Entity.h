#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PolyGameX {

class Mesh;
class RigidBody;
class Renderer;

class POLYGAMEX_API Entity {
public:
    Entity(const std::string& name);
    ~Entity();
    
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    
    // Transform
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3 GetPosition() const { return m_Position; }
    
    void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
    glm::vec3 GetRotation() const { return m_Rotation; }
    
    void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    glm::vec3 GetScale() const { return m_Scale; }
    
    glm::mat4 GetTransformMatrix() const;
    
    // Components
    void SetMesh(Mesh* mesh) { m_Mesh = mesh; }
    Mesh* GetMesh() const { return m_Mesh; }
    
    void SetRigidBody(RigidBody* rigidBody) { m_RigidBody = rigidBody; }
    RigidBody* GetRigidBody() const { return m_RigidBody; }
    
    // Lifecycle
    void Update(float deltaTime);
    void Render(Renderer* renderer);
    
    // State
    void SetActive(bool active) { m_Active = active; }
    bool IsActive() const { return m_Active; }
    
private:
    std::string m_Name;
    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_Rotation = glm::vec3(0.0f);
    glm::vec3 m_Scale = glm::vec3(1.0f);
    
    Mesh* m_Mesh = nullptr;
    RigidBody* m_RigidBody = nullptr;
    
    bool m_Active = true;
};

} // namespace PolyGameX
