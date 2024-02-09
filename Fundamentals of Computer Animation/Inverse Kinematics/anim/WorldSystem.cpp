#include "WorldSystem.h"
#include "HermiteSystem.h"

//HermiteSystem* h = new HermiteSystem("hermite");
Vector root;
Vector endE;
std::vector<double> rot;
std::vector<double> leftR;

WorldSystem::WorldSystem(const std::string& name) :
	BaseSystem(name)
{
	setVector(root, 0, 0, 2.5);
	for (int i = 0; i < 7; i++) {
		rot.push_back(0);
		leftR.push_back(0);
	}
}	// SampleParticle

void WorldSystem::getState(double* p0) {

	VecCopy(p0, root);
	

}	// BaseSystem::getState

std::vector<double> WorldSystem::getRot() {
	std::vector<double> p;
	for (int i = 0; i < 7; i++) {
		p.push_back(rot[i]);
	}
	return p;
}

void WorldSystem::getEnd(Vector e) {
	VecCopy(e, endE);
}

void WorldSystem::setState(double* p0, std::vector<double> p1) {

	VecCopy(root, p0);
	for (int i = 0; i < 7; i++) {
		rot[i] = p1[i];
	}

}	// BaseSystem::setState

void WorldSystem::rest(std::vector<double> left, std::vector<double> right) {
	for (int i = 0; i < 7; i++) {
		leftR[i] = left[i];
		rot[i] = right[i];
	}
}

int WorldSystem::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "position") == 0) {
		endE[0] = atof(argv[1]);
		endE[1] = atof(argv[2]);
		endE[2] = atof(argv[3]);
	}

	glutPostRedisplay();
	return TCL_OK;

}	// ObjectPathSystem::command

void WorldSystem::display(GLenum mode) {
	
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	//wall
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-5, 5, 0);
	glVertex3f(5, 5, 0);
	glVertex3f(5, -5, 0);
	glVertex3f(-5, -5, 0);
	glEnd();
	glPopMatrix();

	//blackboard
	glPushMatrix();
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-2.5, 2, 0.01);
	glVertex3f(2.5, 2, 0.01);
	glVertex3f(2.5, -2, 0.01);
	glVertex3f(-2.5, -2, 0.01);
	glEnd();
	glPopMatrix();

	//floor
	glPushMatrix();
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-5, -5, 0);
	glVertex3f(5, -5, 0);
	glVertex3f(5, -5, 5);
	glVertex3f(-5, -5, 5);
	glEnd();
	glPopMatrix();

	//character
	glColor3f(0, 1, 0);
	glPushMatrix(); //torso
	glTranslatef(root[0],root[1],root[2]);
	glScalef(0.7, 1.5, 1);
	GLdrawCircle(1, 20);
	glScalef(1/0.7, 1/1.5, 1);

	glPushMatrix(); //upper-right leg
	glTranslatef(0.5, -2.1, 0);
	glScalef(0.3, 0.8, 1);
	GLdrawCircle(1, 20);
	glScalef(1/0.3, 1/0.8, 1);

	glPushMatrix(); //lower-right leg
	glTranslatef(0, -1.9, 0);
	glScalef(0.3, 1, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.3, 1, 1);
	
	glPushMatrix(); //right foot
	glTranslatef(0, -1, -0.2);
	glRotatef(90, 1, 0, 0);
	glScalef(0.2, 0.2, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.2, 1/0.2, 1);

	glPopMatrix(); //right foot
	glPopMatrix(); //lower-right leg
	glPopMatrix(); //upper-right leg

	glPushMatrix(); //upper-left leg
	glTranslatef(-0.5, -2.1, 0);
	glScalef(0.3, 0.8, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.3, 1 / 0.8, 1);

	glPushMatrix(); //lower-left leg
	glTranslatef(0, -1.9, 0);
	glScalef(0.3, 1, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.3, 1, 1);

	glPushMatrix(); //left foot
	glTranslatef(0, -1, -0.2);
	glRotatef(90, 1, 0, 0);
	glScalef(0.2, 0.2, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.2, 1 / 0.2, 1);

	glPopMatrix(); //left foot
	glPopMatrix(); //lower-left leg
	glPopMatrix(); //upper-left leg

	glPushMatrix(); //head
	glTranslatef(0, 2.25, 0);
	glScalef(0.7, 0.7, 1);
	GLdrawCircle(1, 20);
	glPopMatrix(); //head

	glPushMatrix(); //upper-right arm
	glTranslatef(1.5, 1, 0);
	glTranslatef(-1, 0, 0);
	glRotatef(rot[0], 1, 0, 0); glRotatef(rot[1], 0, 1, 0); glRotatef(rot[2], 0, 0, 1);
	glTranslatef(1, 0, 0);
	glScalef(0.8, 0.3, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.8, 1 / 0.3, 1);

	glPushMatrix(); //lower-right arm
	glTranslatef(1.7, 0, 0);
	glTranslatef(-1, 0, 0);
	glRotatef(rot[3], 1, 0, 0); glRotatef(rot[4], 0, 1, 0);
	glTranslatef(1, 0, 0);
	glScalef(0.8, 0.3, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.8, 1 / 0.3, 1);

	glPushMatrix(); //right hand
	glTranslatef(1.2, 0, 0);
	glTranslatef(-0.4, 0, 0);
	glRotatef(rot[5], 0, 1, 0); glRotatef(rot[6], 0, 0, 1);
	glTranslatef(0.4, 0, 0);
	glScalef(0.3, 0.3, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.3, 1 / 0.3, 1);

	glPushMatrix(); //phand (end effector)
	Vector temp = { 0.4,0,0 };
	glColor3f(1, 0, 1);
	glPointSize(7.0);
	glBegin(GL_POINTS);
	glVertex3dv(temp);
	glEnd();

	glPopMatrix(); //phand (end effector)
	glPopMatrix(); //right hand
	glPopMatrix(); //lower-right arm
	glPopMatrix(); //upper-right arm

	glColor3f(0, 1, 0);
	glPushMatrix(); //upper-left arm
	glTranslatef(-1.5, 1, 0);
	glTranslatef(1, 0, 0);
	glRotatef(leftR[0], 1, 0, 0); glRotatef(leftR[1], 0, 1, 0); glRotatef(leftR[2], 0, 0, 1);
	glTranslatef(-1, 0, 0);
	glScalef(0.8, 0.3, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.8, 1 / 0.3, 1);

	glPushMatrix(); //lower-left arm
	glTranslatef(-1.7, 0, 0);
	glTranslatef(1, 0, 0);
	glRotatef(leftR[3], 1, 0, 0); glRotatef(leftR[4], 0, 1, 0);
	glTranslatef(-1, 0, 0);
	glScalef(0.8, 0.3, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.8, 1 / 0.3, 1);

	glPushMatrix(); //left hand
	glTranslatef(-1.2, 0, 0);
	glTranslatef(0.4, 0, 0);
	glRotatef(leftR[5], 0, 1, 0); glRotatef(leftR[6], 0, 0, 1);
	glTranslatef(-0.4, 0, 0);
	glScalef(0.3, 0.3, 1);
	GLdrawCircle(1, 20);
	glScalef(1 / 0.3, 1 / 0.3, 1);

	glPopMatrix(); //left hand
	glPopMatrix(); //lower-left arm
	glPopMatrix(); //upper-left arm

	glPopMatrix(); //torso

	//h->display();
}