#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace CGRA350 {

class Light
{
public:
	static Light* getInstance();

	void renderGUI();

	inline glm::vec3 getPosition() {
		return _position;
	}

	glm::mat4 getViewMatirx();

	glm::mat4 getProjectionMatrix();

private:
	Light();
	virtual ~Light();
	Light(const Light&);
	Light& operator = (const Light&);

private:
	static Light* _instance;

	glm::vec3 _position;
};

} // namespace CGRA350

#endif // LIGHT_H