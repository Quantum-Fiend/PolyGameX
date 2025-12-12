#include "PolyGameX/Scene/SceneManager.h"
#include "PolyGameX/ECS/Entity.h"
#include "PolyGameX/Core/Logger.h"

namespace PolyGameX {

// Scene Implementation
Scene::Scene(const std::string& name) : m_Name(name) {
    Logger::Info("Created scene: " + name);
}

Scene::~Scene() {
    for (auto* entity : m_Entities) {
        delete entity;
    }
    m_Entities.clear();
}

Entity* Scene::CreateEntity(const std::string& name) {
    Entity* entity = new Entity(name);
    m_Entities.push_back(entity);
    return entity;
}

void Scene::DestroyEntity(Entity* entity) {
    auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
    if (it != m_Entities.end()) {
        delete *it;
        m_Entities.erase(it);
    }
}

Entity* Scene::FindEntity(const std::string& name) {
    for (auto* entity : m_Entities) {
        if (entity->GetName() == name) {
            return entity;
        }
    }
    return nullptr;
}

void Scene::Update(float deltaTime) {
    for (auto* entity : m_Entities) {
        entity->Update(deltaTime);
    }
}

void Scene::Render(Renderer* renderer) {
    for (auto* entity : m_Entities) {
        entity->Render(renderer);
    }
}

bool Scene::SaveToFile(const std::string& filepath) {
    // TODO: Implement scene serialization
    Logger::Warning("Scene serialization not yet implemented");
    return false;
}

bool Scene::LoadFromFile(const std::string& filepath) {
    // TODO: Implement scene deserialization
    Logger::Warning("Scene deserialization not yet implemented");
    return false;
}

// SceneManager Implementation
SceneManager::~SceneManager() {
    m_Scenes.clear();
}

Scene* SceneManager::CreateScene(const std::string& name) {
    auto scene = std::make_unique<Scene>(name);
    Scene* scenePtr = scene.get();
    m_Scenes.push_back(std::move(scene));
    return scenePtr;
}

void SceneManager::DestroyScene(Scene* scene) {
    auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(),
        [scene](const std::unique_ptr<Scene>& s) { return s.get() == scene; });
    
    if (it != m_Scenes.end()) {
        if (m_ActiveScene == scene) {
            m_ActiveScene = nullptr;
        }
        m_Scenes.erase(it);
    }
}

} // namespace PolyGameX
