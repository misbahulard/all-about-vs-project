// OpenGLMisbah.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <glut.h>

typedef struct {
	float x;
	float y;
} point2D_t;

typedef struct {
	float v[3];
} vector2D_t;

typedef struct {
	float m[3][3];
} matrix2D_t;

typedef struct {
	float r;
	float g;
	float b;
} color_t;

static float tick = 0;
float x2, y2;
float sudut = 0;

vector2D_t point2vector(point2D_t pnt) {
	vector2D_t vec;
	vec.v[1] = pnt.x;
	vec.v[2] = pnt.y;
	vec.v[3] = 1.;
	return vec;
}

point2D_t vector2point(vector2D_t vec) {
	point2D_t pnt;
	pnt.x = vec.v[1];
	pnt.y = vec.v[2];
	return pnt;
}

matrix2D_t createIdentity() {
	matrix2D_t mat;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.m[i][j] = 0.;
		}
		mat.m[i][i] = 1.;
	}
	return mat;
}

matrix2D_t translationMTX(float dx, float dy) {
	matrix2D_t trans = createIdentity();
	trans.m[0][2] = dx;
	trans.m[1][2] = dy;
	return trans;
}

matrix2D_t scalingMTX(float mx, float my) {
	matrix2D_t scale = createIdentity();
	scale.m[0][0] = mx;
	scale.m[1][1] = my;
	return scale;
}

matrix2D_t rotationMTX(float theta) {
	matrix2D_t rotate = createIdentity();
	float cs = cos(theta);
	float sn = sin(theta);
	rotate.m[0][0] = cs;
	rotate.m[0][1] = -sn;
	rotate.m[1][0] = sn;
	rotate.m[1][1] = cs;
	return rotate;
}

matrix2D_t operator *(matrix2D_t a, matrix2D_t b) {
	matrix2D_t c;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			c.m[i][j] = 0;
			for (int k = 0; k < 3; k++)
			{
				c.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return c;
}

vector2D_t operator *(matrix2D_t a, vector2D_t b) {
	vector2D_t c;
	for (int i = 0; i < 3; i++)
	{
		c.v[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			c.v[i] += a.m[i][j] * b.v[j];
		}
	}
	return c;
}

void drawPolygon(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<n; i++) {
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolyline(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawCircle(float r, int n) {
	point2D_t p[360];
	float a = 6.28 / n;
	for (int i = 0; i<n; i++) {
		p[i].x = r*(float)cos((float)i*a);
		p[i].y = r*(float)sin((float)i*a);
	}
	drawPolygon(p, n);
}

void drawDotCircle(float x, float tick) {
	glColor3f(1, 0, 1);
	glPointSize(10);

	float teta = (float)(tick / 57.3);
	float x2 = (float)(x*cos(teta));
	float y2 = (float)(x*sin(teta));

	glBegin(GL_POINTS);
		glVertex2f(x2, y2);
	glEnd();
}

void drawSin() {
	glColor3f(0, 0, 1);
	point2D_t p[360];

	for (int i = 0; i < 360; i++)
	{
		p[i].x = (float)i;
		p[i].y = (float)sin((float)i / 57.3);
	}

	drawPolyline(p, 360);
}

void drawDotSin(float pos) {
	glColor3f(1, 0, 0);
	glPointSize(10);

	float x = (float)pos;
	float y = (float)sin((float)pos / 57.3);
	
	if (pos == 360)
	{
		printf("here: %f", tick);
		tick = 0;
		printf("here: %f", tick);
	}

	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
}

void createSin() {
	glColor3f(0, 0, 1);
	point2D_t p[360];

	for (int i = 0; i < 360; i++)
	{
		p[i].x = (float)i;
		p[i].y = (float)sin((float)i / 57.3);
	}

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 360; i++)
	{
		glVertex2f(p[i].x, p[i].y);
	}
	glEnd();
}

void drawClock() {
	
	// Draw clock
	drawCircle(100, 360);
	drawDotCircle(100, tick);

	tick++;
}

void drawSinus() {
	// Draw Sin
	drawSin();
	drawDotSin(tick);

	tick++;
}

void userdraw() {
	//drawClock();
	drawSinus();
}

void display(void) { //clear screen 
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void initialize() {
	glClearColor(1, 1, 1, 0);
	glLoadIdentity();
	// Ortho biasa
	//gluOrtho2D(-320., 320., -240., 240.);
	// ortho sinus
	gluOrtho2D(0., 360., -1.1, 1.1);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	//Inisialisasi Toolkit 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("My Awesome Drawing");
	initialize();
	//glutIdleFunc(display);
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}


