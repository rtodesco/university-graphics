#include "PartSimulator.h"

std::vector<double> previous;
std::vector<double> springs;

PartSimulator::PartSimulator( const std::string& name):
	BaseSimulator( name )
{
	numSpring = 0;
	usedSprings = 0;
	drag = 0;
	gravity = 0;
	ground = false;
}	// PartSimulator

PartSimulator::~PartSimulator()
{
}	// PartSimulator::~PartSimulator

int PartSimulator::step(double time) {
	Vector a;
	std::vector<double> v;
	std::vector<double> p;
	for (int j = 0; j < usedSprings; j++) {
		springs[(j * 9) + 5] = 0;
		springs[(j * 9) + 6] = 0; springs[(j * 9) + 7] = 0; springs[(j * 9) + 8] = 0;
	}
	//Euler
	if (integration == 0) {
		for (int i = 0; i < partSys->numPart; i++) {
			
			if (partSys->particles[i * 8] == 0) {
				Vector Fi;
				force(i, Fi);
				a[0] = Fi[0] / partSys->particles[(i * 8) + 1];
				a[1] = Fi[1] / partSys->particles[(i * 8) + 1];
				a[2] = Fi[2] / partSys->particles[(i * 8) + 1];
				collision(i, a);
				v.push_back(partSys->particles[(i * 8) + 5] + (a[0] * timeStep));
				v.push_back(partSys->particles[(i * 8) + 6] + (a[1] * timeStep));
				v.push_back(partSys->particles[(i * 8) + 7] + (a[2] * timeStep));

				p.push_back(partSys->particles[(i * 8) + 2] + (partSys->particles[(i * 8) + 5] * timeStep));
				p.push_back(partSys->particles[(i * 8) + 3] + (partSys->particles[(i * 8) + 6] * timeStep));
				p.push_back(partSys->particles[(i * 8) + 4] + (partSys->particles[(i * 8) + 7] * timeStep));
				
			}
			else {
				v.push_back(partSys->particles[(i * 8) + 5]);
				v.push_back(partSys->particles[(i * 8) + 6]);
				v.push_back(partSys->particles[(i * 8) + 7]);

				p.push_back(partSys->particles[(i * 8) + 2]);
				p.push_back(partSys->particles[(i * 8) + 3]);
				p.push_back(partSys->particles[(i * 8) + 4]);
			}
			//draw
			partSys->display();
		}
	}
	//Symplectic
	else if (integration == 1) {
		for (int i = 0; i < partSys->numPart; i++) {
			if (partSys->particles[i * 8] == 0) {
				Vector Fi;
				force(i, Fi);
				a[0] = Fi[0] / partSys->particles[(i * 8) + 1];
				a[1] = Fi[1] / partSys->particles[(i * 8) + 1];
				a[2] = Fi[2] / partSys->particles[(i * 8) + 1];
				collision(i, a);
				v.push_back(partSys->particles[(i * 8) + 5] + (a[0] * timeStep));
				v.push_back(partSys->particles[(i * 8) + 6] + (a[1] * timeStep));
				v.push_back(partSys->particles[(i * 8) + 7] + (a[2] * timeStep));

				p.push_back(partSys->particles[(i * 8) + 2] + (v[(i * 3)] * timeStep));
				p.push_back(partSys->particles[(i * 8) + 3] + (v[(i * 3) + 1] * timeStep));
				p.push_back(partSys->particles[(i * 8) + 4] + (v[(i * 3) + 2] * timeStep));
			}
			else {
				v.push_back(partSys->particles[(i * 8) + 5]);
				v.push_back(partSys->particles[(i * 8) + 6]);
				v.push_back(partSys->particles[(i * 8) + 7]);

				p.push_back(partSys->particles[(i * 8) + 2]);
				p.push_back(partSys->particles[(i * 8) + 3]);
				p.push_back(partSys->particles[(i * 8) + 4]);
			}
			//draw
			partSys->display();
		}
	}
	//Verlet
	else {
		for (int i = 0; i < partSys->numPart; i++) {
			if (partSys->particles[i * 8] == 0) {
				Vector Fi;
				force(i, Fi);
				a[0] = Fi[0] / partSys->particles[(i * 8) + 1];
				a[1] = Fi[1] / partSys->particles[(i * 8) + 1];
				a[2] = Fi[2] / partSys->particles[(i * 8) + 1];
				collision(i, a);
				
				p.push_back((2 * partSys->particles[(i * 8) + 2]) - previous[i * 3] + (a[0] * pow(timeStep, 2)));
				p.push_back((2 * partSys->particles[(i * 8) + 3]) - previous[(i * 3)+1] + (a[1] * pow(timeStep, 2)));
				p.push_back((2 * partSys->particles[(i * 8) + 4]) - previous[(i * 3)+2] + (a[2] * pow(timeStep, 2)));

				v.push_back((p[i * 3] - previous[i * 3]) / 2 * timeStep);
				v.push_back((p[(i * 3)+1] - previous[(i * 3)+1]) / 2 * timeStep);
				v.push_back((p[(i * 3)+2] - previous[(i * 3)+2]) / 2 * timeStep);
			}
			else {
				v.push_back(partSys->particles[(i * 8) + 5]);
				v.push_back(partSys->particles[(i * 8) + 6]);
				v.push_back(partSys->particles[(i * 8) + 7]);

				p.push_back(partSys->particles[(i * 8) + 2]);
				p.push_back(partSys->particles[(i * 8) + 3]);
				p.push_back(partSys->particles[(i * 8) + 4]);
			}
			//draw
			partSys->display();
		}
	}
	for (int i = 0; i < partSys->numPart; i++) {
		previous.clear();
		previous.push_back(partSys->particles[(i * 8) + 2]);
		previous.push_back(partSys->particles[(i * 8) + 3]);
		previous.push_back(partSys->particles[(i * 8) + 4]);

		partSys->particles[(i * 8) + 2] = p[(i * 3)];
		partSys->particles[(i * 8) + 3] = p[(i * 3) + 1];
		partSys->particles[(i * 8) + 4] = p[(i * 3) + 2];

		partSys->particles[(i * 8) + 5] = v[(i * 3)];
		partSys->particles[(i * 8) + 6] = v[(i * 3) + 1];
		partSys->particles[(i * 8) + 7] = v[(i * 3) + 2];
	}
	return 0;

}	// PartSimulator::step

