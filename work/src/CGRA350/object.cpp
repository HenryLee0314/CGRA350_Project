#include "object.h"

#include <memory>

namespace CGRA350 {
Object::Object(Object* parent)
	: _parent(parent)
{
	if (_parent != nullptr) {
		_parent->addChild(this);
	}
}

Object::~Object()
{

}

void Object::render()
{
	std::vector<std::shared_ptr<Object>>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++) {
		(*iter)->render();
	}
}

void Object::renderGUI()
{
	std::vector<std::shared_ptr<Object>>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++) {
		(*iter)->renderGUI();
	}
}

void Object::update()
{
	std::vector<std::shared_ptr<Object>>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++) {
		(*iter)->update();
	}
}

void Object::updateGlData()
{
	std::vector<std::shared_ptr<Object>>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++) {
		(*iter)->updateGlData();
	}
}


void Object::addChild(Object* child)
{
	if (child != nullptr) {
		_children.push_back(std::shared_ptr<Object>(child));
	}
}

} // namespace CGRA350
