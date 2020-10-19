#ifndef FLOOR_SHADOW_H
#define FLOOR_SHADOW_H

#include <stdint.h>

#include <glm/glm.hpp>

namespace CGRA350 {

class FloorShadow
{
public:
	static FloorShadow* getInstance();

	virtual void renderGUI();

	void render();

	void renderShadow();

	glm::mat4 getProjectionMatrix();

	glm::mat4 getViewMatirx(glm::vec3 lightPosition);

protected:
	FloorShadow();
	virtual ~FloorShadow();
	FloorShadow(const FloorShadow&);
	FloorShadow& operator = (const FloorShadow&);

	static FloorShadow* _instance;

private:
	uint32_t _depthMapFBO;
	uint32_t _depthMap;
};

} // namespace CGRA350

#endif // FLOOR_SHADOW_H