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
	float r;
	float g;
	float b;
} color_t;

void setColor(color_t col) {
	glColor3f(col.r, col.g, col.b);
}

void drawPolyline(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < n; i++)
	{
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolygon(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < n; i++)
	{
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

void gradatePolygon(point2D_t pnt[], int n, color_t color) {
	int i;
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++)
	{
		setColor(color);
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawCircle(float r, int n) {
	point2D_t p[360];
	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = r * (float)cos((float)i * a);
		p[i].y = r * (float)sin((float)i * a);
	}

	drawPolygon(p, n);
}

void drawCircle(point2D_t p[], point2D_t p0, float r, int n) {
	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = p0.x + r*cos(i*a);
		p[i].y = p0.y + r*sin(i*a);
	}

	drawPolygon(p, n);
}

void drawEclipse(point2D_t p[], float r1, float r2, int n) {
	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = r1*cos(i*a);
		p[i].y = r2*sin(i*a);
	}

	drawPolygon(p, n);
}

void drawEclipse(point2D_t p[], point2D_t p0, float r1, float r2, int n) {
	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = p0.x + r1*cos(i*a);
		p[i].y = p0.y + r2*sin(i*a);
	}

	drawPolygon(p, n);
}

void centerPolygon(point2D_t p[], point2D_t pc, color_t col, color_t colp, int n) {
	for (int i = 0; i < n; i++)
	{
		glBegin(GL_LINES);
		setColor(colp);
		glVertex2f(pc.x, pc.y);
		setColor(col);
		glVertex2f(p[i].x, p[i].y);
		glEnd();
	}
}

void userdraw() {
	point2D_t kotak[4] = {
		{100, 100},
		{300, 100},
		{300, 200},
		{100, 200}
	};

	point2D_t bintang[10] = {
		{80, 146},
		{99, 90},
		{157, 90},
		{110, 55},
		{128, 1},
		{80, 34},
		{32, 1},
		{54, 55},
		{3, 90},
		{63, 90}
	};

	color_t col = { 0, 0, 1 };

	glColor3f(0, 0, 0);
	//fillPolygon(kotak, 4, col);

	//drawPolygon(bintang, 10);

	glColor3f(1, 0, 0);
	point2D_t shape[360];
	double srad, r;
	for (int s = 0; s < 360; s++)
	{
		srad = s*3.14 / 180;
		r = sin(5 * srad);
		shape[s].x = (float)(r*cos(srad));
		shape[s].y = (float)(r*sin(srad));
	}
	
	//drawPolyline(shape, 360);

	point2D_t p[360];
	for (int i = 0; i < 360; i++)
	{
		p[i].x = (float)i;
		p[i].y = (float)sin((float)i / 57.3);
	}

	// bunga / sin
	drawPolyline(p, 360);

	// Draw Circle
	//drawCircle(100, 100);

	point2D_t x[360];
	point2D_t titik = { -200, 100 };
	//drawCircle(x, titik, 60, 40);

	// draw eclipse
	point2D_t eclipse[360];
	point2D_t titikeclipse = {-150, -100};
	//drawEclipse(eclipse, -40, -60, 100);
	//drawEclipse(eclipse, titikeclipse, -40, -60, 100);

	// center polygon
	point2D_t p2[360];
	point2D_t pusat = { 0, 0 };
	color_t putih = { 1, 1, 1 };
	color_t biru = { 0,0,1 };
	//drawCircle(p2, pusat, 50, 360);
	//centerPolygon(p2, pusat, biru, putih, 360);

	// Animasi
	static int tick = 0;
	point2D_t shape2[360];
	double srad2, r2;
	for (int s = 0; s < 360; s++)
	{
		srad2 = (s + tick)*3.14 / 180;
		r2 = sin(5 * srad2);
		shape2[s].x = (float)(r2*cos(srad2));
		shape2[s].y = (float)(r2*sin(srad2));
	}
	//drawPolygon(shape2, 360);
	tick++;
}

void display(void) { //clear screen 
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

void initialize() {
	glClearColor(1, 1, 1, 0);
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Ortho biasa
	//gluOrtho2D(-320., 320., -240., 240.);
	// Ortho sin
	gluOrtho2D(0., 360., -1.1, 1.1);
	// Ortho bunga
	// gluOrtho2D(-1., 1., -1., 1.);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	//Inisialisasi Toolkit 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("My Awesome Drawing");
	initialize();
	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}


