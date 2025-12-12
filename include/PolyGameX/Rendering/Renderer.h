#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

namespace PolyGameX {

class Shader;
class Camera;
class Mesh;

class POLYGAMEX_API Renderer {
public:
    Renderer() = default;
    ~Renderer();
    
    bool Initialize(int width, int height);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void SetViewport(int x, int y, int width, int height);
    void SetClearColor(const glm::vec4& color);
    
    // Rendering
    void DrawMesh(Mesh* mesh, const glm::mat4& transform, Shader* shader);
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
    
    // Camera
    void SetActiveCamera(Camera* camera) { m_ActiveCamera = camera; }
    Camera* GetActiveCamera() const { return m_ActiveCamera; }
    
    // Stats
    struct RenderStats {
        uint32_t drawCalls = 0;
        uint32_t triangles = 0;
        uint32_t vertices = 0;
    };
    
    const RenderStats& GetStats() const { return m_Stats; }
    void ResetStats();
    
private:
    int m_Width = 0;
    int m_Height = 0;
    glm::vec4 m_ClearColor = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
    
    Camera* m_ActiveCamera = nullptr;
    RenderStats m_Stats;
    
    std::unique_ptr<Shader> m_DefaultShader;
    std::unique_ptr<Shader> m_LineShader;
};

} // namespace PolyGameX