int PartSimulator::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.");
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "link") == 0) {
		numSpring = atoi(argv[2]);
		partSys = dynamic_cast<PartSystem*>(GlobalResourceManager::use()->getSystem("partSys"));
		
		for (int i = 0; i < partSys->numPart; i++) {
			previous.push_back(partSys->particles[(i * 8) + 2] - (partSys->particles[(i * 8) + 5] * timeStep));
			previous.push_back(partSys->particles[(i * 8) + 3] - (partSys->particles[(i * 8) + 6] * timeStep));
			previous.push_back(partSys->particles[(i * 8) + 4] - (partSys->particles[(i * 8) + 7] * timeStep));
		}
		animTcl::OutputMessage("Simulator linked with %i springs", numSpring);
	}
	else if (strcmp(argv[0], "spring") == 0) {
		if (usedSprings == numSpring) {
			animTcl::OutputMessage("Number of Springs allocated already used");
		}
		else {
			double part1 = atof(argv[1]); double part2 = atof(argv[2]);
			double ks = atof(argv[3]); double kd = atof(argv[4]);
			double len = atof(argv[5]);
			springs.push_back(part1);springs.push_back(part2);springs.push_back(ks);springs.push_back(kd);
			if (len < 0) {
				len = sqrt(pow(partSys->particles[(part1 * 8) + 2] - partSys->particles[(part2 * 8) + 2], 2) +
						   pow(partSys->particles[(part1 * 8) + 3] - partSys->particles[(part2 * 8) + 3], 2) +
						   pow(partSys->particles[(part1 * 8) + 4] - partSys->particles[(part2 * 8) + 4], 2));
			}
			springs.push_back(len);
			//done
			springs.push_back(0); 
			//force
			springs.push_back(0); springs.push_back(0); springs.push_back(0);
			usedSprings++;
			animTcl::OutputMessage("Spring created between particle %s and %s with ks %s, kd %s and rest length of %s", argv[1], argv[2], argv[3], argv[4], argv[5]);
			
			partSys->addSpring(part1, part2);
		}
	}
	else if (strcmp(argv[0], "fix") == 0) {
		partSys->particles[atoi(argv[1]) * 8] = 1;
		animTcl::OutputMessage("Particle %s Fixed", argv[1]);
	}
	else if (strcmp(argv[0], "integration") == 0) {
		if (strcmp(argv[1], "euler") == 0) {
			animTcl::OutputMessage("Now using Euler Integration with Time Step %s",argv[2]);
			integration = 0;
			timeStep = atof(argv[2]);
		}
		else if (strcmp(argv[1], "symplectic") == 0) {
			animTcl::OutputMessage("Now using Symplectic Integration with Time Step %s", argv[2]);
			integration = 1;
			timeStep = atof(argv[2]);
		}
		else if (strcmp(argv[1], "verlet") == 0) {
			animTcl::OutputMessage("Now using Verlet Integration with Time Step %s", argv[2]);
			integration = 2;
			timeStep = atof(argv[2]);
		}
	}
	else if (strcmp(argv[0], "ground") == 0) {
		ksG = atof(argv[1]);
		kdG = atof(argv[2]);
		ground = true;
		animTcl::OutputMessage("Ground created with ks %s and kd %s",argv[1],argv[2]);
	}
	else if (strcmp(argv[0], "gravity") == 0) {
		gravity = atof(argv[1]);
		animTcl::OutputMessage("Set gravity to %s",argv[1]);
	}
	else if (strcmp(argv[0], "drag") == 0) {
		drag = atof(argv[1]);
		animTcl::OutputMessage("Set global drag coefficient to %s", argv[1]);
	}

	glutPostRedisplay();
	return TCL_OK;
}	// PartSimulator::command

