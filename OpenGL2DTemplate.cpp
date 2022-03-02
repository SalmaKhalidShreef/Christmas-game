#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

using namespace std;

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
void Timer(void);
void drawTree();
void print(int x, int y, char* string);
bool gameWonOnce = false;
bool gift1Collected = false;
bool gameOver = false;
bool gameWin = false;
bool gift0Collected = false;
bool gift2Collected = false;
bool gift3Collected = false;
bool collectionSound = false;
bool backGroundOff = false;
bool fenceOver = false;
bool frontveiw = false;
bool sideview = false;
bool topview = false;
bool originalview = false;
double treeRot = 0;
double snowmanRot = 0;
double colAnimR = 0.5;
double colAnimG = 0.0;
double colAnimB = 0.1;
double gametime = 0;
double lanternRot = 0;
double fenceScaling =0.0;
double playerx = 0.0;
double playerz = 0.0;
double playerRot = 0;
double rlRot=90;
bool front = true;
bool enableSnowmanRot = false;
bool enableFenceScaling = false;
bool enableTreeRot = false;
bool enableLanternRot = false;
bool back = false;
bool rightDirec = false;
bool leftDirec = false;
bool collect = false;
void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.5f, float eyeY = 1.2f, float eyeZ = 1.5f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
	/*
	void look(double x, double y, double z, double centerx, double centery, double centerz, double upx, double upy, double upz) {
		eye.x = x;
		eye.y = y;
		eye.z = z;
		gluLookAt(x, y, z, centerx, centery, centerz, upx, upy, upz);
	}*/
};

Camera camera;

