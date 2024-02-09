#include "PartSystem.h"

std::vector<double> s;

PartSystem::PartSystem(const std::string& name) :
	BaseSystem(name)
{
	numPart = 0;
}	// PartSystem

void PartSystem::getState(double* p) {

	VecCopy(p, m_pos);

}	// PartSystem::getState

void PartSystem::setState(double* p) {

	VecCopy(m_pos, p);

}	// PartSystem::setState

int PartSystem::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "dim") == 0) {
		numPart = atof(argv[1]);
		for (int i = 0; i < numPart; i++) {
			for (int j = 0; j < 8; j++) {
				particles.push_back(0);
			}
		}
		animTcl::OutputMessage("%s Particle(s) Initialized",argv[1]);
	}
	else if (strcmp(argv[0], "particle") == 0) {
		int index = atof(argv[1]);
		for (int i = 1; i < 8; i++) {
			particles[(index * 8) + i] = atof(argv[i + 1]);
		}

		animTcl::OutputMessage("Particle %s set with mass %s, position %s %s %s and velocity %s %s %s", argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8]);
	}
	else if (strcmp(argv[0], "all_velocities") == 0) {
		for (int i = 0; i < numPart; i++) {
			particles[(i * 8) + 5] = atof(argv[1]); particles[(i * 8) + 6] = atof(argv[2]); particles[(i * 8) + 7] = atof(argv[3]);
		}
		animTcl::OutputMessage("All Velocities set to %s %s %s", argv[1],argv[2],argv[3]);
	}

	glutPostRedisplay();
	return TCL_OK;

}	// PartSystem::command

void PartSystem::display(GLenum mode) {
	double point[3];
	glPointSize(5.0);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < numPart; i++) {
		point[0] = particles[(i * 8) + 2]; point[1] = particles[(i * 8) + 3]; point[2] = particles[(i * 8) + 4];
		glBegin(GL_POINTS);
		glVertex3dv(point);
		glEnd();
	}

	double p1[3]; double p2[3];
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < s.size(); i += 2) {
		p1[0] = particles[(s[i] * 8) + 2]; p1[1] = particles[(s[i] * 8) + 3]; p1[2] = particles[(s[i] * 8) + 4];
		p2[0] = particles[(s[i + 1] * 8) + 2]; p2[1] = particles[(s[i + 1] * 8) + 3]; p2[2] = particles[(s[i + 1] * 8) + 4];
		glBegin(GL_LINES);
		glVertex3dv(p1);
		glVertex3dv(p2);
		glEnd();
	}
}	// PartSystem::display

void PartSystem::addSpring(int p1, int p2) {
	s.push_back(p1);s.push_back(p2);
}