#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace PolyGameX {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class POLYGAMEX_API Mesh {
public:
    Mesh() = default;
    ~Mesh();
    
    void SetVertices(const std::vector<Vertex>& vertices);
    void SetIndices(const std::vector<unsigned int>& indices);
    void Build();
    
    void Draw() const;
    
    uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_Vertices.size()); }
    uint32_t GetTriangleCount() const { return static_cast<uint32_t>(m_Indices.size()) / 3; }
    
    // Primitive generators
    static Mesh* CreateCube();
    static Mesh* CreateSphere(int segments = 32);
    static Mesh* CreatePlane(float width = 1.0f, float height = 1.0f);
    
private:
    void Cleanup();
    
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;
    bool m_Built = false;
};

} // namespace PolyGameX
