#ifndef MY_HERMITE_SYSTEM_H
#define MY_HERMITE_SYSTEM_H

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

class HermiteSystem : public BaseSystem {
public:
	HermiteSystem(const std::string& name);
	virtual void getState(double* p);
	vector<double> getU();
	vector<double> getS();
	vector<double> getSpline();
	vector<double> getSplineDx();
	vector<double> getSplineDx2();
	virtual void setState(double* p);
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);
	void arcLengthTable();
	void load(char* s);
	double getArcLength(double u);
	double points[40][6];

protected:
	
	double knots[40];
	int num;
};

#endif