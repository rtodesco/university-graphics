#include "HandSimulator.h"

double h[57] = { -0.8,-0.1,0 , 0,0,0 , -0.2,0.2,0 , -0.5,0.5,0 , 0,0,0 , -0.05,0.3,0, -0.05,0.3,0 , 0,0.5,0 , 0,0,0 , 0,0.4,0 , 0,0.4,0 , 0.5,0.5,0 , 0,0,0 , 0,0.3,0 , 0,0.3,0 , 0.8,0.25,0, 0,0,0 , 0.05,0.25,0 , 0.05,0.25,0 };
int rotation;
boolean inst1 = false;
boolean inst2 = false;
boolean g = true;
Vector f = { 0,0,-1 };
Vector handF = { 0,12,0 };
MatrixXd rollX(double r);
MatrixXd rollY(double r);
boolean grip = false;

HandSimulator::HandSimulator( const std::string& name, BaseSystem* target):
	BaseSimulator( name ),
	m_object(target)
{
	sys = dynamic_cast<HandSystem*>(target);
	
}	// HandSimulator

HandSimulator::~HandSimulator()
{
}	// HandSimulator::~HandSimulator

int HandSimulator::step(double time) {
	Vector a;
	std::vector<double> v;
	std::vector<double> p;
	
	rotation = 0;
	for (int i = 0; i < sys->particles.size() / 13; i++) {
		Vector Fi;
		setVector(Fi, 0, 0, 0);
		
		force(i, Fi);
		collision(i, Fi);
		
		a[0] = Fi[0] / sys->particles[(i * 13) + 9];
		a[1] = Fi[1] / sys->particles[(i * 13) + 9];
		a[2] = Fi[2] / sys->particles[(i * 13) + 9];
		
		v.push_back(sys->particles[(i * 13) + 3] + (a[0] * 0.01));
		v.push_back(sys->particles[(i * 13) + 4] + (a[1] * 0.01));
		v.push_back(sys->particles[(i * 13) + 5] + (a[2] * 0.01));
		
		p.push_back(sys->particles[(i * 13)]     + (sys->particles[(i * 13) + 3] * 0.01));
		p.push_back(sys->particles[(i * 13) + 1] + (sys->particles[(i * 13) + 4] * 0.01));
		p.push_back(sys->particles[(i * 13) + 2] + (sys->particles[(i * 13) + 5] * 0.01));
		sys->display();
	}

	for (int i = 0; i < sys->particles.size() / 13; i++) {
		sys->particles[(i * 13)] = p[(i * 3)];
		sys->particles[(i * 13) + 1] = p[(i * 3) + 1];
		sys->particles[(i * 13) + 2] = p[(i * 3) + 2];

		sys->particles[(i * 13) + 3] = v[(i * 3)];
		sys->particles[(i * 13) + 4] = v[(i * 3) + 1];
		sys->particles[(i * 13) + 5] = v[(i * 3) + 2];
	}
	return 0;

}	// HandSimulator::step

