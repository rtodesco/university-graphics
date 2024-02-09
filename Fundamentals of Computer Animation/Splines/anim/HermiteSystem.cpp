#include "HermiteSystem.h"

std::vector<double> spline;
std::vector<double> splineDx;
std::vector<double> splineDx2;
std::vector<double> U;
std::vector<double> S;

HermiteSystem::HermiteSystem(const std::string& name) :
	BaseSystem(name)
{
	num = 0;
}	// HermiteSystem

void HermiteSystem::getState(double* p) {

	assert(false);

}	// HermiteSystem::getState

vector<double> HermiteSystem::getU() {
	return U;
}

vector<double> HermiteSystem::getS() {
	return S;
}

vector<double> HermiteSystem::getSpline() {
	return spline;
}

vector<double> HermiteSystem::getSplineDx() {
	return splineDx;
}

vector<double> HermiteSystem::getSplineDx2() {
	return splineDx2;
}

void HermiteSystem::setState(double* p) {

	assert(false);

}	// HermiteSystem::setState

int HermiteSystem::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "cr") == 0) {
		animTcl::OutputMessage("Catmull-Rom Initialization");
		points[0][3] = (2.0 * (points[1][0] - points[0][0])) - ((points[2][0] - points[0][0]) / 2.0);
		points[0][4] = (2.0 * (points[1][1] - points[0][1])) - ((points[2][1] - points[0][1]) / 2.0);
		points[0][5] = (2.0 * (points[1][2] - points[0][2])) - ((points[2][2] - points[0][2]) / 2.0);
		for (int i = 1; i < num-1; i++) {
			points[i][3] = (points[i + 1][0] - points[i - 1][0]) / 2.0;
			points[i][4] = (points[i + 1][1] - points[i - 1][1]) / 2.0;
			points[i][5] = (points[i + 1][2] - points[i - 1][2]) / 2.0;
		}
		points[num-1][3] = (2.0 * (points[num-1][0] - points[num-2][0])) - ((points[num-1][0] - points[num-3][0]) / 2.0);
		points[num-1][4] = (2.0 * (points[num-1][1] - points[num-2][1])) - ((points[num-1][1] - points[num-3][1]) / 2.0);
		points[num-1][5] = (2.0 * (points[num-1][2] - points[num-2][2])) - ((points[num-1][2] - points[num-3][2]) / 2.0);
	}
	else if (strcmp(argv[1], "tangent") == 0) {
		animTcl::OutputMessage("Set tangent of point %s to [%s, %s, %s]",argv[2],argv[3],argv[4],argv[5]);
		for (int i = 3; i < 6; i++) {
			points[atoi(argv[2])][i] = atof(argv[i]);
		}
	}
	else if (strcmp(argv[0], "add") == 0) {
		animTcl::OutputMessage("Added point (%d/40)",num+1);
		animTcl::OutputMessage("	location: [%s, %s, %s]", argv[2], argv[3], argv[4]);
		animTcl::OutputMessage("	tangent: [%s, %s, %s]", argv[5], argv[6], argv[7]);
		for (int i = 0; i < 6; i++) {
			points[num][i] = atof(argv[i + 2]);
		}
		knots[num] = (double)num;
		num++;
		
	}
	else if (strcmp(argv[1], "point") == 0) {
		animTcl::OutputMessage("Set point %s to [%s, %s, %s]", argv[2], argv[3], argv[4], argv[5]);
		for (int i = 0; i < 3; i++) {
			points[atoi(argv[2])][i] = atof(argv[i+3]);
		}
	}
	else if (strcmp(argv[0], "getArcLength") == 0) {
		double u = atof(argv[1]);
		
		animTcl::OutputMessage("Arc Length at %s is: %f",argv[1],getArcLength(u));
	}
	else if (strcmp(argv[0], "load") == 0) {
		load(argv[1]);
	}
	else if (strcmp(argv[0], "export") == 0) {
		animTcl::OutputMessage("Spline exported to %s", argv[1]);
		ofstream MyFile(argv[1]);
		MyFile << "hermite " << num << "\n";

		for (int i = 0; i < num; i++) {
			MyFile << points[i][0] << " " <<
					  points[i][1] << " " <<
					  points[i][2] << " " <<
					  points[i][3] << " " <<
					  points[i][4] << " " <<
					  points[i][5] << "\n";
		}

		MyFile.close();
	}

	glutPostRedisplay();
	return TCL_OK;

}	// HermiteSystem::command

