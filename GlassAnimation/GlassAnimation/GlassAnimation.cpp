// GlassAnimation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <glut.h>

static float tick = 0;
bool balik = false;

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

vector2D_t point2vector(point2D_t pnt) {
	vector2D_t vec;
	vec.v[0] = pnt.x;
	vec.v[1] = pnt.y;
	vec.v[2] = 1.;
	return vec;
}

point2D_t vector2point(vector2D_t vec) {
	point2D_t pnt;
	pnt.x = vec.v[0];
	pnt.y = vec.v[1];
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

void setColor(color_t col) {
	glColor3f(col.r, col.g, col.b);
}

void drawPolygon(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<n; i++) {
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolygon(point2D_t pnt[], int n, color_t c) {
	int i;
	setColor(c);
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

void fillPolygon(point2D_t pnt[], int n, color_t color) {
	int i;
	setColor(color);
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++)
	{
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void gradatePolygon(point2D_t pnt[], int n, color_t color[]) {
	int i;
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++)
	{
		setColor(color[i]);
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawRectangle(point2D_t p[], color_t c[]) {
	gradatePolygon(p, 4, c);
}

void drawRectangleAnimate(point2D_t p[], point2D_t trans, float tck, color_t c[]) {
	vector2D_t vec[4];
	for (int i = 0; i < 4; i++)
	{
		vec[i] = point2vector(p[i]);
	}

	matrix2D_t matRotasi = rotationMTX(tck);
	matrix2D_t matTrans = translationMTX(trans.x, trans.y);

	vec[0] = operator*(matRotasi, vec[0]);
	vec[1] = operator*(matRotasi, vec[1]);
	vec[2] = operator*(matRotasi, vec[2]);
	vec[3] = operator*(matRotasi, vec[3]);

	vec[0] = operator*(matTrans, vec[0]);
	vec[1] = operator*(matTrans, vec[1]);
	vec[2] = operator*(matTrans, vec[2]);
	vec[3] = operator*(matTrans, vec[3]);

	for (int i = 0; i < 4; i++)
	{
		p[i] = vector2point(vec[i]);
	}

	gradatePolygon(p, 4, c);
}

void userdraw() {
	color_t glassColorGrad[4] = {
		{ 0, 0.4, 0.7 },
		{ 0, 0.2, 0.5 },
		{ 0, 0.2, 0.5 },
		{ 0, 0.4, 0.7 },
	};

	// Gelas
	point2D_t p1[4] = {
		{0, 200},
		{100, 200},
		{100, 0},
		{0, 0}
	};
	drawRectangle(p1, glassColorGrad);

	// Tutup Gelas
	point2D_t p2[4] = {
		{ 0, 100 },
		{ 20, 100 },
		{ 20, 0 },
		{ 0, 0 }
	};
	drawRectangleAnimate(p2, { 100, 200 }, tick, glassColorGrad);

	// Tutup Gelas
	point2D_t p3[4] = {
		{ 20, 60 },
		{ 40, 60 },
		{ 40, 40 },
		{ 20, 40 }
	};
	drawRectangleAnimate(p3, { 100, 200 }, tick, glassColorGrad);

	if (balik)
	{
		tick -= 0.01;
		if (tick < 0)
		{
			balik = false;
		}
		else {
			balik = true;
		}
	} else {
		tick += 0.01;
		if (tick >= 1.6)
		{
			balik = true;
		}
		else {
			balik = false;
		}
	}
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
	gluOrtho2D(-300., 300., -300., 300.);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	//Inisialisasi Toolkit 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("My Awesome Glass");
	initialize();
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}