void HandSimulator::force(int i, Vector Fi) {
	double* rot = sys->getRot();
	double gravity = -9.8;
	setVector(Fi, 0, 0, 0);
	
	if (sys->particles[(i * 13) + 11] == 1) {
		if (sys->particles[(i * 13) + 12] == 0) { 
			Vector o = { sys->particles[13],sys->particles[14],sys->particles[15] };
			Vector p = { sys->particles[(i * 13)], sys->particles[(i * 13) + 1], sys->particles[(i * 13) + 2] };
			Vector temp; VecSubtract(temp, o, p);
			VecNormalize(temp);
			Fi[0] += temp[0] * 3;
			Fi[1] += temp[1] * 3;
			Fi[2] += temp[2] * 3;
			VecNormalize(f);
			double d = acos(VecDotProd(f, temp)) * (180 / PI);
			double* rot = sys->getRot();

			//x rotation
			if (abs(d) - abs(rot[rotation]) > 0.0001) {
				if (o[1] < p[1]) {
					rot[rotation] = rot[rotation] - 2;
				}
				else if (o[1] > p[1]) {
					rot[rotation] = rot[rotation] + 2;
				}
			}
			//y rotation
			if (abs(d) - abs(rot[rotation + 1]) > 0.0001) {
				if (o[0] < p[0]) {
					rot[rotation + 1] = rot[rotation + 1] + 2;
				}
				else if (o[0] > p[0]) {
					rot[rotation + 1] = rot[rotation + 1] - 2;
				}
			}
		}
		else if (totalCol() == 15) { 
			Fi[0] = handF[0];
			Fi[1] = handF[1];
			Fi[2] = handF[2];
			double massAdj0 = (sys->particles[22] - 1) * (handF[0] + sys->particles[19]);
			double massAdj1 = (sys->particles[22] - 1) * (handF[1] + sys->particles[20]);
			double massAdj2 = (sys->particles[22] - 1) * (handF[2] + sys->particles[21]);
			sys->particles[(i * 13) + 6] = handF[0] + massAdj0;
			sys->particles[(i * 13) + 7] = handF[1] + massAdj1;
			sys->particles[(i * 13) + 8] = handF[2] + massAdj2;
		}
		else {
			Fi[0] -= sys->particles[19];
			sys->particles[(i * 13) + 6] = -sys->particles[19];
			Fi[1] -= sys->particles[20];
			sys->particles[(i * 13) + 7] = -sys->particles[20];
			Fi[2] -= sys->particles[21];
			sys->particles[(i * 13) + 8] = -sys->particles[21];
		}
		rotation += 3;
	}
	else if(sys->particles[(i * 13) + 11] == 0) {
		if (g) {
			Fi[1] += gravity * sys->particles[(i * 13) + 9];
			sys->particles[(i * 13) + 7] = gravity * sys->particles[(i * 13) + 9];
		}
	}
	else if (sys->particles[(i * 13) + 11] == 2) {
		Fi[0] += sys->particles[6];
		Fi[1] += sys->particles[7];
		Fi[2] += sys->particles[8];
		if (sys->particles[25] == 1) {
			if (sys->particles[(i * 13) + 12] == 0) {
				switch (rotation) {
				case 3:
					rot[rotation] = rot[rotation] - 0.5;
					rot[rotation + 1] = rot[rotation + 1] - 1.5;
					break;
				case 6:
					rot[rotation] = rot[rotation] - 0.5;
					rot[rotation + 1] = rot[rotation + 1] - 0.5;
					break;
				case 10:
					rot[rotation] = rot[rotation] - 0.5;
					break;
				case 14:
					rot[rotation] = rot[rotation] - 0.5;
					rot[rotation + 1] = rot[rotation + 1] + 0.5;
					break;
				case 18:
					rot[rotation] = rot[rotation] - 0.5;
					rot[rotation + 1] = rot[rotation + 1] + 0.5;
					break;
				}
			}
			calcPoint(rot[rotation], rot[rotation + 1], i, (rotation - 3));
			sys->setRot(rot);
		}
		rotation += 2;
	}
	else if(sys->particles[(i * 13) + 11] == 3) {
		Fi[0] += sys->particles[6];
		Fi[1] += sys->particles[7];
		Fi[2] += sys->particles[8];
		if (sys->particles[25] == 1) {
			if (sys->particles[(i * 13) + 12] == 0) {
				switch (rotation) {
				case 5:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 8:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 12:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 16:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 20:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				}
			}
			calcPoint(rot[rotation], 0, i, (rotation - 3));
			sys->setRot(rot);
		}
		rotation += 1;
	}
	else {
		Fi[0] += sys->particles[6];
		Fi[1] += sys->particles[7];
		Fi[2] += sys->particles[8];
		if (sys->particles[25] == 1) {
			if (sys->particles[(i * 13) + 12] == 0) {
				switch (rotation) {
				case 9:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 13:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 17:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				case 21:
					rot[rotation] = rot[rotation] - 0.4;
					break;
				}
			}
			calcPoint(rot[rotation], 0, i, (rotation - 3));
			sys->setRot(rot);
		}
		
		rotation += 1;
	}
}