void HermiteSystem::display(GLenum mode) {
	spline.clear();
	splineDx.clear();
	splineDx2.clear();
	double point[3];
	double x; double y; double z;
	double dx; double dy; double dz;
	double dx2; double dy2; double dz2;
	double numer;
	double denom;
	
	//Draw Points
	for (int i = 0; i <= num; i++) {
		point[0] = points[i][0];point[1] = points[i][1];point[2] = points[i][2];
		glPointSize(5.0);
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		glVertex3dv(point);
		glEnd();
	}

	//Calculate and Draw Spline
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < num-1; i++) {
		for (double t = knots[i]; t <= knots[i+1]; t += 0.001) {
			numer = t - knots[i];
			denom = knots[i + 1] - knots[i];

			x = (points[i][0] * (((2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) - ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + 1.0)) +
				(points[i + 1][0] * (((-2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) + ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))))) +
				(points[i][3] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) + ((-2.0 * pow(numer, 2.0)) / (denom)) + (numer))) +
				(points[i + 1][3] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) - ((pow(numer, 2.0)) / (denom))));

			dx = (points[i][0] * (((6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) - ((6.0 * numer) / (pow(denom, 2.0))))) +
				 (points[i + 1][0] * (((-6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) + ((6.0 * numer) / (pow(denom, 2.0))))) +
				 (points[i][3] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + ((-4.0 *numer) / (denom)) + 1.0)) +
				 (points[i + 1][3] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) - ((2.0 * numer) / (denom))));

			dx2 = (points[i][0] * (((12.0 * numer) / (pow(denom, 3.0))) - (6.0 / (pow(denom, 2.0))))) +
				  (points[i + 1][0] * (((-12.0 * numer) / (pow(denom, 3.0))) + (6.0 / (pow(denom, 2.0))))) +
				  (points[i][3] * (((6.0 * numer) / (pow(denom, 2.0))) + (-4.0 / (denom)))) +
				  (points[i + 1][3] * (((6.0 * numer) / (pow(denom, 2.0))) - (2.0 / (denom))));

			y = (points[i][1] * (((2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) - ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + 1.0)) +
				(points[i + 1][1] * (((-2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) + ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))))) +
				(points[i][4] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) + ((-2.0 * pow(numer, 2.0)) / (denom)) + (numer))) +
				(points[i + 1][4] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) - ((pow(numer, 2.0)) / (denom))));

			dy = (points[i][1] * (((6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) - ((6.0 * numer) / (pow(denom, 2.0))))) +
				(points[i + 1][1] * (((-6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) + ((6.0 * numer) / (pow(denom, 2.0))))) +
				(points[i][4] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + ((-4.0 * numer) / (denom)) + 1.0)) +
				(points[i + 1][4] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) - ((2.0 * numer) / (denom))));

			dy2 = (points[i][1] * (((12.0 * numer) / (pow(denom, 3.0))) - (6.0 / (pow(denom, 2.0))))) +
				(points[i + 1][1] * (((-12.0 * numer) / (pow(denom, 3.0))) + (6.0 / (pow(denom, 2.0))))) +
				(points[i][4] * (((6.0 * numer) / (pow(denom, 2.0))) + (-4.0 / (denom)))) +
				(points[i + 1][4] * (((6.0 * numer) / (pow(denom, 2.0))) - (2.0 / (denom))));

			z = (points[i][2] * (((2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) - ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + 1.0)) +
				(points[i + 1][2] * (((-2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) + ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))))) +
				(points[i][5] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) + ((-2.0 * pow(numer, 2.0)) / (denom)) + (numer))) +
				(points[i + 1][5] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) - ((pow(numer, 2.0)) / (denom))));

			dz = (points[i][2] * (((6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) - ((6.0 * numer) / (pow(denom, 2.0))))) +
				(points[i + 1][2] * (((-6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) + ((6.0 * numer) / (pow(denom, 2.0))))) +
				(points[i][5] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + ((-4.0 * numer) / (denom)) + 1.0)) +
				(points[i + 1][5] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) - ((2.0 * numer) / (denom))));

			dz2 = (points[i][2] * (((12.0 * numer) / (pow(denom, 3.0))) - (6.0 / (pow(denom, 2.0))))) +
				(points[i + 1][2] * (((-12.0 * numer) / (pow(denom, 3.0))) + (6.0 / (pow(denom, 2.0))))) +
				(points[i][5] * (((6.0 * numer) / (pow(denom, 2.0))) + (-4.0 / (denom)))) +
				(points[i + 1][5] * (((6.0 * numer) / (pow(denom, 2.0))) - (2.0 / (denom))));

			point[0] = x;point[1] = y;point[2] = z;
			spline.push_back(x);spline.push_back(y);spline.push_back(z);
			splineDx.push_back(dx);splineDx.push_back(dy);splineDx.push_back(dz);
			splineDx2.push_back(dx2);splineDx2.push_back(dy2);splineDx2.push_back(dz2);
			glVertex3dv(point);
		}
	}
	glEnd();
	if (spline.size() > 0) { arcLengthTable(); }
}	// HermiteSystem::display

void HermiteSystem::arcLengthTable() {
	int i; double d;
	double x0 = spline[0]; double y0 = spline[1]; double z0 = spline[2]; double d0 = 0.0;
	double x; double y; double z; double ui;
	U.clear();
	S.clear();

	for (int u = 0; u <= 10000; u++) {
		ui = (double)u / 10000;
		U.push_back(ui);
		i = (int)(ui * ((spline.size() / 3.0) - 1));
		x = spline[i * 3]; y = spline[(i * 3) + 1]; z = spline[(i * 3) + 2];
			
		d = (sqrt(pow((x - x0), 2) + pow((y - y0), 2) + pow((z - z0), 2)))+d0;

		S.push_back(d);

		x0 = x; y0 = y; z0 = z; d0 = d;
	}
}

void HermiteSystem::load(char* s) {
	animTcl::OutputMessage("Loading Spline from %s",s);
	string text;

	ifstream MyReadFile(s);
	getline(MyReadFile, text);

	while (getline(MyReadFile, text)) {
		char* arr = new char[text.length() + 1];
		strcpy(arr, text.c_str());
		points[num][0] = atof(strtok(arr, " "));
		for (int i = 1; i < 6; i++) {
			points[num][i] = atof(strtok(NULL, " "));
		}
		animTcl::OutputMessage("Added point (%d/40)", num + 1);
		animTcl::OutputMessage("	location: [%f, %f, %f]", points[num][0], points[num][1], points[num][2]);
		animTcl::OutputMessage("	tangent: [%f, %f, %f]", points[num][3], points[num][4], points[num][5]);
		knots[num] = (double)num;
		num++;
	}
}

double HermiteSystem::getArcLength(double u) {
	int i = (int)(u / 0.0001);

	if (u == 1.0) { return S[i]; }
	else { return S[i] + ((u - U[i]) / (U[i + 1] - U[i])) * (S[i + 1] - S[i]); }
}