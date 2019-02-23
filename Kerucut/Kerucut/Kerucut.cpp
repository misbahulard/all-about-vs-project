// Grafik3D2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

static float sudut = 0.0;

typedef struct {
	float x;
	float y;
} point2D_t;

typedef struct
{
	float x, y, z;
} point3D_t;

typedef struct {
	float v[3];
} vector2D_t;

typedef struct {
	float v[4];
} vector3D_t;

typedef struct {
	float m[3][3];
} matrix2D_t;

typedef struct {
	float m[3][3];
} matrix3D_t;

typedef struct {
	int numOfVertices;
	long int pnt[32];
} face_t;

typedef struct {
	float r;
	float g;
	float b;
} color_t;

typedef struct {
	int numOfVertices;
	point3D_t pnt[600];
	int numOfFaces;
	face_t fc[1200];
}object3D_t;


// Yusrl
const int MAXSIZE = 1500;
point3D_t point[MAXSIZE]; face_t face[MAXSIZE]; color_t color[MAXSIZE];
vector3D_t vec[MAXSIZE], vecbuff[MAXSIZE], NormalVector;
point2D_t titik2D[MAXSIZE];

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

point2D_t vector2point2d(vector3D_t vec)
{
	point2D_t pnt;
	pnt.x = vec.v[0];
	pnt.y = vec.v[1];
	return pnt;
}

vector3D_t point2vector(point3D_t pnt)
{
	vector3D_t vec;
	vec.v[0] = pnt.x;
	vec.v[1] = pnt.y;
	vec.v[2] = pnt.z;
	vec.v[3] = 1.;
	return vec;
}

matrix3D_t createIdentity() {
	matrix3D_t rotate;
	rotate.m[0][0] = 0.0;
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = 0.0;
	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = 0.0;
	rotate.m[1][2] = 0.0;
	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = 0.0;
	rotate.m[2][1] = 0.0;
	return rotate;
}

matrix3D_t rotationX(float teta) {
	matrix3D_t rotate = createIdentity();
	rotate.m[0][0] = 1.0;
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = 0.0;
	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = cos(teta / 57.3);
	rotate.m[1][2] = -sin(teta / 57.3);
	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = sin(teta / 57.3);
	rotate.m[2][2] = cos(teta / 57.3);
	return rotate;
}

matrix3D_t rotationY(float teta) {
	matrix3D_t rotate = createIdentity();
	rotate.m[0][0] = cos(teta / 57.3);
	rotate.m[0][1] = 0.0;
	rotate.m[0][2] = sin(teta / 57.3);
	rotate.m[1][0] = 0.0;
	rotate.m[1][1] = 1.0;
	rotate.m[1][2] = 0.0;
	rotate.m[2][0] = -sin(teta / 57.3);
	rotate.m[2][1] = 0.0;
	rotate.m[2][2] = cos(teta / 57.3);
	return rotate;
}

matrix3D_t rotationZ(float teta) {
	matrix3D_t rotate = createIdentity();
	rotate.m[0][0] = cos(teta / 57.3);
	rotate.m[0][1] = -sin(teta / 57.3);
	rotate.m[0][2] = 0.0;
	rotate.m[1][0] = sin(teta / 57.3);
	rotate.m[1][1] = cos(teta / 57.3);
	rotate.m[1][2] = 0.0;
	rotate.m[2][0] = 0.0;
	rotate.m[2][1] = 0.0;
	rotate.m[2][2] = 1.0;
	return rotate;
}

vector3D_t operator +(vector3D_t a, vector3D_t b) {
	vector3D_t c;
	for (int i = 0; i<3; i++) {
		c.v[i] = a.v[i] + b.v[i];
	}
	return c;
}

vector3D_t operator -(vector3D_t a, vector3D_t b) {
	vector3D_t c;
	for (int i = 0; i<3; i++) {
		c.v[i] = a.v[i] - b.v[i];
	}
	return c;
}

