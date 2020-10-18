#ifndef SHADER_SECOND_PASS_H
#define SHADER_SECOND_PASS_H

#include "shader.h"

namespace CGRA350 {

class SecondPassShader
{
public:
	static SecondPassShader* getInstance();

	void useFloorShader(glm::mat4 view, glm::mat4 proj, glm::vec3 viewPos);

private:
	SecondPassShader();
	virtual ~SecondPassShader();
	SecondPassShader(const SecondPassShader&);
	SecondPassShader& operator = (const SecondPassShader&);

private:
	static SecondPassShader* _instance;

	Shader _floorShader;
};

} // namespace CGRA350

#endif // SHADER_SECOND_PASS_H