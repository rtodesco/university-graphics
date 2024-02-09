#include "SplineMoveSimulator.h"

HermiteSystem* h2;
boolean first = true;
double t1; double t2; double tf; double t;
double d1; double a0; double v1;

SplineMoveSimulator::SplineMoveSimulator( const std::string& name, BaseSystem* target ):
	BaseSimulator( name ),
	m_object( target )
{
	d = 0.0;
	v1 = 3.5;
	h2 = dynamic_cast<ObjectPathSystem*>(target)->getSplineSystem();
}	// SplineMoveSimulator

SplineMoveSimulator::~SplineMoveSimulator()
{
}	// SplineMoveSimulator::~SplineMoveSimulator

int SplineMoveSimulator::step(double time) {
	if (first) {
		t1 = (2.0 * h2->getArcLength(0.1)) / v1;
		t2 = ((h2->getArcLength(0.9) - h2->getArcLength(0.1)) / v1) + t1;
		tf = ((2.0 * (h2->getArcLength(1.0) - h2->getArcLength(0.9))) / v1) + t2;
		t1 = t1 / tf; t2 = t2 / tf; a0 = v1 / t1;
		d1 = ((1.0 / 2.0) * (a0 * t1) * t1) + ((a0 * t1) * (t2 - t1)) + ((1.0 / 2.0) * (a0 * t1) * (1 - t2));
		first = false;
	}
	t = time / tf;

	if (abs(d-1.0) > 0.00001) {
		Vector pos; vector<double> S = h2->getS();
		Vector wV; Vector uTemp; Vector uV; Vector vV;
		
		m_object->getState(pos);
		animTcl::OutputMessage("%f",d);
		auto it = std::upper_bound(S.begin(), S.end(), S.back() * d);
		int index = it - S.begin();
		
		double u = h2->getU()[index];
		int i = ((int)(u * ((h2->getSpline().size() / 3.0) - 1)))*3;

		pos[0] = h2->getSpline()[i];
		pos[1] = h2->getSpline()[i + 1];
		pos[2] = h2->getSpline()[i + 2];

		//first 10%
		if (t <= t1) {
			d = ((a0 * t1) * (pow(t, 2) / (2 * t1))) / d1;
			vel = (a0 * t1) * (t / t1);
		}
		//last 10%
		else if (t >= t2) {
			d = (((a0 * t1) * (t1 / 2)) + ((a0 * t1) * (t2 - t1)) + ((a0 * t1) * (t - t2) * (1 - ((t - t2) / (2 * (1 - t2)))))) / d1;
			vel = (a0 * t1) * (1 - ((t - t2) / (1 - t2)));
		}
		//10%-90%
		else {
			d = (((a0 * t1) * (t1 / 2)) + ((a0 * t1) * (t - t1))) / d1;
			vel = a0 * t1;
		}

		m_object->setState(pos);

		setVector(wV, h2->getSplineDx()[i], h2->getSplineDx()[i + 1], h2->getSplineDx()[i + 2]);
		setVector(uTemp, h2->getSplineDx2()[i], h2->getSplineDx2()[i + 1], h2->getSplineDx2()[i + 2]);
		VecCrossProd(uV, wV, uTemp);
		VecCrossProd(vV, wV, uV);
		VecNormalize(wV); VecNormalize(uV); VecNormalize(vV);

		dynamic_cast<ObjectPathSystem*>(m_object)->setRot(wV,uV);
	}
	
	if (time - (int)time < 0.01) {
		animTcl::OutputMessage("Simulation time: %lf, Car Velocity: %lf", time, vel);
	}
	
	return 0;

}	// SplineMoveSimulator::step

void SplineMoveSimulator::reset(double time) {
	d = 0.0;
}

