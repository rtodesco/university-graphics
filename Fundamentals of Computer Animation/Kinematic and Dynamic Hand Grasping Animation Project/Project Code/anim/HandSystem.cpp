#include "HandSystem.h"
double hi[57] = { -0.8,-0.1,0 , 0,0,0 , -0.2,0.2,0 , -0.5,0.5,0 , 0,0,0 , -0.05,0.3,0, -0.05,0.3,0 , 0,0.5,0 , 0,0,0 , 0,0.4,0 , 0,0.4,0 , 0.5,0.5,0 , 0,0,0 , 0,0.3,0 , 0,0.3,0 , 0.8,0.25,0, 0,0,0 , 0.05,0.25,0 , 0.05,0.25,0 };
double rot[22] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
boolean c = true;
boolean f = false;
HandSystem::HandSystem(const std::string& name) :
	BaseSystem(name)
{
	setVector(m_pos, 0, 0, 4);

	//palm
	particles.push_back(m_pos[0]); particles.push_back(m_pos[1]); particles.push_back(m_pos[2]); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(1); particles.push_back(0); //mass size type col

	//object
	particles.push_back(0); particles.push_back(-2); particles.push_back(0); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.7); particles.push_back(0); particles.push_back(0); //mass size type col

	//thumb 1
	particles.push_back(hi[0]); particles.push_back(hi[1]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(2); particles.push_back(0); //mass size type col

	//thumb 2
	particles.push_back(hi[6]); particles.push_back(hi[7]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(3); particles.push_back(0); //mass size type col

	//pointer 1
	particles.push_back(hi[9]); particles.push_back(hi[10]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(2); particles.push_back(0); //mass size type col

	//pointer 2
	particles.push_back(hi[15]); particles.push_back(hi[16]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(3); particles.push_back(0); //mass size type col

	//pointer 3
	particles.push_back(hi[18]); particles.push_back(hi[19]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(4); particles.push_back(0); //mass size type col

	//middle 1
	particles.push_back(hi[21]); particles.push_back(hi[22]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(2); particles.push_back(0); //mass size type col

	//middle 2
	particles.push_back(hi[27]); particles.push_back(hi[28]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(3); particles.push_back(0); //mass size type col

	//middle 3
	particles.push_back(hi[30]); particles.push_back(hi[31]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(4); particles.push_back(0); //mass size type col

	//ring 1
	particles.push_back(hi[33]); particles.push_back(hi[34]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(2); particles.push_back(0); //mass size type col

	//ring 2
	particles.push_back(hi[39]); particles.push_back(hi[40]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(3); particles.push_back(0); //mass size type col

	//ring 3
	particles.push_back(hi[42]); particles.push_back(hi[43]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(4); particles.push_back(0); //mass size type col

	//pinky 1
	particles.push_back(hi[45]); particles.push_back(hi[46]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(2); particles.push_back(0); //mass size type col

	//pinky 2
	particles.push_back(hi[51]); particles.push_back(hi[52]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(3); particles.push_back(0); //mass size type col

	//pinky 3
	particles.push_back(hi[54]); particles.push_back(hi[55]); particles.push_back(4); //x y z
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //vx vy vz
	particles.push_back(0); particles.push_back(0); particles.push_back(0); //Fx Fy Fz
	particles.push_back(1); particles.push_back(0.1); particles.push_back(4); particles.push_back(0); //mass size type col
}	// HandSystem

void HandSystem::getState(double* p) {

	VecCopy(p, m_pos);

}	// PartSystem::getState

void HandSystem::setState(double* p) {

	VecCopy(m_pos, p);

}	// HandSystem::setState

double* HandSystem::getRot() {
	return rot;
}

void HandSystem::setRot(double* newRot) {
	for (int i = 0; i < 22; i++) {
		rot[i] = newRot[i];
	}
}

void HandSystem::display(GLenum mode) {

	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW);

	//object
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslated(particles[13], particles[14], particles[15]);
	glScalef(particles[23], particles[23], particles[23]);
	if(f) { glutWireSphere(1.0, 20, 20); }
	else { glutSolidSphere(1.0, 20, 20); }
	
	glScalef(1/particles[23], 1/particles[23], 1/particles[23]);
	glPopMatrix();

	//ground
	glColor3f(0, 0.6, 0);
	glBegin(GL_QUADS);
	glVertex3f(-100, -3, -100);
	glVertex3f(100, -3, -100);
	glVertex3f(100, -3, 100);
	glVertex3f(-100, -3, 100);
	glEnd();
	
	//palm
	glPushMatrix();
	if(c) {glColor3f(1, 0.67, 0.58);}
	else { glColor3f(0, 0, 1); }
	glTranslated(particles[0], particles[1], particles[2]);
	glRotatef(rot[0], 1, 0, 0); glRotatef(rot[1], 0, 1, 0); glRotatef(rot[2], 0, 0, 1);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0/0.1, 1.0/0.1, 1.0/0.1);
	
	//thumb 1
	glPushMatrix();
	if (!c) { glColor3f(0, 1, 0); }
	glRotatef(rot[3], 1, 0, 0); glRotatef(rot[4], 0, 1, 0);
	glTranslated(hi[0], hi[1], hi[2]);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//thumb 2
	glPushMatrix();
	if (!c) { glColor3f(1, 0, 0); }
	glRotatef(rot[5], 1, 0, 0);
	glTranslated(-0.2,0.2,0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);
	glPopMatrix();

	glPopMatrix();

	//pointer 1
	glPushMatrix();
	if (!c) { glColor3f(0, 1, 0); }
	glRotatef(rot[6], 1, 0, 0); glRotatef(rot[7], 0, 1, 0);
	glTranslated(hi[9], hi[10], hi[11]);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//pointer 2
	glPushMatrix();
	if (!c) { glColor3f(1, 0, 0); }
	glRotatef(rot[8], 1, 0, 0);
	glTranslated(-0.05, 0.3, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//pointer 3
	glPushMatrix();
	if (!c) { glColor3f(1, 1, 0); }
	glRotatef(rot[9], 1, 0, 0);
	glTranslated(-0.05, 0.3, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//middle 1
	glPushMatrix();
	if (!c) { glColor3f(0, 1, 0); }
	glRotatef(rot[10], 1, 0, 0); glRotatef(rot[11], 0, 1, 0);
	glTranslated(hi[21], hi[22], hi[23]);
	glScalef(0.1,0.1,0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//middle 2
	glPushMatrix();
	if (!c) { glColor3f(1, 0, 0); }
	glRotatef(rot[12], 1, 0, 0);
	glTranslated(0, 0.4, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);
	
	//middle 3
	glPushMatrix();
	if (!c) { glColor3f(1, 1, 0); }
	glRotatef(rot[13], 1, 0, 0);
	glTranslated(0, 0.4, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//ring 1
	glPushMatrix();
	if (!c) { glColor3f(0, 1, 0); }
	glRotatef(rot[14], 1, 0, 0); glRotatef(rot[15], 0, 1, 0);
	glTranslated(hi[33], hi[34], hi[35]);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//ring 2
	glPushMatrix();
	if (!c) { glColor3f(1, 0, 0); }
	glRotatef(rot[16], 1, 0, 0);
	glTranslated(0, 0.3, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//ring 3
	glPushMatrix();
	if (!c) { glColor3f(1, 1, 0); }
	glRotatef(rot[17], 1, 0, 0);
	glTranslated(0, 0.3, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	//pinky 1
	glPushMatrix();
	if (!c) { glColor3f(0, 1, 0); }
	glRotatef(rot[18], 1, 0, 0); glRotatef(rot[19], 0, 1, 0);
	glTranslated(hi[45], hi[46], hi[47]);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//pinky 2
	glPushMatrix();
	if (!c) { glColor3f(1, 0, 0); }
	glRotatef(rot[20], 1, 0, 0);
	glTranslated(0.05, 0.25, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	//pinky 3
	glPushMatrix();
	if (!c) { glColor3f(1, 1, 0); }
	glRotatef(rot[21], 1, 0, 0);
	glTranslated(0.05, 0.25, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidSphere(1.0, 20, 20);
	glScalef(1.0 / 0.1, 1.0 / 0.1, 1.0 / 0.1);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
}	// HandSystem::display

int HandSystem::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[1], "position") == 0) {
		particles[13] = atof(argv[2]);
		particles[14] = atof(argv[3]);
		particles[15] = atof(argv[4]);
	}
	else if (strcmp(argv[1], "mass") == 0) {
		particles[22] = atof(argv[2]);
	}
	else if (strcmp(argv[1], "size") == 0) {
		particles[23] = atof(argv[2]);
	}
	else if (strcmp(argv[1], "colour") == 0) {
		if (c) { c = false; }
		else { c = true; }
	}
	else if (strcmp(argv[1], "frame") == 0) {
		if (f) { f = false; }
		else { f = true; }
	}


	glutPostRedisplay();
	return TCL_OK;
}