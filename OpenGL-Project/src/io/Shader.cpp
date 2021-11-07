#include "Shader.h"
#include "common/Define.h"

#include <sstream>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader() = default;
Shader::~Shader()
{
    if (m_shaderProgramId != GL_INVALID_VALUE)
    {
        glDeleteProgram(m_shaderProgramId);
        m_shaderProgramId = GL_INVALID_VALUE;
    }
}

bool Shader::LoadShader(std::string const& vertexSource, std::string const& fragmentSource)
{
    const std::string vertexCode = LoadFile(vertexSource);
    if (vertexCode.empty())
        return false;

    const std::string fragmentCode = LoadFile(fragmentSource);
    if (fragmentCode.empty())
        return false;

    uint32_t vertexId;
    if (!Compile(vertexCode, GL_VERTEX_SHADER, vertexId))
        return false;

    uint32_t fragmentId;
    if (!Compile(fragmentCode, GL_FRAGMENT_SHADER, fragmentId))
        return false;

    uint32_t programId;
    const bool success = Create(vertexId, fragmentId, programId);

    // When program is ready, we can dump shaders freely, even if we succeed
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    m_shaderProgramId = success ? programId : GL_INVALID_VALUE;

    m_viewMatrixId = GetUniformLocation(VIEW_MATRIX);
    m_projectionMatrixId = GetUniformLocation(PROJECTION_MATRIX);
    m_modelMatrixId = GetUniformLocation(MODEL_MATRIX);

    return success;
}

void Shader::Bind() const
{
    glUseProgram(m_shaderProgramId);
}

void Shader::SetCameraMatrix(glm::mat4 const& projectionMatrix, glm::mat4 const& viewMatrix) const
{
    SetUniformLocation(m_projectionMatrixId, projectionMatrix);
    SetUniformLocation(m_viewMatrixId, viewMatrix);
}

void Shader::SetModelMatrix(glm::mat4 const& modelMatrix) const
{
    SetUniformLocation(m_modelMatrixId, modelMatrix);
}

int32_t Shader::GetUniformLocation(std::string const& handle) const
{
    return glGetUniformLocation(m_shaderProgramId, handle.c_str());
}

void Shader::SetUniformLocation(int32_t const& location, glm::mat4 const& matrix) const
{
    glProgramUniformMatrix4fv(m_shaderProgramId, location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVector3Location(int32_t const& location, glm::vec3 const& vec3) const
{
    glUniform3f(location, vec3.x, vec3.y, vec3.z);
}

bool Shader::Compile(std::string const& source, const int shaderType, uint32_t& shaderId)
{
    const uint32_t shader = glCreateShader(shaderType);

    char const* sourcePointer = source.c_str();
    glShaderSource(shader, 1, &sourcePointer, nullptr);
    glCompileShader(shader);

    int32_t compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (!compileResult)
    {
        char errorLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, errorLog);
        Log::Print("Failed to compile shader: %s", errorLog);
        glDeleteShader(shader);
        return false;
    }

    shaderId = shader;
    return true;
}

bool Shader::Create(const uint32_t vertexId, const uint32_t fragmentId, uint32_t& programId)
{
    programId = glCreateProgram();
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    glLinkProgram(programId);

    int32_t linkResult = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);

    if (!linkResult)
    {
        char errorLog[512];
        glGetProgramInfoLog(programId, 512, nullptr, errorLog);
        Log::Print("Failed to create shader: %s", errorLog);
        glDeleteProgram(programId);
        return false;
    }

    return true;
}

std::string Shader::LoadFile(std::string const& filePath)
{
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        Log::Print("Failed to read file: %s", filePath.c_str());
        return "";
    }

    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    std::string content = stringStream.str();
    fileStream.close();

    return content;
}
