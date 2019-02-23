// Flower.cpp : Defines the entry point for the console application.
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

void userdraw() {

	glColor3f(1, 0, 0);

	point2D_t shape[360];
	double srad, r;
	for (int s = 0; s<360; s++)
	{
		srad = s*3.14 / 180;
		r = sin(4 * srad);
		shape[s].x = (float)(r*cos(srad));
		shape[s].y = (float)(r*sin(srad));
	}
	drawPolygon(shape, 360);

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
	 //gluOrtho2D(0., 360., -1.1, 1.1);
	// Ortho bunga
	gluOrtho2D(-1., 1., -1., 1.);
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