void PartSimulator::force(int i, Vector Fi) {
	Vector Xi; Vector Vi; Vector tempV; Vector tempV2;
	setVector(Fi, 0, 0, 0); 
	setVector(Xi, partSys->particles[(i*8)+2], partSys->particles[(i * 8) + 3], partSys->particles[(i * 8) + 4]);
	setVector(Vi, partSys->particles[(i * 8) + 5], partSys->particles[(i * 8) + 6], partSys->particles[(i * 8) + 7]);

	//drag
	VecCopy(tempV, Vi);
	VecScale(tempV, -drag);
	VecAdd(Fi, Fi, tempV);

	//gravity
	Fi[1] += partSys->particles[(i * 8) + 1] * gravity;

	//springs
	Vector Vj; Vector Xj; double temp;
	for (int j = 0; j < usedSprings; j++) {
		if (springs[(j * 9)] == i) {
			if (springs[(j * 9) + 5] == 1) {
				Fi[0] -= springs[(j * 9) + 6]; Fi[1] -= springs[(j * 9) + 7]; Fi[2] -= springs[(j * 9) + 8];
			}
			else {

				setVector(Xj, partSys->particles[(springs[(j * 9) + 1] * 8) + 2], partSys->particles[(springs[(j * 9) + 1] * 8) + 3], partSys->particles[(springs[(j * 9) + 1] * 8) + 4]);
				setVector(Vj, partSys->particles[(springs[(j * 9) + 1] * 8) + 5], partSys->particles[(springs[(j * 9) + 1] * 8) + 6], partSys->particles[(springs[(j * 9) + 1] * 8) + 7]);

				//Spring Force
				VecSubtract(tempV, Xi, Xj);
				temp = springs[(j * 9) + 4] - VecLength(tempV);
				temp *= springs[(j * 9) + 2];
				VecNormalize(tempV);
				VecScale(tempV, temp);
				VecAdd(Fi, Fi, tempV);
				springs[(j * 9) + 6] += tempV[0]; springs[(j * 9) + 7] += tempV[1]; springs[(j * 9) + 8] += tempV[2];

				//Damper Force
				VecSubtract(tempV, Xi, Xj);
				VecNormalize(tempV);
				VecSubtract(tempV2, Vi, Vj);
				temp = -springs[(j * 9) + 3] * VecDotProd(tempV2, tempV);
				VecScale(tempV, temp);
				VecAdd(Fi, Fi, tempV);
				springs[(j * 9) + 6] += tempV[0]; springs[(j * 9) + 7] += tempV[1]; springs[(j * 9) + 8] += tempV[2];

				springs[(j * 9) + 5] = 1;
			}
			
		}
		else if (springs[(j * 9) + 1] == i) {
			if (springs[(j * 9) + 5] == 1) {
				Fi[0] -= springs[(j * 9) + 6]; Fi[1] -= springs[(j * 9) + 7]; Fi[2] -= springs[(j * 9) + 8];
			}
			else {
				setVector(Xj, partSys->particles[(springs[(j * 9)] * 8) + 2], partSys->particles[(springs[(j * 9)] * 8) + 3], partSys->particles[(springs[(j * 9)] * 8) + 4]);
				setVector(Vj, partSys->particles[(springs[(j * 9)] * 8) + 5], partSys->particles[(springs[(j * 9)] * 8) + 6], partSys->particles[(springs[(j * 9)] * 8) + 7]);

				//Spring Force
				VecSubtract(tempV, Xi, Xj);
				temp = springs[(j * 9) + 4] - VecLength(tempV);
				temp *= springs[(j * 9) + 2];
				VecNormalize(tempV);
				VecScale(tempV, temp);
				VecAdd(Fi, Fi, tempV);
				springs[(j * 9) + 6] += tempV[0]; springs[(j * 9) + 7] += tempV[1]; springs[(j * 9) + 8] += tempV[2];

				//Damper Force
				VecSubtract(tempV, Xi, Xj);
				VecNormalize(tempV);
				VecSubtract(tempV2, Vi, Vj);
				temp = -springs[(j * 9) + 3] * VecDotProd(tempV2, tempV);
				VecScale(tempV, temp);
				VecAdd(Fi, Fi, tempV);
				springs[(j * 9) + 6] += tempV[0]; springs[(j * 9) + 7] += tempV[1]; springs[(j * 9) + 8] += tempV[2];

				springs[(j * 9) + 5] = 1;
			}
		}
	}
}

void PartSimulator::collision(int i, Vector F) {
	if (ground) {
		Vector X = { partSys->particles[(i * 8) + 2],partSys->particles[(i * 8) + 3],partSys->particles[(i * 8) + 4] };
		Vector V = { partSys->particles[(i * 8) + 5],partSys->particles[(i * 8) + 6],partSys->particles[(i * 8) + 7] };
		Vector P = { 0,-3,0 };
		Vector N = { 0,1,0 };
		Vector temp;

		VecSubtract(temp, X, P);
		if (VecDotProd(temp, N) < 0) {
			Vector N2 = { N[0],N[1],N[2] };
			VecScale(N, -ksG * VecDotProd(temp, N));
			VecScale(N2, kdG * VecDotProd(V, N2));
			VecSubtract(temp, N, N2);
			VecAdd(F, F, temp);
		}
	}
}

void PartSimulator::display(GLenum mode) {
	if (ground) {
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_QUADS);
		glVertex3f(-100, -3, -100);
		glVertex3f(100, -3, -100);
		glVertex3f(100, -3, 100);
		glVertex3f(-100, -3, 100);
		glEnd();
	}
}