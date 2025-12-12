#pragma once

#include <string>
#include <vector>
#include <functional>

namespace PolyGameX {

/**
 * Debug drawing utilities for visualizing physics and AI.
 */
class POLYGAMEX_API DebugDraw {
public:
    struct Line {
        glm::vec3 start;
        glm::vec3 end;
        glm::vec4 color;
        float duration;
    };
    
    struct Sphere {
        glm::vec3 center;
        float radius;
        glm::vec4 color;
        float duration;
    };
    
    struct Box {
        glm::vec3 center;
        glm::vec3 extents;
        glm::vec4 color;
        float duration;
    };
    
    static DebugDraw& GetInstance();
    
    // Draw primitives
    void DrawLine(const glm::vec3& start, const glm::vec3& end, 
                  const glm::vec4& color = glm::vec4(1.0f), float duration = 0.0f);
    
    void DrawSphere(const glm::vec3& center, float radius,
                    const glm::vec4& color = glm::vec4(1.0f), float duration = 0.0f);
    
    void DrawBox(const glm::vec3& center, const glm::vec3& extents,
                 const glm::vec4& color = glm::vec4(1.0f), float duration = 0.0f);
    
    void DrawPath(const std::vector<glm::vec3>& points,
                  const glm::vec4& color = glm::vec4(1.0f), float duration = 0.0f);
    
    // Update and render
    void Update(float deltaTime);
    void Render(class Renderer* renderer);
    void Clear();
    
private:
    DebugDraw() = default;
    
    std::vector<Line> m_Lines;
    std::vector<Sphere> m_Spheres;
    std::vector<Box> m_Boxes;
};

} // namespace PolyGameX
