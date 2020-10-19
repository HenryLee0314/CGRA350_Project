#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace CGRA350 {

class Object
{
public:
	Object(Object* parent = nullptr);

	virtual ~Object();

	virtual void render();

	virtual void renderGUI();

	virtual void update();

	virtual void updateGlData();

protected:
	Object* _parent;
	std::vector<std::shared_ptr<Object>> _children;
	void addChild(Object* child);
};

} // namespace CGRA350

#endif // OBJECT_H
