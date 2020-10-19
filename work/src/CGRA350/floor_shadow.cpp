#include "floor_shadow.h"
#include "floor.h"
#include "grass_bundle.h"

#include "shader_first_pass.h"

#include "opengl.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

namespace CGRA350 {

#ifdef __APPLE__
const GLuint SHADOW_WIDTH = 2000 * 2, SHADOW_HEIGHT = 2000 * 2;
#else
const GLuint SHADOW_WIDTH = 2000, SHADOW_HEIGHT = 2000;
#endif

FloorShadow* FloorShadow::_instance = nullptr;

FloorShadow* FloorShadow::getInstance()
{
	if (_instance == nullptr) {
		_instance = new FloorShadow();
	}

	return _instance;
}

glm::mat4 FloorShadow::getProjectionMatrix()
{
	GLfloat near_plane = 0.01f, far_plane = 1000.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	return lightProjection;
}

glm::mat4 FloorShadow::getViewMatirx(glm::vec3 lightPosition)
{
	glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return lightView;
}

FloorShadow::FloorShadow()
	: _depthMapFBO(0)
	, _depthMap(0)
{

	glGenFramebuffers(1, &_depthMapFBO);

	glGenTextures(1, &_depthMap);
	glBindTexture(GL_TEXTURE_2D, _depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FloorShadow::~FloorShadow()
{

}

void FloorShadow::renderGUI()
{

}

void FloorShadow::renderShadow()
{
	CGRA_ACTIVITY_START(TWO_PASS);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	FirstPassShader::getInstance()->useGrassShader();
	GrassBundle::getInstance()->render();
    FirstPassShader::getInstance()->useFloorShader();
    Floor::getInstance()->render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CGRA_ACTIVITY_END(TWO_PASS);
}

void FloorShadow::render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depthMap);

	Floor::getInstance()->render();
}

} // namespace CGRA350