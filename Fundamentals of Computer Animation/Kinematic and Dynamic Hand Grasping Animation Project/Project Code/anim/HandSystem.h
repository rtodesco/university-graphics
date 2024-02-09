#ifndef MY_HAND_SYSTEM_H
#define MY_HAND_SYSTEM_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseObject.h"
#include "BaseSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

#include <assert.h>

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class HandSystem : public BaseSystem {
public:
	HandSystem(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	double* getRot();
	void setRot(double* newRot);
	void display(GLenum mode = GL_RENDER);
	int command(int argc, myCONST_SPEC char** argv);
	std::vector<double> particles;

protected:

	Vector m_pos;
	
};

#endif