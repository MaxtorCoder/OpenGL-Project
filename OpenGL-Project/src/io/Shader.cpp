#include "Shader.h"
#include "common/Define.h"

#include <sstream>
#include <fstream>

Shader::Shader() : m_shaderProgramId(0) {}
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

    return success;
}

void Shader::Bind() const
{
    glUseProgram(m_shaderProgramId);
}

int32_t Shader::GetUniformLocation(std::string const& handle) const
{
    return glGetUniformLocation(m_shaderProgramId, handle.c_str());
}

bool Shader::Compile(std::string const& source, const int shaderType, uint32_t& shaderId)
{
    uint32_t shader = glCreateShader(shaderType);

    char const* sourcePointer = source.c_str();
    glShaderSource(shader, 1, &sourcePointer, nullptr);
    glCompileShader(shader);

    int32_t compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (!compileResult)
    {
        char errorLog[512];
        glGetShaderInfoLog(shader, 512, NULL, errorLog);
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
        glGetProgramInfoLog(programId, 512, NULL, errorLog);
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
