#include "IKSimulator.h"

HermiteSystem* h = new HermiteSystem("hermite");
std::vector<double> currentTheta;
std::vector<double> newTheta;
double t = 0;
double d = 0;
boolean linear;
boolean spl;
Vector targetP;
Vector currentP;
Vector start;
VectorXd pHand = VectorXd::Zero(4);
MatrixXd tRoot = MatrixXd::Zero(4, 4);
MatrixXd tSh = MatrixXd::Zero(4, 4);
MatrixXd tShM = MatrixXd::Zero(4, 4);
MatrixXd tShRM = MatrixXd::Zero(4, 4);
MatrixXd tEl = MatrixXd::Zero(4, 4);
MatrixXd tElM = MatrixXd::Zero(4, 4);
MatrixXd tElRM = MatrixXd::Zero(4, 4);
MatrixXd tWr = MatrixXd::Zero(4, 4);
MatrixXd tWrM = MatrixXd::Zero(4, 4);
MatrixXd tWrRM = MatrixXd::Zero(4, 4);
MatrixXd calcJ();
void IKSolve(MatrixXd J, std::vector<double> currentTheta, Vector pTargetP);
void calcPoint(Vector p, std::vector<double> currentTheta);
MatrixXd rollX(double rot2);
MatrixXd deriveX(double rot2);
MatrixXd rollY(double rot2);
MatrixXd deriveY(double rot2);
MatrixXd rollZ(double rot2);
MatrixXd deriveZ(double rot2);
VectorXd calcColumn(MatrixXd r1, MatrixXd r2, MatrixXd r3, MatrixXd r4, MatrixXd r5, MatrixXd r6, MatrixXd r7);

IKSimulator::IKSimulator( const std::string& name, BaseSystem* target ):
	BaseSimulator( name ),
	m_object( target )
{
	for (int i = 0; i < 7; i++) {
		currentTheta.push_back(0);
	}
	linear = false;
	spl = false;
	pHand(0) = 0.4; pHand(3) = 1;
	
	tRoot(0, 0) = 1; tRoot(1, 1) = 1; tRoot(2, 2) = 1; tRoot(3, 3) = 1;
	tRoot(2,3) = 2.5;

	tSh(0,0) = 1; tSh(1,1) = 1; tSh(2,2) = 1; tSh(3,3) = 1;
	tSh(0,3) = 1.5; tSh(1,3) = 1;

	tShM(0, 0) = 1; tShM(1, 1) = 1; tShM(2, 2) = 1; tShM(3, 3) = 1;
	tShM(0, 3) = -1;

	tShRM(0, 0) = 1; tShRM(1, 1) = 1; tShRM(2, 2) = 1; tShRM(3, 3) = 1;
	tShRM(0, 3) = 1;

	tEl(0,0) = 1; tEl(1,1) = 1; tEl(2,2) = 1; tEl(3,3) = 1;
	tEl(0,3) = 1.7;

	tElM(0, 0) = 1; tElM(1, 1) = 1; tElM(2, 2) = 1; tElM(3, 3) = 1;
	tElM(0, 3) = -1;

	tElRM(0, 0) = 1; tElRM(1, 1) = 1; tElRM(2, 2) = 1; tElRM(3, 3) = 1;
	tElRM(0, 3) = 1;

	tWr(0,0) = 1; tWr(1,1) = 1; tWr(2,2) = 1; tWr(3,3) = 1;
	tWr(0,3) = 1.2;

	tWrM(0, 0) = 1; tWrM(1, 1) = 1; tWrM(2, 2) = 1; tWrM(3, 3) = 1;
	tWrM(0, 3) = -0.4;

	tWrRM(0, 0) = 1; tWrRM(1, 1) = 1; tWrRM(2, 2) = 1; tWrRM(3, 3) = 1;
	tWrRM(0, 3) = 0.4;

	calcPoint(start, currentTheta);
}	// IKSimulator

IKSimulator::~IKSimulator()
{
}	// IKSimulator::~IKSimulator

