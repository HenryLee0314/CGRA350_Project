#include "grass_parameters.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

namespace CGRA350 {

GrassParameters* GrassParameters::_instance = nullptr;

GrassParameters* GrassParameters::getInstance()
{
	if (_instance == nullptr) {
		_instance = new GrassParameters();
	}

	return _instance;
}

GrassParameters::GrassParameters()
    : Ws_coefficient(3000)
    , Rs_coefficient(152)
	, Wb_coefficient(1000)
	, Rb_coefficient(500)
	, Angle_coefficient(0.005)
{

}

GrassParameters::~GrassParameters()
{

}

void GrassParameters::renderGUI()
{
	ImGui::SliderFloat("Ws coefficient", &Ws_coefficient, 0, 100000);
	ImGui::SliderFloat("Rs coefficient", &Rs_coefficient, 0, 1000);

	ImGui::SliderFloat("Wb coefficient", &Wb_coefficient, 0, 1000);
	ImGui::SliderFloat("Rb coefficient", &Rb_coefficient, 0, 1000);

	ImGui::SliderFloat("Angle coefficient", &Angle_coefficient, 0, 0.1);
}

} // namespace CGRA350