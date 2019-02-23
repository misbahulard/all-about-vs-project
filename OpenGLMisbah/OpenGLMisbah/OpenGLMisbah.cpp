// OpenGLMisbah.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <glut.h>

void dotdot() {
	int s;
	float x, y;
	for (int i = 0; i < 100; i++)
	{
		s = rand() % 4 + 1;
		glPointSize(s);
		glBegin(GL_POINTS);
		x = -200 + 400 * (float)rand() / RAND_MAX;
		y = -300 + 600 * (float)rand() / RAND_MAX;
		glVertex2f(x, y);
		glEnd();
	}
}

void rain() {
	int s = 2;
	float x, y;
	for (int i = 0; i < 100; i++)
	{
		glPointSize(s);
		x = 640 * (float)rand() / RAND_MAX;
		y = 480 * (float)rand() / RAND_MAX;
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x - 8, y - 8);
		glEnd();
	}
}

void drawDot(float x, float y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void grid() {
	glColor3f(0.5, 0, 1);
	for (float x = -320; x < 320; x += 20)
	{

		glBegin(GL_LINES);
		drawLine(-320, x, 320, x);
		drawLine(x, 240, x, -240);
	}
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	drawLine(-320, 0, 320, 0);
	drawLine(0, 240, 0, -240);
}

void star() {
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 60);
	glVertex2f(20, 20);
	glVertex2f(60, 20);
	glVertex2f(35, -10);
	glVertex2f(50, -60);
	glVertex2f(0, -30);
	glVertex2f(-50, -60);
	glVertex2f(-35, -10);
	glVertex2f(-60, 20);
	glVertex2f(-20, 20);
	glEnd();
}

void kotak() {
	glColor3f(1, 0, 1);
	glBegin(GL_QUADS);
	glVertex2f(60, -60);
	glVertex2f(100, -60);
	glVertex2f(100, -100);
	glVertex2f(60, -100);
	glEnd();
}

//void star() {
//	glColor3f(0, 0, 0);
//	glBegin(GL_LINE_LOOP);
//	glVertex2f(0, 60);
//	glVertex2f(60, -60);
//	glVertex2f(0, -20);
//	glVertex2f(-60, -60);
//	glEnd();
//	glBegin(GL_LINE_LOOP);
//	glVertex2f(-60, 20);
//	glVertex2f(60, 20);
//	glVertex2f(0, -40);
//	glEnd();
//}

void triangle_line() {
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-120, 140);
	glVertex2f(-80, 80);
	glVertex2f(-160, 80);
	glEnd();
}

void triangle() {
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-120, 140);
	glVertex2f(-80, 80);
	glVertex2f(-160, 80);
	glVertex2f(-220, 240);
	glVertex2f(-180, 180);
	glVertex2f(-260, 180);
	glEnd();
}

void userdraw() {
	//glPointSize(5);
	glColor3f(0, 0, 0);
	//grid();
	//Star
	star();
	triangle();
	kotak();
}

void display(void) { //clear screen 
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

void initialize() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320., 320., -240., 240.); 
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


