#include "ObjectPathSystem.h"
#include "HermiteSystem.h"

bool load = false;
HermiteSystem* h = new HermiteSystem("hermite");
Vector fO; Vector uO; Vector uOld;

ObjectPathSystem::ObjectPathSystem(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{
	
	m_model.ReadOBJ("data/porsche.obj");


}	// SampleParticle

void ObjectPathSystem::getState(double* p) {

	VecCopy(p, m_pos);

}	// BaseSystem::getState


void ObjectPathSystem::setState(double* p) {

	VecCopy(m_pos, p);

}	// BaseSystem::setState

HermiteSystem* ObjectPathSystem::getSplineSystem() {
	return h;
}

void ObjectPathSystem::setRot(Vector f, Vector u) {
	VecCopy(fO, f);
	VecCopy(uO, u);
}

void ObjectPathSystem::reset(double time) {

	setVector(m_pos, h->points[0][0], h->points[0][1], h->points[0][2]);

}	// ObjectPathSystem::Reset

int ObjectPathSystem::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "load") == 0) {
		h->load(argv[1]);
		load = true;
		m_pos[0] = h->points[0][0];m_pos[1] = h->points[0][1];m_pos[2] = h->points[0][2];
	}

	glutPostRedisplay();
	return TCL_OK;

}	// ObjectPathSystem::command

void ObjectPathSystem::display(GLenum mode) {
	glm::vec3 pos(m_pos[0], m_pos[1], m_pos[2]);
	glm::vec3 scale(0.02, 0.02, 0.02);
	glm::vec3 forward(fO[0], fO[1], fO[2]);
	if (VecDotProd(uOld, uO) < 0) {
		VecCopy(uO, uOld);
	}
	glm::vec3 up(uO[0], uO[1], uO[2]);
	glm::quat rot = glm::quatLookAt(forward, up);
	h->display();
	if (load) {
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, pos);
		transform = glm::scale(transform, scale);
		glm::quat rotFix = glm::quat(glm::vec3(3.14, 0.0, 0.0));
		transform *= glm::toMat4(rot);
		transform *= glm::toMat4(rotFix);
		glMultMatrixf(&transform[0][0]);
		
		glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);

		glPopMatrix();
		glPopAttrib();
	}
	VecCopy(uOld, uO);
}