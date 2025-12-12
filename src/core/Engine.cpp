#include "PolyGameX/Engine.h"
#include "PolyGameX/Rendering/Renderer.h"
#include "PolyGameX/Physics/PhysicsEngine.h"
#include "PolyGameX/Scene/SceneManager.h"
#include "PolyGameX/Core/Logger.h"
#include <GLFW/glfw3.h>
#include <chrono>

namespace PolyGameX {

Engine& Engine::GetInstance() {
    static Engine instance;
    return instance;
}

bool Engine::Initialize(int width, int height, const std::string& title) {
    Logger::Info("Initializing PolyGameX Engine...");
    
    // Initialize GLFW
    if (!glfwInit()) {
        Logger::Error("Failed to initialize GLFW");
        return false;
    }
    
    // Set OpenGL version (4.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        Logger::Error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window));
    
    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Error("Failed to initialize GLAD");
        return false;
    }
    
    Logger::Info("OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));
    
    // Initialize subsystems
    m_Renderer = std::make_unique<Renderer>();
    if (!m_Renderer->Initialize(width, height)) {
        Logger::Error("Failed to initialize Renderer");
        return false;
    }
    
    m_Physics = std::make_unique<PhysicsEngine>();
    m_Physics->Initialize();
    
    m_SceneManager = std::make_unique<SceneManager>();
    
    m_Running = true;
    m_LastFrameTime = glfwGetTime();
    
    Logger::Info("Engine initialized successfully");
    return true;
}

void Engine::Run() {
    Logger::Info("Starting main loop...");
    
    while (m_Running && !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window))) {
        double currentTime = glfwGetTime();
        m_DeltaTime = static_cast<float>(currentTime - m_LastFrameTime);
        m_LastFrameTime = currentTime;
        
        ProcessInput();
        Update();
        Render();
        
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
        glfwPollEvents();
    }
}

void Engine::Update() {
    // Update physics
    m_Physics->Update(m_DeltaTime);
    
    // Update active scene
    if (m_SceneManager->GetActiveScene()) {
        m_SceneManager->GetActiveScene()->Update(m_DeltaTime);
    }
}

void Engine::Render() {
    m_Renderer->BeginFrame();
    
    // Render active scene
    if (m_SceneManager->GetActiveScene()) {
        m_SceneManager->GetActiveScene()->Render(m_Renderer.get());
    }
    
    m_Renderer->EndFrame();
}

void Engine::ProcessInput() {
    // Handle ESC key to close
    if (glfwGetKey(static_cast<GLFWwindow*>(m_Window), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        m_Running = false;
    }
}

void Engine::Shutdown() {
    Logger::Info("Shutting down engine...");
    
    m_SceneManager.reset();
    m_Physics.reset();
    m_Renderer.reset();
    
    if (m_Window) {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
        m_Window = nullptr;
    }
    
    glfwTerminate();
    Logger::Info("Engine shutdown complete");
}

double Engine::GetTime() const {
    return glfwGetTime();
}

} // namespace PolyGameX
