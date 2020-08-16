#include "basic.h"

namespace CGRA350 {

Basic* Basic::_instance = nullptr;

Basic* Basic::getInstance()
{
	if (_instance == nullptr) {
		_instance = new Basic();
	}

	return _instance;
}

Basic::Basic()
{

}

Basic::~Basic()
{

}

} // namespace CGRA350
