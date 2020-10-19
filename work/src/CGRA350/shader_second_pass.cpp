#include "shader_second_pass.h"
#include "light.h"

namespace CGRA350 {

SecondPassShader* SecondPassShader::_instance = nullptr;

SecondPassShader* SecondPassShader::getInstance()
{
	if (_instance == nullptr) {
		_instance = new SecondPassShader();
	}

	return _instance;
}

SecondPassShader::SecondPassShader()
	: _floorShader(CGRA_SRCDIR "/res/shaders/vertexShader/second_pass.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/second_pass.fs")
{

}

SecondPassShader::~SecondPassShader()
{

}

void SecondPassShader::useFloorShader(glm::mat4 view, glm::mat4 proj, glm::vec3 viewPos)
{
	glm::mat4 model = glm::mat4(1.0);

	_floorShader.use();

	_floorShader.setMat4("model", model);
	_floorShader.setMat4("view", view);
	_floorShader.setMat4("projection", proj);

	_floorShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
	_floorShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
	_floorShader.setVec3("lightPos", Light::getInstance()->getPosition());
	_floorShader.setVec3("viewPos", viewPos);

	_floorShader.setMat4("lightView", Light::getInstance()->getViewMatirx());
	_floorShader.setMat4("lightProjection", Light::getInstance()->getProjectionMatrix());

}

} // namespace CGRA350
