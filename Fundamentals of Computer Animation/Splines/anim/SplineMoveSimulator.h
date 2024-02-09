#ifndef MY_SPLINE_SIMULATOR_H
#define MY_SPLINE_SIMULATOR_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "HermiteSystem.h"
#include "ObjectPathSystem.h"

#include <string>

class SplineMoveSimulator : public BaseSimulator 
{
public:

	SplineMoveSimulator( const std::string& name, BaseSystem* target );
	~SplineMoveSimulator();

	int step(double time);
	int init(double time) 
	{ 
		m_object->getState(m_pos0);
		vel = 0;
		return 0;
	};
	void reset(double time);

protected:

	Vector m_pos0; // initial position
	Vector m_rot0;
	Vector m_pos;
	double vel;
	double d;

	BaseSystem* m_object;

};


#endif