int IKSimulator::step(double time) {
	std::vector<double> sp = h->getSpline();
	
	Vector err;
	Vector r;
	dynamic_cast<WorldSystem*>(m_object)->getState(r);
	currentTheta = dynamic_cast<WorldSystem*>(m_object)->getRot();
	calcPoint(currentP,currentTheta);

	if (!spl) {
		
		Vector endE;
		dynamic_cast<WorldSystem*>(m_object)->getEnd(endE);
		targetP[0] = (1 - t) * start[0] + t * endE[0];
		targetP[1] = (1 - t) * start[1] + t * endE[1];
		targetP[2] = (1 - t) * start[2] + t * endE[2];
		t += 0.01;
		if (abs(targetP[0] - endE[0]) <= 0.0001 && abs(targetP[1] - endE[1]) <= 0.0001 && abs(targetP[2] - endE[2]) <= 0.0001) {
			t -= 0.01;
		}
	}
	else if (linear) {
		targetP[0] = (1 - t) * start[0] + t * sp[0];
		targetP[1] = (1 - t) * start[1] + t * sp[1];
		targetP[2] = (1 - t) * start[2] + t * sp[2];
		t += 0.01;
		if (abs(targetP[0] - sp[0]) <= 0.0001 && abs(targetP[1] - sp[1]) <= 0.0001 && abs(targetP[2] - sp[2]) <= 0.0001) {
			linear = false;
			t = 0;
		}
	}
	else {
		vector<double> S = h->getS();
		auto it = std::upper_bound(S.begin(), S.end(), S.back() * d);
		int index = it - S.begin();
		double u = h->getU()[index];
		int i = ((int)(u * ((sp.size() / 3.0) - 1))) * 3;
		
		targetP[0] = sp[i]; targetP[1] = sp[i + 1]; targetP[2] = sp[i + 2];
		d += 0.001;
		
		if (abs(d - 1.0) <= 0.00001) {
			linear = true;
			d = 0;
			std::vector<double> left; std::vector<double> right;
			left.push_back(0);left.push_back(0);left.push_back(75);left.push_back(0);left.push_back(-10);left.push_back(0);left.push_back(0);
			right.push_back(10);right.push_back(10);right.push_back(-75);right.push_back(10);right.push_back(10);right.push_back(10);right.push_back(10);
			dynamic_cast<WorldSystem*>(m_object)->rest(left, right);
			currentTheta = dynamic_cast<WorldSystem*>(m_object)->getRot();
			calcPoint(currentP, currentTheta);
			targetP[0] = start[0];
			targetP[1] = start[1];
			targetP[2] = start[2];
		}
	}

	do {
		currentTheta = dynamic_cast<WorldSystem*>(m_object)->getRot();
		MatrixXd J = calcJ();
		newTheta.clear();
		VecSubtract(err, targetP, currentP);
		IKSolve(J,currentTheta,err);
		calcPoint(currentP, newTheta);
		dynamic_cast<WorldSystem*>(m_object)->setState(r, newTheta);
	} while (VecLength(err) > 0.05);
		
	return 0;

}	// IKSimulator::step

int IKSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0) {
		h->load(argv[1]);
		std::vector<double> left; std::vector<double> right;
		left.push_back(0);left.push_back(0);left.push_back(75);left.push_back(0);left.push_back(-10);left.push_back(0);left.push_back(0);
		right.push_back(10);right.push_back(10);right.push_back(-75);right.push_back(10);right.push_back(10);right.push_back(10);right.push_back(10);
		dynamic_cast<WorldSystem*>(m_object)->rest(left, right);
		currentTheta = dynamic_cast<WorldSystem*>(m_object)->getRot();
		calcPoint(currentP, currentTheta);
		start[0] = currentP[0];
		start[1] = currentP[1];
		start[2] = currentP[2];
		spl = true;
		linear = true;
		t = 0;
	}

	glutPostRedisplay();
	return TCL_OK;

}	// ObjectPathSystem::command

MatrixXd calcJ() {
	MatrixXd J(3, 7);
	MatrixXd rx1 = rollX(currentTheta[0]); MatrixXd rx2 = rollX(currentTheta[3]);
	MatrixXd dx1 = deriveX(currentTheta[0]); MatrixXd dx2 = deriveX(currentTheta[3]);
	MatrixXd ry1 = rollY(currentTheta[1]); MatrixXd ry2 = rollY(currentTheta[4]); MatrixXd ry3 = rollY(currentTheta[5]);
	MatrixXd dy1 = deriveY(currentTheta[1]); MatrixXd dy2 = deriveY(currentTheta[4]); MatrixXd dy3 = deriveY(currentTheta[5]);
	MatrixXd rz1 = rollZ(currentTheta[2]); MatrixXd rz2 = rollZ(currentTheta[6]);
	MatrixXd dz1 = deriveZ(currentTheta[2]); MatrixXd dz2 = deriveZ(currentTheta[6]);
	VectorXd col(4);

	col = calcColumn(dx1, ry1, rz1, rx2, ry2, ry3, rz2);
	J(0, 0) = col(0); J(1, 0) = col(1); J(2, 0) = col(2);

	col = calcColumn(rx1, dy1, rz1, rx2, ry2, ry3, rz2);
	J(0, 1) = col(0); J(1, 1) = col(1); J(2, 1) = col(2);

	col = calcColumn(rx1, ry1, dz1, rx2, ry2, ry3, rz2);
	J(0, 2) = col(0); J(1, 2) = col(1); J(2, 2) = col(2);

	col = calcColumn(rx1, ry1, rz1, dx2, ry2, ry3, rz2);
	J(0, 3) = col(0); J(1, 3) = col(1); J(2, 3) = col(2);

	col = calcColumn(rx1, ry1, rz1, rx2, dy2, ry3, rz2);
	J(0, 4) = col(0); J(1, 4) = col(1); J(2, 4) = col(2);

	col = calcColumn(rx1, ry1, rz1, rx2, ry2, dy3, rz2);
	J(0, 5) = col(0); J(1, 5) = col(1); J(2, 5) = col(2);

	col = calcColumn(rx1, ry1, rz1, rx2, ry2, ry3, dz2);
	J(0, 6) = col(0); J(1, 6) = col(1); J(2, 6) = col(2);

	return J;
}

