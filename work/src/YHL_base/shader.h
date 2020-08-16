#ifndef CGRA350_SHADER_H
#define CGRA350_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CGRA350 {

class Shader
{
public:
    enum ShaderType {
        INVALID = 0,
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION,
        PROGRAM,
        ShaderTypeMax
    };

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessellationControlPath = nullptr, const char* tessellationEvaluationPath = nullptr);

    virtual ~Shader();

    void use();

    inline unsigned int getId() {
        return _id;
    }

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setFloat3(const std::string &name, float valueX, float valueY, float valueZ) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;

    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;

    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;

    void setMat3(const std::string &name, const glm::mat3 &mat) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void checkCompileErrors(int shader, ShaderType type);

private:
    unsigned int _id;
};

static const std::string ShaderTypeName[Shader::ShaderType::ShaderTypeMax] = {
    "INVALID",
    "VERTEX",
    "FRAGMENT",
    "GEOMETRY",
    "TESS_CONTROL",
    "TESS_EVALUATION",
    "PROGRAM"
};

} // namespace CGRA350

#endif // CGRA350_SHADER_H