void Timer(int value) {
	if (gametime == 20000 ) {
		wchar_t* path = L"D:/guc/7/graphics/labs/OpenGL2DTemplate/gameOver.wav";
		gameOver = true;
		bool x = PlaySound(path, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	if (gift0Collected && gift2Collected&&gift3Collected &&gift1Collected && !gameWonOnce) {
		cout <<"Iwon";
		gameWin = true;
		gameWonOnce = true;
		wchar_t* path = L"D:/guc/7/graphics/labs/OpenGL2DTemplate/win.wav";
		bool x = PlaySound(path, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	if (!collectionSound && backGroundOff &&!gameWonOnce) {
		wchar_t* path = L"D:/guc/7/graphics/labs/OpenGL2DTemplate/backGround.wav";
		bool x = PlaySound(path, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		backGroundOff = false;
	}
	if (collectionSound && !gameWonOnce) {
		wchar_t* path = L"D:/guc/7/graphics/labs/OpenGL2DTemplate/JINGLES.wav";
		bool x = PlaySound(path, NULL, SND_ASYNC | SND_FILENAME);
		backGroundOff = true;

		collectionSound = false;
	}

	cout << gametime;
	if (colAnimR >=1 ) colAnimR = 0.5;
	else 	colAnimR += 0.01;
	if (colAnimG >=1 ) colAnimG = 0.0;
	else 	colAnimG += 0.1;
	if (colAnimB >= 1) colAnimB = 0.1;
	else 	colAnimB += 0.1;
	if (enableTreeRot == true) {
		treeRot += 30;
	}
	else treeRot += 0;
	if (enableSnowmanRot == true) {
		snowmanRot += 30;
	}
	else enableSnowmanRot += 0;
	if (enableLanternRot == true) {
		lanternRot += 30;
	}
	else lanternRot += 0;
	if (enableFenceScaling && !fenceOver) {
		fenceScaling +=0.05;
		if(fenceScaling>=0.5)
			fenceOver = true;

	}
	else if(enableFenceScaling){
		//fenceOver = true;
		fenceScaling -= 0.05;
		if (fenceScaling <= 0.0)
			fenceOver = false;
	}
	else fenceScaling += 0;
	gametime += 200;
	//cout << "fence '\n'" << fenceScaling;
	glutPostRedisplay();
	
	//else treeRot = 0;

	// recall the Timer function after 20 seconds (20,000 milliseconds)
	glutTimerFunc(200, Timer, 0);
}
void drawTrees() {

	glPushMatrix();
	glTranslated(0.2, 0.0, 0.3);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1, 0.0, 0.3);
	drawTree();
	glPopMatrix();
}
void drawTree() {
	glPushMatrix();
	if (enableTreeRot) {
		glTranslated(0.1, 0.1, 0.3);
		glRotatef(treeRot, 0, 1, 0);

		glTranslated(-0.1, -0.1, -0.3);
	}
	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.1, 0.1, 0.3);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.02, 0.02, 0.3, 8, 8);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0.1, 0.35, 0.3);
	glColor3f(0.3, 0.4, 0.2);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.44, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, 0.5, 0.3);
	glColor3f(0.3, 0.4, 0.2);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, 0.6, 0.3);
	glColor3f(0.3, 0.4, 0.2);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, 0.7, 0.3);
	glColor3f(0.3, 0.4, 0.2);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.08, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, 0.75, 0.35);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.18, 0.68, 0.35);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, 0.6, 0.2);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.12, 0.4, 0.2);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, 0.45, 0.38);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.07, 0.66, 0.35);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.073, 0.56, 0.35);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	
	glPushMatrix();
	glTranslated(0.22, 0.56, 0.39);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.22, 0.5, 0.35);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPopMatrix();


	

}
void drawPlayer() {
	glRotated(playerRot, 0, 1, 0);
	glTranslated(-0.5, 0.0, -0.5);
	glPushMatrix();	
	glTranslated(0.5, 0.5, 0.5);

	glPushMatrix();
	glColor3f(0.8, 0.8, 0.2);
	//glRotated(playerRot, 0, 1, 0);
	glutSolidSphere(0.06, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.03, 0.08);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();
	
	glPushMatrix();//head
	glTranslated(0.02, 0.03, 0.08);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();

	glPushMatrix();//hat
	glColor3f(1, 0.0, 0);
	glTranslated(0.0, 0.05, 0.0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.05, 0.06, 10, 10);
	glPopMatrix();
	
	glPushMatrix();//sphere over hat
	glTranslated(0.0, 0.1, 0.0);
	glColor3f(0.9, 1, 0.2);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();//body
	glColor3f(1, 0.0, 0);
	glTranslated(0.5, 0.17, 0.5);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.38, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.42, 0.37, 0.55);
	glRotated(140, 0, 1, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.1, 10, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.55, 0.37, 0.55);
	glRotated(-140, 0, 1, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.1, 10, 8);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.47, 0.1, 0.52);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.3, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.52, 0.1, 0.52);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.3, 8, 8);
	glPopMatrix();
	
	glPopMatrix();
	glPopMatrix();
}
void drawSnowMan() {
	if (enableSnowmanRot) {
		glTranslated(0.7, 0.2, 0.3);
		glRotated(snowmanRot, 0, 1, 0);
		glTranslated(-0.7,- 0.2,- 0.3);

	}
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.7, 0.2, 0.3);
	glutSolidSphere(0.1, 10, 10);
	glTranslated(0.0, 0.21, 0);
	glutSolidSphere(0.09, 10, 10);
	glTranslated(0.0, 0.14, 0);
	glutSolidSphere(0.06, 10, 10);
	
	glPushMatrix();
	glTranslated(0.0, 0.086, 0.03);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.01, 10, 10);
	glTranslated(0.04, 0.0, 0.0);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.8, 0.3, 0.1);
	glTranslated(0.7, 0.49, 0.4);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();
	
	
	
	glPushMatrix();
	glColor3f(0.8, 0.3, 0.1);
	glTranslated(0.7, 0.45, 0.4);
	glutSolidSphere(0.01, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.59, 0.45, 0.35);
	glRotated(140, 0, 1, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.1,10 , 8);
	glTranslated(0.08, -0.08, 0.22);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.82, 0.45, 0.35);
	glRotated(-140, 0, 1, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.1, 10, 8);
	glTranslated(0.1, -0.08, 0.22);
	//glRotated(playerRot, 0, 1, 0);
	glPopMatrix();

	glPopMatrix();
}
void drawLantern() {
	if (enableLanternRot) {
		glTranslated(0.5, 0.4, 0.3);
		glRotated(lanternRot, 0, 1, 0);
		glTranslated(-0.5, -0.4, -0.3);
	}

	glPushMatrix();
	glColor3f(0.6, 0.3, 0);
	glTranslated(0.5, 0.4, 0.3);
	glRotated(-90, 1, 0, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, 0.01, 0.01, 0.3, 5, 8);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.2);
	glTranslated(0.5, 0.6, 0.3);
	glutSolidSphere(0.05, 15, 15);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.9, 0.8, 0.2);
	glTranslated(0.5, 0.69, 0.3);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.2);
	glTranslated(0.5, 0.4, 0.3);
	glutSolidSphere(0.01, 15, 15);
	glPopMatrix();

}

