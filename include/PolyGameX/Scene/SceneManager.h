#pragma once

#include <string>
#include <vector>
#include <memory>

namespace PolyGameX {

class Entity;
class Renderer;

class POLYGAMEX_API Scene {
public:
    Scene(const std::string& name);
    ~Scene();
    
    const std::string& GetName() const { return m_Name; }
    
    // Entity management
    Entity* CreateEntity(const std::string& name);
    void DestroyEntity(Entity* entity);
    Entity* FindEntity(const std::string& name);
    
    const std::vector<Entity*>& GetEntities() const { return m_Entities; }
    
    // Lifecycle
    void Update(float deltaTime);
    void Render(Renderer* renderer);
    
    // Serialization
    bool SaveToFile(const std::string& filepath);
    bool LoadFromFile(const std::string& filepath);
    
private:
    std::string m_Name;
    std::vector<Entity*> m_Entities;
};

class POLYGAMEX_API SceneManager {
public:
    SceneManager() = default;
    ~SceneManager();
    
    Scene* CreateScene(const std::string& name);
    void DestroyScene(Scene* scene);
    
    void SetActiveScene(Scene* scene) { m_ActiveScene = scene; }
    Scene* GetActiveScene() const { return m_ActiveScene; }
    
private:
    std::vector<std::unique_ptr<Scene>> m_Scenes;
    Scene* m_ActiveScene = nullptr;
};

} // namespace PolyGameX
