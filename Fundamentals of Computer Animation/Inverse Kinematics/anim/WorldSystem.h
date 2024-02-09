#ifndef MY_WORLD_SYSTEM_H
#define MY_WORLD_SYSTEM_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseObject.h"
#include "BaseSystem.h"
#include "HermiteSystem.h"
#include "GlobalResourceManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

#include <assert.h>

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class WorldSystem : public BaseSystem
{
public:
	WorldSystem(const std::string& name);
	virtual void getState(double* p0);
	std::vector<double> getRot();
	void getEnd(Vector e);
	virtual void setState(double* p0, std::vector<double> p1);
	void WorldSystem::rest(std::vector<double> left, std::vector<double> right);
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);

protected:
	
};

#endif