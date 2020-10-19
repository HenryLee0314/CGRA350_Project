#include "light.h"

#include "cgra/cgra_gui.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace CGRA350 {

Light* Light::_instance = nullptr;

Light* Light::getInstance()
{
	if (_instance == nullptr) {
		_instance = new Light();
	}

	return _instance;
}

Light::Light()
: _position(3.0, 10.0, 3.0)
{

}

Light::~Light()
{

}

glm::mat4 Light::getProjectionMatrix()
{
	GLfloat near_plane = 0.01f, far_plane = 1000.0f;
	return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
}

glm::mat4 Light::getViewMatirx()
{
	return glm::lookAt(_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Light::renderGUI()
{
	float data[3] = {_position.x, _position.y, _position.z};
	ImGui::Text("Light Position");
	ImGui::SliderFloat(" X", &data[0], -5.0f, 5.0f);
	// ImGui::SliderFloat(" Y", &data[1], 0.0f, 20.0f);
	ImGui::SliderFloat(" Z", &data[2], -5.0f, 5.0f);
	_position.x = data[0];
	_position.y = data[1];
	_position.z = data[2];
}

} // namespace CGRA350