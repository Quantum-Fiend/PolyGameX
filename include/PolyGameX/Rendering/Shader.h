#pragma once

#include <string>
#include <glm/glm.hpp>

namespace PolyGameX {

class POLYGAMEX_API Shader {
public:
    Shader() = default;
    ~Shader();
    
    bool CompileFromSource(const char* vertexSource, const char* fragmentSource);
    bool CompileFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    
    void Use() const;
    unsigned int GetID() const { return m_ProgramID; }
    
    // Uniform setters
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat3(const std::string& name, const glm::mat3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;
    
private:
    unsigned int CompileShader(unsigned int type, const char* source);
    bool LinkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    int GetUniformLocation(const std::string& name) const;
    
    unsigned int m_ProgramID = 0;
};

} // namespace PolyGameX
