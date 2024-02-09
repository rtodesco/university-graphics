#ifndef MY_PARTICLE_SIMULATOR_H
#define MY_PARTICLE_SIMULATOR_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "PartSystem.h"
#include "GlobalResourceManager.h"

#include <string>

class PartSimulator : public BaseSimulator 
{
public:

	PartSimulator( const std::string& name);
	~PartSimulator();

	int step(double time);
	int init(double time)
	{
		return 0;
	};
	int command(int argc, myCONST_SPEC char** argv);
	void force(int i, Vector Fi);
	void display(GLenum mode = GL_RENDER);
	void collision(int i, Vector a);

protected:

	PartSystem* partSys;

	int integration;
	double timeStep;

	int numSpring;
	int usedSprings;
	double gravity;
	double drag;
	double ksG;
	double kdG;
	boolean ground;
};


#endif