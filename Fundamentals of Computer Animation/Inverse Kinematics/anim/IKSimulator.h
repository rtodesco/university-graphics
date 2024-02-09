#ifndef MY_IK_SIMULATOR_H
#define MY_IK_SIMULATOR_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "HermiteSystem.h"
#include "WorldSystem.h"

#include "Eigen/Dense"
#include "Eigen/LU"
using Eigen::MatrixXd;
using Eigen::VectorXd;

#include <string>

class IKSimulator : public BaseSimulator 
{
public:

	IKSimulator( const std::string& name, BaseSystem* target );
	~IKSimulator();

	int step(double time);
	int init(double time) 
	{ 
		return 0;
	};
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);

protected:

	BaseSystem* m_object;

};


#endif