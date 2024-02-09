#ifndef MY_OBJECT_PATH_SYSTEM_H
#define MY_OBJECT_PATH_SYSTEM_H

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

class ObjectPathSystem : public BaseSystem
{
public:
	ObjectPathSystem(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	HermiteSystem* getSplineSystem();
	void setRot(Vector f, Vector u);
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);
	void reset(double time);

protected:
	float m_sx;
	float m_sy;
	float m_sz;

	Vector m_pos;
	GLMmodel m_model;
};

#endif