void HandSimulator::collision(int i, Vector F) {
	double rx = sys->particles[(i * 13) + 10];
	Vector X = { sys->particles[(i * 13)],sys->particles[(i * 13) + 1],sys->particles[(i * 13) + 2]};
	Vector temp;
	Vector temp2;

	if (sys->particles[(i * 13) + 11] == 0 && !grip) {
		Vector X2 = { X[0],X[1] - rx,X[2] };
		Vector V = { sys->particles[(i * 13) + 3] ,sys->particles[(i * 13) + 4] ,sys->particles[(i * 13) + 5] };
		Vector P = { 0,-3,0 };
		Vector N = { 0,1,0 };
		VecSubtract(temp2, X2, P);
		if (VecDotProd(temp2, N) < 0) {
			Vector N2 = { N[0],N[1],N[2] };
			VecScale(N, -1000 * VecDotProd(temp2, N));
			VecScale(N2, 20 * VecDotProd(V, N2));
			VecSubtract(temp2, N, N2);
			VecAdd(F, F, temp2);
		}
	}
	for (int j = 0; j < sys->particles.size() / 13;j++) {
		if (i == j) { continue; }
		if (sys->particles[(i * 13) + 11] == 1 && sys->particles[(j * 13) + 11] == 2) { continue; } //palm-finger(2-rot)
		if (sys->particles[(i * 13) + 11] == 1 && sys->particles[(j * 13) + 11] == 3) { continue; } //palm-finger(1-rot1)
		if (sys->particles[(i * 13) + 11] == 1 && sys->particles[(j * 13) + 11] == 4) { continue; } //palm-finger(1-rot2)
		if (sys->particles[(i * 13) + 11] == 2 && sys->particles[(j * 13) + 11] == 1) { continue; } //finger(2-rot)-palm
		if (sys->particles[(i * 13) + 11] == 2 && sys->particles[(j * 13) + 11] == 2) { continue; } //finger(2-rot)-finger(2-rot)
		if (sys->particles[(i * 13) + 11] == 2 && sys->particles[(j * 13) + 11] == 3) { continue; } //finger(2-rot)-finger(1-rot1)
		if (sys->particles[(i * 13) + 11] == 2 && sys->particles[(j * 13) + 11] == 4) { continue; } //finger(2-rot)-finger(1-rot2)
		if (sys->particles[(i * 13) + 11] == 3 && sys->particles[(j * 13) + 11] == 1) { continue; } //finger(1-rot1)-palm
		if (sys->particles[(i * 13) + 11] == 3 && sys->particles[(j * 13) + 11] == 2) { continue; } //finger(1-rot1)-finger(2-rot)
		if (sys->particles[(i * 13) + 11] == 3 && sys->particles[(j * 13) + 11] == 3) { continue; } //finger(1-rot1)-finger(1-rot1)
		if (sys->particles[(i * 13) + 11] == 3 && sys->particles[(j * 13) + 11] == 4) { continue; } //finger(1-rot1)-finger(1-rot2)
		if (sys->particles[(i * 13) + 11] == 4 && sys->particles[(j * 13) + 11] == 1) { continue; } //finger(1-rot2)-palm
		if (sys->particles[(i * 13) + 11] == 4 && sys->particles[(j * 13) + 11] == 2) { continue; } //finger(1-rot2)-finger(2-rot)
		if (sys->particles[(i * 13) + 11] == 4 && sys->particles[(j * 13) + 11] == 3) { continue; } //finger(1-rot2)-finger(1-rot1)
		if (sys->particles[(i * 13) + 11] == 4 && sys->particles[(j * 13) + 11] == 4) { continue; } //finger(1-rot2)-finger(1-rot2)

		double rp = sys->particles[(j * 13) + 10];
		Vector P = { sys->particles[(j * 13)],sys->particles[(j * 13) + 1],sys->particles[(j * 13) + 2]};
		VecSubtract(temp,X, P);

		if (sys->particles[(i * 13) + 11] == 1) {
			if (((VecLength(temp) + rx) - rp) < 0.3) {
				if (sys->particles[(i * 13) + 12] == 0) {
					sys->particles[(i * 13) + 12] = 1;
					grip = true;
					F[0] = -(sys->particles[(i * 13) + 3] / 0.01);
					F[1] = -(sys->particles[(i * 13) + 4] / 0.01);
					F[2] = -(sys->particles[(i * 13) + 5] / 0.01);
				}
				else if(inst1) {
					F[0] += sys->particles[(j * 13) + 6];
					F[1] += sys->particles[(j * 13) + 7];
					F[2] += sys->particles[(j * 13) + 8];
					
				}
				else {
					inst1 = true;
					F[0] -= sys->particles[(i * 13) + 6];
					F[1] -= sys->particles[(i * 13) + 7];
					F[2] -= sys->particles[(i * 13) + 8];
				}
			}
			else {
				sys->particles[(i * 13) + 12] = 0;
				grip = false;
			}
		}
		else if (sys->particles[(i * 13) + 11] == 2) {
			if (((VecLength(temp) + rx) - rp) < 0.2) {
				sys->particles[(i * 13) + 12] = 1;
				F[0] = -(sys->particles[(i * 13) + 3] / 0.01);
				F[1] = -(sys->particles[(i * 13) + 4] / 0.01);
				F[2] = -(sys->particles[(i * 13) + 5] / 0.01);
			}
		}
		else if (sys->particles[(i * 13) + 11] == 3) {
			if (((VecLength(temp) + rx) - rp) < 0.2) {
				sys->particles[(i * 13) + 12] = 1;
				F[0] = -(sys->particles[(i * 13) + 3] / 0.01);
				F[1] = -(sys->particles[(i * 13) + 4] / 0.01);
				F[2] = -(sys->particles[(i * 13) + 5] / 0.01);
			}
		}
		else if (sys->particles[(i * 13) + 11] == 4) {
			if (((VecLength(temp) + rx) - rp) < 0.2) {
				sys->particles[(i * 13) + 12] = 1;
				F[0] = -(sys->particles[(i * 13) + 3] / 0.01);
				F[1] = -(sys->particles[(i * 13) + 4] / 0.01);
				F[2] = -(sys->particles[(i * 13) + 5] / 0.01);
			}
		}
		else {
			if (sys->particles[(j * 13) + 11] == 1) {
				if (((VecLength(temp) + rp) - rx) < 0.3) {
					if (sys->particles[(i * 13) + 12] == 0) {
						sys->particles[(i * 13) + 12] = 1;
						F[0] = -(sys->particles[(i * 13) + 3] / 0.01) * sys->particles[(i * 13) + 9];
						F[1] = -(sys->particles[(i * 13) + 4] / 0.01) * sys->particles[(i * 13) + 9];
						F[2] = -(sys->particles[(i * 13) + 5] / 0.01) * sys->particles[(i * 13) + 9];
					}
					else if(inst2) {
						F[0] += sys->particles[(j * 13) + 6];
						F[1] += sys->particles[(j * 13) + 7];
						F[2] += sys->particles[(j * 13) + 8];
					}
					else {
						inst2 = true;
						F[0] -= sys->particles[(i * 13) + 6];
						F[1] -= sys->particles[(i * 13) + 7];
						F[2] -= sys->particles[(i * 13) + 8];
				
					}
				}
				else {
					sys->particles[(i * 13) + 12] = 0;
				}
			}
			else {
				if (((VecLength(temp) + rp) - rx) < 0.2) {
					sys->particles[(i * 13) + 12] = 1;
				}
			}
		}	
	}
}

