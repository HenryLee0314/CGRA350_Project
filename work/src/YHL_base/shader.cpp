#include "shader.h"

#include "cgra_log.h"
#include "cgraFile.h"

#include "opengl.hpp"

namespace CGRA350 {

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessellationControlPath, const char* tessellationEvaluationPath)
{

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::string tessellationControlCode;
    std::string tessellationEvaluationCode;

    cgraFile vertexShaderFile(vertexPath);
    cgraFile fragmentShaderFile(fragmentPath);
    cgraFile geometryShaderFile(geometryPath);

    cgraFile tessellationControlShaderFile(tessellationControlPath);
    cgraFile tessellationEvaluationShaderFile(tessellationEvaluationPath);

    if (!vertexShaderFile.read(vertexCode) || !fragmentShaderFile.read(fragmentCode)) {
        CGRA_LOGE("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        return;
    }

    if (geometryPath != nullptr && !geometryShaderFile.read(geometryCode))
    {
        CGRA_LOGE("ERROR::SHADER::GEOMETRY SHADER FILE_NOT_SUCCESFULLY_READ");
        return;
    }

    if (tessellationControlPath != nullptr && !tessellationControlShaderFile.read(tessellationControlCode))
    {
        CGRA_LOGE("ERROR::SHADER::TESSELLATION CONTROL SHADER FILE_NOT_SUCCESFULLY_READ");
        return;
    }

    if (tessellationEvaluationPath != nullptr && !tessellationEvaluationShaderFile.read(tessellationEvaluationCode))
    {
        CGRA_LOGE("ERROR::SHADER::TESSELLATION EVALUATION SHADER FILE_NOT_SUCCESFULLY_READ");
        return;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    const char *tcShaderCode = tessellationControlCode.c_str();
    const char *teShaderCode = tessellationEvaluationCode.c_str();


    unsigned int vertex, fragment, geometry;
    unsigned int tessellationControl, tessellationEvaluation;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, VERTEX);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, FRAGMENT);

    if (geometryPath != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, GEOMETRY);
    }

    if (tessellationControlPath != nullptr) {
        tessellationControl = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessellationControl, 1, &tcShaderCode, NULL);
        glCompileShader(tessellationControl);
        checkCompileErrors(tessellationControl, TESS_CONTROL);
    }
    if (tessellationEvaluationPath != nullptr) {
        tessellationEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessellationEvaluation, 1, &teShaderCode, NULL);
        glCompileShader(tessellationEvaluation);
        checkCompileErrors(tessellationEvaluation, TESS_EVALUATION);
    }

    _id = glCreateProgram();

    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    if (geometryPath != nullptr) {
        glAttachShader(_id, geometry);
    }

    if (tessellationControlPath != nullptr) {
        glAttachShader(_id, tessellationControl);
    }

    if (tessellationEvaluationPath != nullptr) {
        glAttachShader(_id, tessellationEvaluation);
    }

    glLinkProgram(_id);
    checkCompileErrors(_id, PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }
    if (tessellationControlPath != nullptr) {
        glDeleteShader(tessellationControl);
    }
    if (tessellationEvaluationPath != nullptr) {
        glDeleteShader(tessellationEvaluation);
    }

}

Shader::~Shader()
{

}


void Shader::use()
{
    glUseProgram(_id);
}


void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat3(const std::string &name, float valueX, float valueY, float valueZ) const
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), valueX, valueY, valueZ);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(int shader, ShaderType type)
{
    int success;
    char infoLog[1024];
    if (type != PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            CGRA_LOGE("ERROR::SHADER_COMPILATION_ERROR of type(%s): %s ", ShaderTypeName[type].c_str() , infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            CGRA_LOGE("ERROR::PROGRAM_LINKING_ERROR of type(%s): %s ", ShaderTypeName[type].c_str() , infoLog);
        }
    }
}


} // namespace CGRA350