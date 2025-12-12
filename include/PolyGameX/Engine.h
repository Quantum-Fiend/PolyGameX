#pragma once

#ifdef _WIN32
    #ifdef POLYGAMEX_EXPORTS
        #define POLYGAMEX_API __declspec(dllexport)
    #else
        #define POLYGAMEX_API __declspec(dllimport)
    #endif
#else
    #define POLYGAMEX_API
#endif

#include <memory>
#include <string>

namespace PolyGameX {

class Renderer;
class PhysicsEngine;
class SceneManager;
class InputManager;

class POLYGAMEX_API Engine {
public:
    static Engine& GetInstance();
    
    bool Initialize(int width, int height, const std::string& title);
    void Run();
    void Shutdown();
    
    // Subsystem access
    Renderer* GetRenderer() const { return m_Renderer.get(); }
    PhysicsEngine* GetPhysics() const { return m_Physics.get(); }
    SceneManager* GetSceneManager() const { return m_SceneManager.get(); }
    
    // Engine state
    bool IsRunning() const { return m_Running; }
    void Stop() { m_Running = false; }
    float GetDeltaTime() const { return m_DeltaTime; }
    double GetTime() const;
    
private:
    Engine() = default;
    ~Engine() = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    
    void Update();
    void Render();
    void ProcessInput();
    
    bool m_Running = false;
    float m_DeltaTime = 0.0f;
    double m_LastFrameTime = 0.0;
    
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<PhysicsEngine> m_Physics;
    std::unique_ptr<SceneManager> m_SceneManager;
    std::unique_ptr<InputManager> m_Input;
    
    void* m_Window = nullptr; // GLFW window handle
};

} // namespace PolyGameX
