#include "shader_first_pass.h"
#include "light.h"

namespace CGRA350 {

FirstPassShader* FirstPassShader::_instance = nullptr;

FirstPassShader* FirstPassShader::getInstance()
{
	if (_instance == nullptr) {
		_instance = new FirstPassShader();
	}

	return _instance;
}

FirstPassShader::FirstPassShader()
	: _grassShader(CGRA_SRCDIR "/res/shaders/vertexShader/grass.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/first_pass.fs", CGRA_SRCDIR "/res/shaders/geometryShader/grass.gs", CGRA_SRCDIR "/res/shaders/tessellationControlShader/grass.tcs", CGRA_SRCDIR "/res/shaders/tessellationEvaluationShader/grass.tes")
	, _floorShader(CGRA_SRCDIR "/res/shaders/vertexShader/first_pass.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/first_pass.fs")
{

}

FirstPassShader::~FirstPassShader()
{

}

void FirstPassShader::useGrassShader()
{
	glm::mat4 model = glm::mat4(1.0);

	_grassShader.use();
	_grassShader.setMat4("model", model);
	_grassShader.setMat4("view", Light::getInstance()->getViewMatirx());
	_grassShader.setMat4("projection", Light::getInstance()->getProjectionMatrix());
}

void FirstPassShader::useFloorShader()
{
	glm::mat4 model = glm::mat4(1.0);

	_floorShader.use();
	_floorShader.setMat4("model", model);
	_floorShader.setMat4("view", Light::getInstance()->getViewMatirx());
	_floorShader.setMat4("projection", Light::getInstance()->getProjectionMatrix());
}

} // namespace CGRA350
