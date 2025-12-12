#include "PolyGameX/Rendering/Renderer.h"
#include "PolyGameX/Rendering/Shader.h"
#include "PolyGameX/Rendering/Camera.h"
#include "PolyGameX/Rendering/Mesh.h"
#include "PolyGameX/Core/Logger.h"
#include <glad/glad.h>

namespace PolyGameX {

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize(int width, int height) {
    m_Width = width;
    m_Height = height;
    
    Logger::Info("Initializing Renderer...");
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // Set viewport
    glViewport(0, 0, width, height);
    
    // Create default shaders
    const char* defaultVertexShader = R"(
        #version 430 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoord;
        
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;
        
        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoord;
        
        void main() {
            FragPos = vec3(uModel * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(uModel))) * aNormal;
            TexCoord = aTexCoord;
            gl_Position = uProjection * uView * vec4(FragPos, 1.0);
        }
    )";
    
    const char* defaultFragmentShader = R"(
        #version 430 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;
        
        uniform vec3 uColor;
        uniform vec3 uLightPos;
        uniform vec3 uViewPos;
        
        void main() {
            // Ambient
            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * uColor;
            
            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(uLightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * uColor;
            
            // Specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(uViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * vec3(1.0);
            
            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    m_DefaultShader = std::make_unique<Shader>();
    if (!m_DefaultShader->CompileFromSource(defaultVertexShader, defaultFragmentShader)) {
        Logger::Error("Failed to compile default shader");
        return false;
    }
    
    Logger::Info("Renderer initialized successfully");
    return true;
}

void Renderer::Shutdown() {
    m_DefaultShader.reset();
    m_LineShader.reset();
}

void Renderer::BeginFrame() {
    glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ResetStats();
}

void Renderer::EndFrame() {
    // Nothing to do here for now
}

void Renderer::SetViewport(int x, int y, int width, int height) {
    m_Width = width;
    m_Height = height;
    glViewport(x, y, width, height);
}

void Renderer::SetClearColor(const glm::vec4& color) {
    m_ClearColor = color;
}

void Renderer::DrawMesh(Mesh* mesh, const glm::mat4& transform, Shader* shader) {
    if (!mesh || !m_ActiveCamera) return;
    
    Shader* activeShader = shader ? shader : m_DefaultShader.get();
    if (!activeShader) return;
    
    activeShader->Use();
    
    // Set matrices
    activeShader->SetMat4("uModel", transform);
    activeShader->SetMat4("uView", m_ActiveCamera->GetViewMatrix());
    activeShader->SetMat4("uProjection", m_ActiveCamera->GetProjectionMatrix());
    
    // Set lighting uniforms
    activeShader->SetVec3("uLightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    activeShader->SetVec3("uViewPos", m_ActiveCamera->GetPosition());
    activeShader->SetVec3("uColor", glm::vec3(0.8f, 0.8f, 0.9f));
    
    // Draw mesh
    mesh->Draw();
    
    // Update stats
    m_Stats.drawCalls++;
    m_Stats.triangles += mesh->GetTriangleCount();
    m_Stats.vertices += mesh->GetVertexCount();
}

void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color) {
    // TODO: Implement line rendering
}

void Renderer::ResetStats() {
    m_Stats.drawCalls = 0;
    m_Stats.triangles = 0;
    m_Stats.vertices = 0;
}

} // namespace PolyGameX
