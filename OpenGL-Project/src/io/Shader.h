#ifndef SHADER_H__
#define SHADER_H__

#include <string>

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

    void SetCameraMatrix(const float* projectionMatrix, const float* viewMatrix);
    void SetModelMatrix(const float* modelMatrix);

    int32_t GetUniformLocation(std::string const& handle) const;

private:
    bool Compile(std::string const& source, const int shaderType, uint32_t& shaderId);
    bool Create(const uint32_t vertexId, const uint32_t fragmentId, uint32_t& programId);

    std::string LoadFile(std::string const& filePath);

private:
    uint32_t m_shaderProgramId = 0;

    int32_t m_projectionMatrixId = 0;
    int32_t m_viewMatrixId = 0;
    int32_t m_modelMatrixId = 0;
};

#endif // SHADER_H__