void IKSolve(MatrixXd J, std::vector<double> currentTheta, Vector pTargetP) {
	MatrixXd JT = J.transpose();
	MatrixXd temp = J * JT;
	VectorXd X(3);
	X(0) = pTargetP[0]; X(1) = pTargetP[1]; X(2) = pTargetP[2];
	VectorXd B = temp.fullPivLu().solve(X);

	VectorXd T = JT * B;
	for (int i = 0; i < 7; i++) {
		newTheta.push_back(currentTheta[i] + T[i]);
	}
}

void calcPoint(Vector p, std::vector<double> currentTheta) {
	VectorXd p1 = tRoot * tSh * tShM * rollX(currentTheta[0]) * rollY(currentTheta[1]) * rollZ(currentTheta[2]) * tShRM
		          * tEl * tElM * rollX(currentTheta[3]) * rollY(currentTheta[4]) * tElRM 
		          * tWr * tWrM * rollY(currentTheta[5]) * rollZ(currentTheta[6]) * tWrRM * pHand;

	p[0] = p1[0];
	p[1] = p1[1];
	p[2] = p1[2];
}

MatrixXd rollX(double rot2) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0,0) = 1;
	mat(1, 1) = cos(rot2 * (PI / 180)); mat(1, 2) = -sin(rot2 * (PI / 180));
	mat(2,1) = sin(rot2 * (PI / 180)); mat(2,2) = cos(rot2 * (PI / 180));
	mat(3,3) = 1;
	return mat;
}

MatrixXd deriveX(double rot2) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(1,1) = -sin(rot2 * (PI / 180)); mat(1,2) = -cos(rot2 * ((PI / 180)));
	mat(2,1) = cos(rot2 * (PI / 180)); mat(2,2) = -sin(rot2 * (PI / 180));
	return mat;
}

MatrixXd rollY(double rot2) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0,0) = cos(rot2 * (PI / 180)); mat(0,2) = sin(rot2 * (PI / 180));
	mat(1,1) = 1;
	mat(2,0) = -sin(rot2 * (PI / 180)); mat(2,2) = cos(rot2 * (PI / 180));
	mat(3,3) = 1;
	return mat;
}

MatrixXd deriveY(double rot2) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0,0) = -sin(rot2 * (PI / 180)); mat(0,2) = cos(rot2 * (PI / 180));
	mat(2,0) = -cos(rot2 * (PI / 180)); mat(2,2) = -sin(rot2 * (PI / 180));
	return mat;
}

MatrixXd rollZ(double rot2) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0,0) = cos(rot2 * (PI / 180)); mat(0,1) = -sin(rot2 * (PI / 180));
	mat(1,0) = sin(rot2 * (PI / 180)); mat(1,1) = cos(rot2 * (PI / 180));
	mat(2,2) = 1;
	mat(3,3) = 1;
	return mat;
}

MatrixXd deriveZ(double rot2) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0,0) = -sin(rot2 * (PI / 180)); mat(0,1) = -cos(rot2 * (PI / 180));
	mat(1,0) = cos(rot2 * (PI / 180)); mat(1, 1) = -sin(rot2 * (PI / 180));
	return mat;
}

VectorXd calcColumn(MatrixXd r1, MatrixXd r2, MatrixXd r3, MatrixXd r4, MatrixXd r5, MatrixXd r6, MatrixXd r7) {
	VectorXd col(4);
	col = tRoot * tSh * tShM * r1 * r2 * r3 * tShRM * tEl * tElM * r4 * r5 * tElRM * tWr * tWrM * r6 * r7 * tWrRM * pHand;
	return col;
}

void IKSimulator::display(GLenum mode) { 
	h->display();
}