vector3D_t operator *(matrix3D_t a, vector3D_t b) {
	vector3D_t c;
	for (int i = 0; i<3; i++) {
		c.v[i] = 0;
		for (int j = 0; j<3; j++) {
			c.v[i] += a.m[i][j] * b.v[j];
		}
	}
	return c;
}

vector3D_t operator ^ (vector3D_t a, vector3D_t b)
{
	vector3D_t c; //c=a*b
	c.v[0] = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	c.v[1] = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	c.v[2] = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	//c.v[3] = 1.;
	return c;
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

matrix3D_t operator *(matrix3D_t a, matrix3D_t b)
{
	matrix3D_t c;//c=a*b
	int i, j, k;
	for (i = 0; i<3; i++) for (j = 0; j<3; j++) {
		c.m[i][j] = 0;
		for (k = 0; k<3; k++)
			c.m[i][j] += a.m[i][k] * b.m[k][j];
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
	float r = col.r / 255;
	float g = col.g / 255;
	float b = col.b / 255;

	glColor3f(r, g, b);
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

// function that returns a word from 'line' with position 'index'
// note that this is not a zero based index, first word is 1,
// second is 2 etc ..
string strWord(int index, string line)
{
	int count = 0; // number of read words
	string word; // the resulting word
	for (int i = 0; i < line.length(); i++) { // iterate over all characters in 'line'
		if (line[i] == ' ') { // if this character is a space we might be done reading a word from 'line
			if (line[i + 1] != ' ') { // next character is not a space, so we are done reading a word
				count++; // increase number of read words
				if (count == index) { // was this the word we were looking for?
					return word; // yes it was, so return it
				}
				word = ""; // nope it wasn't .. so reset word and start over with the next one in 'line'
			}
		}
		else { // not a space .. so append the character to 'word'
			word += line[i];
			if (i + 1 == line.length())
			{
				return word;
			}
		}
	}
}

object3D_t makeObjKerucut() {
	int n = 32; float r = 0.5, h = 0.9;
	int numOfVertices = n + 1; int numOfFaces = numOfVertices;
	//Point3D_t point[600];
	//int numOfFaces = n * 2;
	//face_t face[1200];
	object3D_t obj;

	//make points
	point[0] = { 0,0,h };
	for (int i = 0; i < numOfVertices; i++)
	{
		float s = i * 2 * M_PI / n;
		point[i + 1].x = r * cos(s);
		point[i + 1].y = r * sin(s);
		point[i + 1].z = 0;
	}

	//make faces
	// atas
	for (int i = 1; i < n; i++)
	{
		face[i - 1] = { 3,{ 0,i + 1,i } };
	}	
	
	face[n - 1] = { 3,{ 0,1,n } };
	
	// alas
	for (int i = 1; i <= n; i++)
	{
		face[n].pnt[i - 1] = i;
	}	
	
	face[n].numOfVertices = n; // Set numOfVertices face terakhir / alas

	//make object
	obj.numOfFaces = numOfFaces;
	for (int i = 0; i < numOfFaces; i++)
	{
		obj.fc[i] = face[i];
	}
	obj.numOfVertices = numOfVertices;
	for (int i = 0; i < numOfVertices; i++)
	{
		obj.pnt[i] = point[i];
	}

	return obj;
}


static void MakeKerucut(object3D_t &kerucut, int n, float h, float r)
{ 
	int i,j;
	float a;
	a = 6.28 / n;
	kerucut.numOfVertices = n + 1;
	kerucut.pnt[0].x = 0;
	kerucut.pnt[0].y = 0;
	kerucut.pnt[0].z = h;
	
	for (i = 0; i<n; i++)
	{
		kerucut.pnt[i].x = r*cos(a*i);
		kerucut.pnt[i].y = r*sin(a*i);
		kerucut.pnt[i].z = 0.;
	}
}

void createGrid() {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(-200.0, 0.0, 0.0);
	glVertex3f(200.0, 0.0, 0.0);
	glVertex3f(0.0, -200.0, 0.0);
	glVertex3f(0.0, 200.0, 0.0);
	glEnd();
}

void create3DObject(object3D_t object, color_t col) {
	for (int i = 0; i<object.numOfFaces; i++) {
		setColor(col);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j<object.fc[i].numOfVertices; j++) {
			int p = object.fc[i].pnt[j];
			float x = object.pnt[p].x;
			float y = object.pnt[p].y;
			glVertex3f(x, y, 0.0);
		}
		glEnd();
	}
}

void create3DObjectVisCol(object3D_t object) {
	matrix3D_t tilting = rotationX(sudut) * rotationZ(sudut);
	vector3D_t vec[MAXSIZE], vecbuff[MAXSIZE], NormalVector;
	point2D_t titik2D[MAXSIZE];
	color_t warna[MAXSIZE];
	float normalzi;
	for (int i = 0; i<object.numOfVertices; i++)
	{
		vec[i] = point2vector(object.pnt[i]);
		vec[i] = tilting*vec[i];
	}
	for (int i = 0; i<object.numOfFaces; i++)
	{
		for (int j = 0; j<object.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[object.fc[i].pnt[j]];
		NormalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = NormalVector.v[2];
		if (normalzi>0.)
		{
			//setColor(colFore);
			for (int j = 0; j < object.fc[i].numOfVertices; j++) {
				titik2D[j] = vector2point2d(vec[object.fc[i].pnt[j]]);
				warna[j] = color[object.fc[i].pnt[j]];
			}
			gradatePolygon(titik2D, object.fc[i].numOfVertices, warna);
		}
	}
}
void create3DObjectVis(object3D_t object, color_t colFore) {
	matrix3D_t tilting = rotationX(sudut);
	vector3D_t vec[MAXSIZE], vecbuff[MAXSIZE], NormalVector;
	point2D_t titik2D[MAXSIZE];
	float normalzi;
	for (int i = 0; i<object.numOfVertices; i++)
	{
		vec[i] = point2vector(object.pnt[i]);
		vec[i] = tilting*vec[i];
	}
	for (int i = 0; i<object.numOfFaces; i++)
	{
		for (int j = 0; j<object.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[object.fc[i].pnt[j]];
		NormalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = NormalVector.v[2];
		if (normalzi>0.)
		{
			setColor(colFore);
			for (int j = 0; j<object.fc[i].numOfVertices; j++)
				titik2D[j] = vector2point2d(vec[object.fc[i].pnt[j]]);
			drawPolygon(titik2D, object.fc[i].numOfVertices);
		}
	}
}


void userdraw() {
	// drawMyObject();
	//drawKubusObject3FacesNew();

	object3D_t obj = makeObjKerucut();
	matrix3D_t matrix_X = rotationX(sudut);
	matrix3D_t matrix_Y = rotationY(sudut);
	matrix3D_t matrix_Z = rotationZ(sudut);
	for (int i = 0; i<obj.numOfVertices; i++) {
		vector3D_t p;
		p.v[0] = obj.pnt[i].x;
		p.v[1] = obj.pnt[i].y;
		p.v[2] = obj.pnt[i].z;
		p = operator *(matrix_Y, p);
		p = operator *(matrix_X, p);
		p = operator *(matrix_Z, p);
		obj.pnt[i].x = p.v[0];
		obj.pnt[i].y = p.v[1];
		obj.pnt[i].z = p.v[2];
	}
	//createobj(obj, 8);
	color_t color1;
	color1.r = 255;
	color1.g = 0;
	color1.b = 0;
	//create3DObject(obj, color1);
	create3DObjectVisCol(obj);
	sudut++; if (sudut >= 360.0) sudut = 0.0;
	glFlush();

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
	//gluOrtho2D(-300., 300., -300., 300.);
	glOrtho(-1, 1, -1, 1, 1, -1);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	//Inisialisasi Toolkit 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("My Awesome Drawing");
	initialize();
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}