void drawLanternsFence() {
	glPushMatrix();
	glTranslated(-1, 0.0, 0.0);
	drawLantern();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-1, 0.0, 0.2);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0.0, 0.4);
	drawLantern();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-1, 0.0, 0.6);
	drawLantern();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-1, 0.0, 0.8);
	drawLantern();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-1, 0.0, 1);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0.0, -0.2);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0.0, -0.4);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 0.0, -0.6);
	drawLantern();
	glPopMatrix();
}
void drawLanterns() {
	glPushMatrix();
	glTranslated(0.0, 0.0, 0.0);
	drawLanternsFence();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2, 0.0, 0.0);
	drawLanternsFence();
	glPopMatrix();


}
void drawWall(double thickness) {
	glPushMatrix();
	glColor3f(0.9, 0.9, 1);
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(2, thickness, 2);
	glutSolidCube(1);
	glPopMatrix();
}

void drawFenceobj(double x, double y, double z) {
	if (enableFenceScaling) {
		glTranslated(x, y, 0);
		//glRotatef(fenceScaling, 1, 0, 0);
		glTranslated(0, fenceScaling, 0);
		//glScalef(fenceScaling, 1, 1);
		glTranslated(-x, -y, 0);
	}
	glPushMatrix();
	glTranslatef(x, y, z-0.5);
	glScaled(0.1, 0.8, 0.02);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawFence() {
	

	glPushMatrix();
	glColor3f(colAnimR, colAnimG, colAnimB);
	drawFenceobj(-0.4, 0.4, 0);
	drawFenceobj(-0.2, 0.4, 0);
	drawFenceobj(0, 0.4, 0);
	drawFenceobj(0.2, 0.4, 0);
	drawFenceobj(0.4, 0.4, 0);
	drawFenceobj(0.6, 0.4, 0);
	drawFenceobj(0.8, 0.4, 0);
	drawFenceobj(1.0, 0.4, 0);
	drawFenceobj(1.2, 0.4, 0);
	drawFenceobj(1.4, 0.4, 0);
	glPopMatrix();
	glPushMatrix();
	

	glPushMatrix();
	glColor3f(colAnimR, colAnimG, colAnimB);
	glTranslated(-1, 0, 0);
	glRotated(-90, 0.0, 1.0, 0.0);
	drawFenceobj(-0.4, 0.4, 0);
	drawFenceobj(-0.2, 0.4, 0);
	drawFenceobj(0, 0.4, 0);
	drawFenceobj(0.2, 0.4, 0);
	drawFenceobj(0.4, 0.4, 0);
	drawFenceobj(0.6, 0.4, 0);
	drawFenceobj(0.8, 0.4, 0);
	drawFenceobj(1.0, 0.4, 0);
	drawFenceobj(1.2, 0.4, 0);
	drawFenceobj(1.4, 0.4, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(colAnimR, colAnimG, colAnimB);
	glTranslated(1, 0, 0);
	glRotated(-90, 0.0, 1.0, 0.0);
	drawFenceobj(-0.2, 0.4, 0);
	drawFenceobj(0, 0.4, 0);
	drawFenceobj(0, 0.4, 0);
	drawFenceobj(0.2, 0.4, 0);
	drawFenceobj(0.4, 0.4, 0);
	drawFenceobj(0.6, 0.4, 0);
	drawFenceobj(0.8, 0.4, 0);
	drawFenceobj(1.0, 0.4, 0);
	drawFenceobj(1.2, 0.4, 0);
	drawFenceobj(1.4, 0.4, 0);
	glPopMatrix();


	glPopMatrix();
}
void drawGift(double width, double height,double thick) {
	glPushMatrix();
	//glColor3f(1, 0, 0);
	glScaled(width, height, thick);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0, 1, 0);
	glScaled(0.201, 0.13, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0, 1, 0);
	glRotated(-90, 0, 1, 0);
	glScaled(0.201, 0.13, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();

	/*double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();*/
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.eye.x = 1.5f;
	camera.eye.y = 1.2f;
	camera.eye.z = 1.5f;
	camera.look();
}
void setupCamerafront() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.eye.x = 0.4;
	camera.eye.y = 1.4;
	camera.eye.z = 1.7;
	/*
	camera.center.x = 0.4;
	camera.center.x = 0.82;
	camera.center.x = 0.89;
	camera.up.x = 0;
	camera.up.y = 1;
	camera.up.z = 0;*/
	camera.look();

	//camera.look(0.4, 1.4, 1.7, 0.4, 0.82, 0.89, 0, 1, 0);

}
void setupCameraTop() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.eye.x = 0.4;
	camera.eye.y = 1.5;
	camera.eye.z = 0.4;
	
	camera.look();

}
void setupCameraSide() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.eye.x = 1.66;
	camera.eye.y = 1.27;
	camera.eye.z = 0.37;

	camera.look();
	//camera.look(1.66, 1.27, 0.37, 0.8, 0.69, 0.37, 0, 1, 0);

}
void drawGifts() {
	//0.3x 0.4z
	glPushMatrix();
	if (gift0Collected) {
		glColor3f(1, 1, 0);

	}
	else {
		glColor3f(1, 0, 0);
	}
	glTranslated(0.2, 0, 0.4);//x,y,z
	drawGift(0.2, 0.1, 0.2);
	glPopMatrix();
	
	glPushMatrix();
	if (gift1Collected) {
		glColor3f(1, 1,0);
	}
	else {
		glColor3f(1, 0, 0);
	}
	glTranslated(0.4, 0.2, 0.8);
	drawGift(0.2, 0.1, 0.2);
	glPopMatrix();
	
	glPushMatrix();
	if (gift2Collected) {
		glColor3f(1, 1, 0);
	}
	else {
		glColor3f(0.9, 0.6, 0.3);
	}
	glTranslated(0.4, 0, 0.5);//x,y,z
	drawGift(0.2, 0.1, 0.2);
	glPopMatrix();
	
	glPushMatrix();
	if (gift3Collected) {
		glColor3f(1, 1, 0);
	}
	else {
		//glColor3f(0, 0, 0);
		glColor3f(0.9, 0, 0);
	}
	glTranslated(0.1, 0, 0.55);//x,y,z
	drawGift(0.2, 0.1, 0.2);
	glPopMatrix();
}
void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (frontveiw == true)
		setupCamerafront();
	else if (sideview == true)
		setupCameraSide();
	else if (topview == true)
		setupCameraTop();
	else if (originalview == true)
		setupCamera();
	else
		setupCamera();
	if(gameOver){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glColor3f(1, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "Game Over !!!!");
		print(200, 200, (char*)p0s);
		glFlush();

	}
	else if (gameWin) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		glColor3f(1, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "You WON !!!!");
		print(200, 200, (char*)p0s);


		glFlush();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//setupLights();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		//glColor3f(1, 0, 0);
		drawSnowMan();
		glPopMatrix();
		glPushMatrix();
		drawGifts();
		glPopMatrix();
		glPushMatrix();
		glColor3f(1, 0.9, 1);
		drawWall(0.02);
		glPopMatrix();
		glPushMatrix();
		drawFence();
		glPopMatrix();
		glPushMatrix();
		drawTrees();
		glPopMatrix();
		glPushMatrix();
		drawLanterns();
		glPopMatrix();
		glPushMatrix();
		glTranslated(playerx, 0, playerz);
		//glRotated(playerRot, 0, 1, 0);
		drawPlayer();
		glPopMatrix();
		glFlush();
	}
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'p':
		if (front) {
			playerRot = 180;
			front = false;
			//cout << " p Case front '\n'";
		}
		else if (rightDirec) {
			playerRot += 90;
			rightDirec = false;
			//cout << "p Case right '\n'" << playerRot;
		}

		else if (leftDirec) {
			playerRot -= 90;
			leftDirec = false;
			//cout << "p Case left '\n'";
		}
		else {
			playerRot = 180;
			back = true;
			//cout << "p Case p '\n'";
		}
		back = true;
		playerz >= 0.1 ? playerz -= 0.1 : playerz;
		cout << "playerz \n" << playerz;
		//playerz -= 0.1;
		break;
	case 'l':
		if (back) {
			//cout << "l back case" << playerRot;
			playerRot += 180;
			back = false;
		}
		else if (rightDirec) {
			//cout << "l right case \n ";
			playerRot -= 90;
			rightDirec = false;
		}
		else if (leftDirec) {
			//cout << "l left case";
			playerRot += 90;
			leftDirec = false;
		}
		else {
			//cout << "l front case";
			front = true;
			playerRot = 0;
			playerz<=1.2?playerz += 0.1:playerz;
			cout << "playerz \n" << playerz;
			//playerz += 0.1;
		}
		break;

	case 'k':
		if (front) {
			playerRot = 90;
			//cout << "k front case '\n'";
			front = false;
		}
		else if (back) {
			playerRot -= 90;
			//cout << "k bk case'\n'";
			back = false;
		}
		else if (leftDirec) {
			//cout << "k left case '\n'";
			playerRot += 180;
			leftDirec = false;
		}
		else {
			rightDirec = true;
			//cout << "k right case '\n'";
			playerRot = 90;
		}
		playerx < 1.3 ? playerx += 0.1 : playerx;
		cout << "playerx \n" << playerx;
		//playerx += 0.1;
		break;
	case'j':
		if (front) {
			front = false;
			playerRot = -90;
		}
		else if (rightDirec) {
			rightDirec = false;
			playerRot += 180;
		}
		else if (back) {
			back = false;
			playerRot += 90;
		}
		else {
			leftDirec = true;
			playerRot = -90;
		}
		playerx >=- 0.1 ? playerx -= 0.1 : playerx;
		cout << "playerx \n" << playerx;
		//playerx -= 0.1;
		break;
	case 'n':
		if (enableTreeRot) enableTreeRot = false;
		else enableTreeRot = true;
		break;
	case 'm':
		if (enableSnowmanRot) enableSnowmanRot = false;
		else enableSnowmanRot = true;
		break;
	case 'b':
		if (enableLanternRot) enableLanternRot = false;
		else enableLanternRot = true;
		break;
	case 'v':
		if (enableFenceScaling) enableFenceScaling = false;
		else enableFenceScaling = true;
		break;
	case 'f':
		frontveiw = true;
		sideview = false;
		originalview = false;
		topview = false; break;
	case 'i':
		frontveiw = false;
		sideview = true;
		originalview = false;
		topview = false; break;
	case 't':
		frontveiw = false;
		sideview = false;
		originalview = false;
		topview = true; break;
	case 'o':
		originalview = true;
		frontveiw = false;
		sideview = false;
		topview = false; break;


	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		}
		//cout << "playerx \n" << playerx;
		cout << "playerz \n" << playerz;


		if ((playerx <= 0.39 && playerx >= 0.1) && (playerz <= 0.5 && playerz >= 0.3) &&(!gift0Collected)) {
			gift0Collected = true;
			collectionSound = true;
			cout << "gift0 \n";

			//glColor3f(1, 1, 0);
		}
		if ((playerx >= 0.2 && playerx <= 0.5) && (playerz >= 0.7 && playerz <= 0.9) && (!gift1Collected)) {
			gift1Collected = true;
			cout << "gift1 \n";
			collectionSound = true;

			//glColor3f(1, 1, 0);
		}
		if ((playerx >= 0.4&& playerx <= 0.7) && (playerz >= 0.4 && playerz <= 0.7) && (!gift2Collected)) {
			gift2Collected = true;
			collectionSound = true;

			cout << "gift2 \n";
			//glColor3f(1, 1, 0);
		}
		if ((playerx >= 0 && playerx <= 0.3) && (playerz >= 0.6 && playerz <= 0.8) && (!gift3Collected)) {
			gift3Collected = true;
			collectionSound = true;
			cout << "gift3 \n";
			//glColor3f(1, 1, 0);
		}


		glutPostRedisplay();
	}

void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	wchar_t* path= L"D:/guc/7/graphics/labs/OpenGL2DTemplate/backGround.wav";
	bool x = PlaySound(path, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	cout << x;
	
	cout << "time'\n'" << time;

	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutTimerFunc(0, Timer, 0);

	glutCreateWindow("Christmas");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	//glutIdleFunc(Anim);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
