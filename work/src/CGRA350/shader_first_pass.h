#ifndef SHADER_FIRST_PASS_H
#define SHADER_FIRST_PASS_H

#include "shader.h"

namespace CGRA350 {

class FirstPassShader
{
public:
	static FirstPassShader* getInstance();

	void useGrassShader();

	void useFloorShader();

private:
	FirstPassShader();
	virtual ~FirstPassShader();
	FirstPassShader(const FirstPassShader&);
	FirstPassShader& operator = (const FirstPassShader&);

private:
	static FirstPassShader* _instance;

	Shader _grassShader;
	Shader _floorShader;
};

} // namespace CGRA350

#endif // SHADER_FIRST_PASS_H