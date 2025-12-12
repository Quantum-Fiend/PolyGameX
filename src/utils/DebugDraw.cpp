#include "PolyGameX/Utils/DebugDraw.h"
#include "PolyGameX/Rendering/Renderer.h"

namespace PolyGameX {

DebugDraw& DebugDraw::GetInstance() {
    static DebugDraw instance;
    return instance;
}

void DebugDraw::DrawLine(const glm::vec3& start, const glm::vec3& end, 
                         const glm::vec4& color, float duration) {
    Line line;
    line.start = start;
    line.end = end;
    line.color = color;
    line.duration = duration;
    m_Lines.push_back(line);
}

void DebugDraw::DrawSphere(const glm::vec3& center, float radius,
                           const glm::vec4& color, float duration) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    sphere.color = color;
    sphere.duration = duration;
    m_Spheres.push_back(sphere);
}

void DebugDraw::DrawBox(const glm::vec3& center, const glm::vec3& extents,
                        const glm::vec4& color, float duration) {
    Box box;
    box.center = center;
    box.extents = extents;
    box.color = color;
    box.duration = duration;
    m_Boxes.push_back(box);
}

void DebugDraw::DrawPath(const std::vector<glm::vec3>& points,
                         const glm::vec4& color, float duration) {
    for (size_t i = 0; i < points.size() - 1; ++i) {
        DrawLine(points[i], points[i + 1], color, duration);
    }
}

void DebugDraw::Update(float deltaTime) {
    // Remove expired debug draws
    auto removePredicate = [deltaTime](auto& item) {
        if (item.duration > 0.0f) {
            item.duration -= deltaTime;
            return item.duration <= 0.0f;
        }
        return false;
    };
    
    m_Lines.erase(
        std::remove_if(m_Lines.begin(), m_Lines.end(), removePredicate),
        m_Lines.end()
    );
    
    m_Spheres.erase(
        std::remove_if(m_Spheres.begin(), m_Spheres.end(), removePredicate),
        m_Spheres.end()
    );
    
    m_Boxes.erase(
        std::remove_if(m_Boxes.begin(), m_Boxes.end(), removePredicate),
        m_Boxes.end()
    );
}

void DebugDraw::Render(Renderer* renderer) {
    if (!renderer) return;
    
    // Render lines
    for (const auto& line : m_Lines) {
        renderer->DrawLine(line.start, line.end, line.color);
    }
    
    // TODO: Render spheres and boxes using wireframe meshes
}

void DebugDraw::Clear() {
    m_Lines.clear();
    m_Spheres.clear();
    m_Boxes.clear();
}

} // namespace PolyGameX
