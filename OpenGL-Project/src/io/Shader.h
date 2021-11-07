#ifndef SHADER_H__
#define SHADER_H__

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader();
    ~Shader();

    static constexpr char VIEW_MATRIX[] = "u_viewMatrix";
    static constexpr char PROJECTION_MATRIX[] = "u_projectionMatrix";
    static constexpr char MODEL_MATRIX[] = "u_modelMatrix";

public:
    bool LoadShader(std::string const& vertexSource, std::string const& fragmentSource);
    void Bind() const;

    void SetCameraMatrix(glm::mat4 const& projectionMatrix, glm::mat4 const& viewMatrix) const;
    void SetModelMatrix(glm::mat4 const& modelMatrix) const;

    [[nodiscard]] int32_t GetUniformLocation(std::string const& handle) const;
    void SetUniformLocation(int32_t const& location, glm::mat4 const& matrix) const;
    void SetVector3Location(int32_t const& location, glm::vec3 const& vec3) const;

private:
    static bool Compile(std::string const& source, const int shaderType, uint32_t& shaderId);
    static bool Create(const uint32_t vertexId, const uint32_t fragmentId, uint32_t& programId);

    static std::string LoadFile(std::string const& filePath);

private:
    uint32_t m_shaderProgramId = 0;

    int32_t m_projectionMatrixId = 0;
    int32_t m_viewMatrixId = 0;
    int32_t m_modelMatrixId = 0;
};

#endif // SHADER_H__