void HandSimulator::calcPoint(double r1, double r2, int i, int j) {
	MatrixXd tRoot = MatrixXd::Zero(4, 4);
	tRoot(0, 0) = 1; tRoot(1, 1) = 1; tRoot(2, 2) = 1; tRoot(3, 3) = 1;
	tRoot(0, 3) = sys->particles[0];tRoot(1, 3) = sys->particles[1];tRoot(2, 3) = sys->particles[2];
	double* rot = sys->getRot();
	VectorXd p;
	if (sys->particles[(i * 13) + 11] == 2) {
		VectorXd tFinger1 = VectorXd::Zero(4);
		tFinger1(0) = h[j * 3]; tFinger1(1) = h[(j * 3) + 1]; tFinger1(2) = h[(j * 3) + 2]; tFinger1(3) = 1;

		p = tRoot * rollX(rot[0]) * rollY(rot[1]) * rollX(r1) * rollY(r2) * tFinger1;
	}
	else if (sys->particles[(i * 13) + 11] == 3) {
		MatrixXd tFinger1 = MatrixXd::Zero(4,4);
		tFinger1(0, 0) = 1; tFinger1(1, 1) = 1; tFinger1(2, 2) = 1; tFinger1(3, 3) = 1;
		tFinger1(0,3) = h[(j*3)-6]; tFinger1(1,3) = h[(j * 3) - 5]; tFinger1(2,3) = h[(j * 3) -4];

		VectorXd tFinger2 = VectorXd::Zero(4);
		tFinger2(0) = h[j * 3]; tFinger2(1) = h[(j * 3) + 1]; tFinger2(2) = h[(j * 3) + 2]; tFinger2(3) = 1;

		p = tRoot * rollX(rot[0]) * rollY(rot[1]) * rollX(rot[j+1]) * rollY(rot[j+2]) * tFinger1 * rollX(r1) * tFinger2;
	}
	else {
		MatrixXd tFinger1 = MatrixXd::Zero(4, 4);
		tFinger1(0, 0) = 1; tFinger1(1, 1) = 1; tFinger1(2, 2) = 1; tFinger1(3, 3) = 1;
		tFinger1(0, 3) = h[(j * 3) - 9]; tFinger1(1, 3) = h[(j * 3) - 8]; tFinger1(2, 3) = h[(j * 3) - 7];

		MatrixXd tFinger2 = MatrixXd::Zero(4, 4);
		tFinger2(0, 0) = 1; tFinger2(1, 1) = 1; tFinger2(2, 2) = 1; tFinger2(3, 3) = 1;
		tFinger2(0, 3) = h[(j * 3) - 3]; tFinger2(1, 3) = h[(j * 3) - 2]; tFinger2(2, 3) = h[(j * 3) - 1];

		VectorXd tFinger3 = VectorXd::Zero(4);
		tFinger3(0) = h[j * 3]; tFinger3(1) = h[(j * 3) + 1]; tFinger3(2) = h[(j * 3) + 2]; tFinger3(3) = 1;

		p = tRoot * rollX(rot[0]) * rollY(rot[1]) * rollX(rot[j]) * rollY(rot[j + 1]) * tFinger1 * rollX(rot[j + 2]) * tFinger2 * rollX(r1) * tFinger3;
	}
	
	sys->particles[(i * 13)] = p[0];sys->particles[(i * 13)+1] = p[1];sys->particles[(i * 13)+2] = p[2];

}

