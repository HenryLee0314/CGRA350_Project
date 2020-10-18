#include "floor_shadow.h"
#include "floor.h"
#include "grass_bundle.h"

#include "shader_first_pass.h"

#include "opengl.hpp"

namespace CGRA350 {

#ifdef __APPLE__
const GLuint SHADOW_WIDTH = 1280 * 2, SHADOW_HEIGHT = 800 * 2;
#else
const GLuint SHADOW_WIDTH = 1280, SHADOW_HEIGHT = 800;
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
	: /*_hasChanged(true)
	, _elevationDivisions(10)
	, _azimuthalAnglesDivisions(10)
	, _vertices(nullptr)
	, _indices(nullptr)
	, _VAO(0)
	, _VBO(0)
	, _EBO(0)*/
	 _depthMapFBO(0)
	, _depthMap(0)
{
	// glGenVertexArrays(1, &_VAO);
	// glGenBuffers(1, &_VBO);
	// glGenBuffers(1, &_EBO);

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
	// glDeleteVertexArrays(1, &_VAO);
	// glDeleteBuffers(1, &_VBO);
	// glDeleteBuffers(1, &_EBO);
}

void FloorShadow::renderGUI()
{
	// if (ImGui::SliderInt("Elevation Divisions", &_elevationDivisions, 2, 40)) {
	// 	_hasChanged = true;
	// }

	// if (ImGui::SliderInt("Azimuthal Angles Divisions", &_azimuthalAnglesDivisions, 3, 40))
	// {
	// 	_hasChanged = true;
	// }
}

void FloorShadow::renderShadow()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//render();


	FirstPassShader::getInstance()->useGrassShader();
	GrassBundle::getInstance()->render();
    FirstPassShader::getInstance()->useFloorShader();
    Floor::getInstance()->render();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FloorShadow::render()
{
	// if (_hasChanged) {
	// 	if (_vertices != nullptr) {
	// 		CGRA_FREE(_vertices);
	// 		_vertices = nullptr;
	// 	}

	// 	if (_indices != nullptr) {
	// 		CGRA_FREE(_indices);
	// 		_indices = nullptr;
	// 	}

	// 	_vertices = (float*)CGRA_MALLOC(sizeof(float) * ((_elevationDivisions + 1) * (_azimuthalAnglesDivisions + 1) * (3 + 3)), CGRA350);
	// 	_indices = (uint32_t*)CGRA_MALLOC(sizeof(uint32_t) * (_elevationDivisions * _azimuthalAnglesDivisions * 3 * 2), CGRA350);

	// 	for (int i = 0; i <= _elevationDivisions; ++i) {
	// 		for (int j = 0; j <= _azimuthalAnglesDivisions; ++j) {
	// 			Vec3 point;
	// 			point.x = cos(PI / _elevationDivisions * i - (PI / 2)) * cos(2 * PI / _azimuthalAnglesDivisions * j);
	// 			point.y = cos(PI / _elevationDivisions * i - (PI / 2)) * sin(2 * PI / _azimuthalAnglesDivisions * j);
	// 			point.z = sin(PI / _elevationDivisions * i - (PI / 2));

	// 			point.unit();

	// 			// vertices
	// 			_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 0] = point.x;
	// 			_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 1] = point.y;
	// 			_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 2] = point.z;

	// 			// normal
	// 			_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 3] = point.x;
	// 			_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 4] = point.y;
	// 			_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 5] = point.z;


	// 			if (i < _elevationDivisions && j < _azimuthalAnglesDivisions) {
	// 				_indices[6 * (i * _azimuthalAnglesDivisions + j) + 0] = i * (_azimuthalAnglesDivisions + 1) + j;
	// 				_indices[6 * (i * _azimuthalAnglesDivisions + j) + 1] = (i + 1) * (_azimuthalAnglesDivisions + 1) + j;
	// 				_indices[6 * (i * _azimuthalAnglesDivisions + j) + 2] = (i + 1) * (_azimuthalAnglesDivisions + 1) + (j + 1);

	// 				_indices[6 * (i * _azimuthalAnglesDivisions + j) + 3] = i * (_azimuthalAnglesDivisions + 1) + j;
	// 				_indices[6 * (i * _azimuthalAnglesDivisions + j) + 4] = (i + 1) * (_azimuthalAnglesDivisions + 1) + (j + 1);
	// 				_indices[6 * (i * _azimuthalAnglesDivisions + j) + 5] = i * (_azimuthalAnglesDivisions + 1) + (j + 1);
	// 			}


	// 		}
	// 	}

	// 	glBindVertexArray(_VAO);
	// 	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((_elevationDivisions + 1) * (_azimuthalAnglesDivisions + 1) * (3 + 3)), _vertices, GL_STATIC_DRAW);
	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * (_elevationDivisions * _azimuthalAnglesDivisions * 3 * 2), _indices, GL_STATIC_DRAW);

	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 	glEnableVertexAttribArray(0);
	// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// 	glEnableVertexAttribArray(1);
	// 	glBindVertexArray(0);

	// 	_hasChanged = false;
	// }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depthMap);

    // render grass
	// glBindVertexArray(_VAO);
	// glDrawElements(GL_TRIANGLES, _elevationDivisions * _azimuthalAnglesDivisions * 2 * 3, GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0);

	Floor::getInstance()->render();
}

} // namespace CGRA350