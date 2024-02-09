#ifndef MY_HAND_SIMULATOR_H
#define MY_HAND_SIMULATOR_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "HandSystem.h"
#include "Eigen/Dense"
using Eigen::MatrixXd;
using Eigen::VectorXd;
#include <string>

class HandSimulator : public BaseSimulator
{
public:

	HandSimulator( const std::string& name, BaseSystem* target);
	~HandSimulator();

	int step(double time);
	int init(double time)
	{
		return 0;
	};
	void force(int i, Vector Fi);
	void collision(int i, Vector F);
	void calcPoint(double r1, double r2, int i, int j);
	
protected:

	Vector m_pos0; // initial position
	Vector m_rot0;
	Vector m_pos;
	BaseSystem* m_object;
	HandSystem* sys;
	boolean col = false;
	int totalCol();
	int command(int argc, myCONST_SPEC char** argv);
};


#endif