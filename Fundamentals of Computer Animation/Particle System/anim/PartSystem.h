#ifndef MY_PARTICLE_SYSTEM_H
#define MY_PARTICLE_SYSTEM_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseObject.h"
#include "BaseSystem.h"

#include <string>

#include <assert.h>

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class PartSystem : public BaseSystem {
public:
	PartSystem(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);
	void addSpring(int p1, int p2);
	std::vector<double> particles;
	int numPart;

protected:

	Vector m_pos;
	
};

#endif