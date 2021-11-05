#ifndef SHADER_H__
#define SHADER_H__

#include <string>

class Shader
{
public:
    Shader();
    ~Shader();

public:
    bool LoadShader(std::string const& vertexSource, std::string const& fragmentSource);
    void Bind() const;

    int32_t GetUniformLocation(std::string const& handle) const;

private:
    bool Compile(std::string const& source, const int shaderType, uint32_t& shaderId);
    bool Create(const uint32_t vertexId, const uint32_t fragmentId, uint32_t& programId);

    std::string LoadFile(std::string const& filePath);

private:
    uint32_t m_shaderProgramId;
};

#endif // SHADER_H__
