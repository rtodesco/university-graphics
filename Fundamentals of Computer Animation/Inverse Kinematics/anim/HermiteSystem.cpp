#include "HermiteSystem.h"

std::vector<double> spline;
std::vector<double> splineDx;
std::vector<double> splineDx2;
std::vector<double> U;
std::vector<double> S;
double points[100][6];

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

void HermiteSystem::display(GLenum mode) {
	double point[3];
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < spline.size();i+=3) {
		point[0] = spline[i];point[1] = spline[i + 1];point[2] = spline[i + 2];
		glVertex3dv(point);
	}
	glEnd();
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
		points[num][0] = atof(strtok(arr, " "))/2.5;
		points[num][1] = atof(strtok(NULL, " "))/2.5;
		points[num][2] = 0.1;		
		knots[num] = (double)num;
		num++;
	}
	points[0][3] = (2.0 * (points[1][0] - points[0][0])) - ((points[2][0] - points[0][0]) / 2.0);
	points[0][4] = (2.0 * (points[1][1] - points[0][1])) - ((points[2][1] - points[0][1]) / 2.0);
	points[0][5] = (2.0 * (points[1][2] - points[0][2])) - ((points[2][2] - points[0][2]) / 2.0);
	for (int i = 1; i < num - 1; i++) {
		points[i][3] = (points[i + 1][0] - points[i - 1][0]) / 2.0;
		points[i][4] = (points[i + 1][1] - points[i - 1][1]) / 2.0;
		points[i][5] = (points[i + 1][2] - points[i - 1][2]) / 2.0;
	}
	points[num - 1][3] = (2.0 * (points[num - 1][0] - points[num - 2][0])) - ((points[num - 1][0] - points[num - 3][0]) / 2.0);
	points[num - 1][4] = (2.0 * (points[num - 1][1] - points[num - 2][1])) - ((points[num - 1][1] - points[num - 3][1]) / 2.0);
	points[num - 1][5] = (2.0 * (points[num - 1][2] - points[num - 2][2])) - ((points[num - 1][2] - points[num - 3][2]) / 2.0);

	double point[3];
	double x; double y; double z;
	double dx; double dy; double dz;
	double dx2; double dy2; double dz2;
	double numer;
	double denom;
	for (int i = 0; i < num - 1; i++) {

		for (double t = knots[i]; t <= knots[i + 1]; t += 0.001) {
			numer = t - knots[i];
			denom = knots[i + 1] - knots[i];

			x = (points[i][0] * (((2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) - ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + 1.0)) +
				(points[i + 1][0] * (((-2.0 * pow(numer, 3.0)) / (pow(denom, 3.0))) + ((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))))) +
				(points[i][3] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) + ((-2.0 * pow(numer, 2.0)) / (denom)) + (numer))) +
				(points[i + 1][3] * (((pow(numer, 3.0)) / (pow(denom, 2.0))) - ((pow(numer, 2.0)) / (denom))));

			dx = (points[i][0] * (((6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) - ((6.0 * numer) / (pow(denom, 2.0))))) +
				(points[i + 1][0] * (((-6.0 * pow(numer, 2.0)) / (pow(denom, 3.0))) + ((6.0 * numer) / (pow(denom, 2.0))))) +
				(points[i][3] * (((3.0 * pow(numer, 2.0)) / (pow(denom, 2.0))) + ((-4.0 * numer) / (denom)) + 1.0)) +
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

			spline.push_back(x);spline.push_back(y);spline.push_back(z);
			splineDx.push_back(dx);splineDx.push_back(dy);splineDx.push_back(dz);
			splineDx2.push_back(dx2);splineDx2.push_back(dy2);splineDx2.push_back(dz2);
		}
	}

	glEnd();
	
	if (spline.size() > 0) { arcLengthTable(); }
}

double HermiteSystem::getArcLength(double u) {
	int i = (int)(u / 0.0001);

	if (u == 1.0) { return S[i]; }
	else { return S[i] + ((u - U[i]) / (U[i + 1] - U[i])) * (S[i + 1] - S[i]); }
}