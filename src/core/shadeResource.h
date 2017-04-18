#ifndef _SHADE_RESOURCE_H_
#define _SHADE_RESOURCE_H_

#include"common\common.h"
#include"core\glProgram.h"
#include"core\glProgramState.h"
#include<map>
#include<unordered_map>

/**
*	\brief �Ե���ģʽʵ����Ⱦ����Դ�Ĵ洢
*/
class ShadeResourceCache
{
public:
	ShadeResourceCache();
	~ShadeResourceCache();

	static ShadeResourceCache& GetInstance();

	void LoadDefaultProgram();
	GLProgramPtr GetGLProgram(const string& key);

private:
	std::unordered_map<std::string, GLProgramPtr> mPrograms;

};


#endif