
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"

#include "cgra_log.h"
#include "opencl_manager.h"
#include "opencl_task.h"


using namespace std;
using namespace cgra;
using namespace glm;
using namespace CGRA350;


// void basic_model::draw(const glm::mat4 &view, const glm::mat4 proj) {
// 	mat4 modelview = view * modelTransform;

// 	glUseProgram(shader); // load shader and variables
// 	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
// 	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
// 	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));

// 	mesh.draw(); // draw
// }


Application::Application(GLFWwindow *window)
	: m_window(window)
	, _camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f))
	, _grassShader(CGRA_SRCDIR "/res/shaders/vertexShader/grass.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/grass.fs", CGRA_SRCDIR "/res/shaders/geometryShader/grass.gs", CGRA_SRCDIR "/res/shaders/tessellationControlShader/grass.tcs", CGRA_SRCDIR "/res/shaders/tessellationEvaluationShader/grass.tes")
	, _fluidShader(CGRA_SRCDIR "/res/shaders/vertexShader/fluid.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/fluid.fs")
	, _fluidGrid()
	, _enable_grass(true)
	, _enable_fluidGrid(false)
{

	m_lightPosition = glm::vec3(3.0f, 3.0f, 3.0f);

	// OpenclManager::getInstance();

	// OpenclTask task(CGRA_SRCDIR "/res/openCL/helloworld.cl");

	// task.run();

}


void Application::render() {

	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// projection matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);

	// view matrix
	// mat4 view = translate(mat4(1), vec3(0, 0, -m_distance))
	//             * rotate(mat4(1), m_pitch, vec3(1, 0, 0))
	//             * rotate(mat4(1), m_yaw,   vec3(0, 1, 0));

	_camera.setYaw(m_yaw);
	_camera.setPitch(m_pitch);
	_camera.setDistance(m_distance);

	mat4 view = _camera.GetViewMatrix();

	static glm::mat4 model = glm::mat4(1.0f);

	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);
	(m_showWireframe) ? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE);



	// draw the model
	//m_model.draw(view, proj);

	if (_enable_grass) {
		_grassShader.use();
		_grassShader.setMat4("model", model);
		_grassShader.setMat4("view", view);
		_grassShader.setMat4("projection", proj);

		_grassShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
		_grassShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
		_grassShader.setVec3("lightPos", m_lightPosition);
		_grassShader.setVec3("viewPos", _camera.getPosition());
		_grass.render();
	}

	if (_enable_fluidGrid) {
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_fluidGrid.update();

		_fluidShader.use();
		_fluidShader.setMat4("model", model);
		_fluidShader.setMat4("view", view);
		_fluidShader.setMat4("projection", proj);

		_fluidShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
		_fluidShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
		_fluidShader.setVec3("lightPos", m_lightPosition);
		_fluidShader.setVec3("viewPos", _camera.getPosition());
		_fluidGrid.render();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);

	// helpful drawing options
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);

	if (ImGui::Button("Grass")) {
		_enable_grass = true;
		_enable_fluidGrid = false;
	}

	if (ImGui::Button("Fluid Grid")) {
		_enable_grass = false;
		_enable_fluidGrid = true;
	}


	ImGui::Separator();

	// example of how to use input boxes
	if (_enable_grass) {
		_grass.renderGUI();
	}

	if (_enable_fluidGrid) {
		_fluidGrid.renderGUI();

	}

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
		                 - acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
		               - acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_yaw > pi<float>()) m_yaw -= float(2 * pi<float>());
		else if (m_yaw < -pi<float>()) m_yaw += float(2 * pi<float>());
	}

	// updated mouse position
	m_mousePosition = vec2(xpos, ypos);
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)mods; // currently un-used

	// capture is left-mouse down
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_leftMouseDown = (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset; // currently un-used
	m_distance *= pow(1.1f, -yoffset);
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