MatrixXd rollX(double r) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0, 0) = 1;
	mat(1, 1) = cos(r * (3.14/180)); mat(1, 2) = -sin(r * (3.14 / 180));
	mat(2, 1) = sin(r * (3.14 / 180)); mat(2, 2) = cos(r * (3.14 / 180));
	mat(3, 3) = 1;
	return mat;
}

MatrixXd rollY(double r) {
	MatrixXd mat = MatrixXd::Zero(4, 4);
	mat(0, 0) = cos(r * (3.14 / 180)); mat(0, 2) = sin(r * (3.14 / 180));
	mat(1, 1) = 1;
	mat(2, 0) = -sin(r * (3.14 / 180)); mat(2, 2) = cos(r * (3.14 / 180));
	mat(3, 3) = 1;
	return mat;
}

int HandSimulator::totalCol() {
	int x = 0;
	for (int i = 1; i < sys->particles.size() / 13; i++) {
		x += sys->particles[(i * 13) + 12];
	}
	return x;
}

int HandSimulator::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[1], "hand") == 0) {
		handF[0] = atof(argv[2]);
		handF[1] = atof(argv[3]);
		handF[2] = atof(argv[4]);
	}
	else if (strcmp(argv[1], "gravity") == 0) {
		if (g) { g = false; }
		else { g = true; }
	}

	glutPostRedisplay();
	return TCL_OK;
}