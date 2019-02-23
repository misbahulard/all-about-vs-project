// Grafik3D1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <glut.h>

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
	int numOfVertices;
	point3D_t pnt[100];
	int numOfFaces;
	face_t fc[32];
} object3D_t;

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

//matrix2D_t createIdentity() {
//	matrix2D_t mat;
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			mat.m[i][j] = 0.;
//		}
//		mat.m[i][i] = 1.;
//	}
//	return mat;
//}

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

//
//matrix2D_t translationMTX(float dx, float dy) {
//	matrix2D_t trans = createIdentity();
//	trans.m[0][2] = dx;
//	trans.m[1][2] = dy;
//	return trans;
//}
//
//matrix2D_t scalingMTX(float mx, float my) {
//	matrix2D_t scale = createIdentity();
//	scale.m[0][0] = mx;
//	scale.m[1][1] = my;
//	return scale;
//}
//
//matrix2D_t rotationMTX(float theta) {
//	matrix2D_t rotate = createIdentity();
//	float cs = cos(theta);
//	float sn = sin(theta);
//	rotate.m[0][0] = cs;
//	rotate.m[0][1] = -sn;
//	rotate.m[1][0] = sn;
//	rotate.m[1][1] = cs;
//	return rotate;
//}

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

void create3DObjectNew(object3D_t object, color_t col) {
	
	vector3D_t normalVector, vec[100], vecbuff[100];
	point2D_t titik2d[100];
	float normalzi = 0.;

	setColor({ 0, 0, 1 });
	for (int i = 0; i<object.numOfFaces; i++)
	{
		for (int j = 0; j<object.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[object.fc[i].pnt[j]];
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j<object.fc[i].numOfVertices; j++)
				titik2d[j] = vector2point2d(vec[object.fc[i].pnt[j]]);
			drawPolygon(titik2d, object.fc[i].numOfVertices);
		}
	}

	setColor({ 0, 1, 1 });
	for (int i = 0; i<object.numOfFaces; i++)
	{
		for (int j = 0; j<object.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[object.fc[i].pnt[j]];
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j<object.fc[i].numOfVertices; j++)
				titik2d[j] = vector2point2d(vec[object.fc[i].pnt[j]]);
			drawPolygon(titik2d, object.fc[i].numOfVertices);
		}
	}
}

void createKubus(vector3D_t kubus[], int n) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	float z1 = 0.0;
	float z2 = 0.0;

	glVertex3f(kubus[0].v[0], kubus[0].v[1], z1); // t 0
	glVertex3f(kubus[1].v[0], kubus[1].v[1], z2); // t 1
	glVertex3f(kubus[1].v[0], kubus[1].v[1], z1); // t 
	glVertex3f(kubus[2].v[0], kubus[2].v[1], z2); // t 2

	glVertex3f(kubus[2].v[0], kubus[2].v[1], z1); // t 2
	glVertex3f(kubus[3].v[0], kubus[3].v[1], z2); // t 3
	glVertex3f(kubus[3].v[0], kubus[3].v[1], z1); // t
	glVertex3f(kubus[0].v[0], kubus[0].v[1], z2); // t 0
	
	glVertex3f(kubus[4].v[0], kubus[4].v[1], z1);
	glVertex3f(kubus[5].v[0], kubus[5].v[1], z2);
	glVertex3f(kubus[5].v[0], kubus[5].v[1], z1);
	glVertex3f(kubus[6].v[0], kubus[6].v[1], z2);
	
	glVertex3f(kubus[6].v[0], kubus[6].v[1], z1);
	glVertex3f(kubus[7].v[0], kubus[7].v[1], z2);
	glVertex3f(kubus[7].v[0], kubus[7].v[1], z1);
	glVertex3f(kubus[4].v[0], kubus[4].v[1], z2);
	
	glVertex3f(kubus[0].v[0], kubus[0].v[1], z1);
	glVertex3f(kubus[4].v[0], kubus[4].v[1], z2);
	glVertex3f(kubus[1].v[0], kubus[1].v[1], z1);
	glVertex3f(kubus[5].v[0], kubus[5].v[1], z2);
	
	glVertex3f(kubus[2].v[0], kubus[2].v[1], z1);
	glVertex3f(kubus[6].v[0], kubus[6].v[1], z2);
	glVertex3f(kubus[3].v[0], kubus[3].v[1], z1);
	glVertex3f(kubus[7].v[0], kubus[7].v[1], z2);
	glEnd();
}

void drawKubus() {
	int n = 8;
	vector3D_t kubus[8] = {
		{ -50.0,50.0,50.0 },
		{ 50.0,50.0,50.0 },
		{ 50.0,-50.0,50.0 },
		{ -50.0,-50.0,50.0 },
		{ -50.0,50.0,-50.0 },
		{ 50.0,50.0,-50.0 },
		{ 50.0,-50.0,-50.0 },
		{ -50.0,-50.0,-50.0 }
	};

	matrix3D_t matrix_X = rotationX(sudut);
	matrix3D_t matrix_Y = rotationY(sudut);
	matrix3D_t matrix_Z = rotationZ(sudut);
	for (int i = 0; i<n; i++) {
		kubus[i] = operator *(matrix_X, kubus[i]);
		kubus[i] = operator *(matrix_Y, kubus[i]);
		kubus[i] = operator *(matrix_Z, kubus[i]);
	}

	createGrid();
	createKubus(kubus, 8);

	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();
}

void drawLimas() {
	object3D_t prisma1 = {
		5,
		{ 
			{ 0,100,0 },
			{ 100,0,0 },
			{ 0,0,100 },
			{ -100,0,0 },
			{ 0,0,-100 } 
		},
		6,
		{
			{ 3,{ 0,2,1 } },
			{ 3,{ 0,3,2 } },
			{ 3,{ 0,4,3 } },
			{ 3,{ 0,1,4 } },
			{ 3,{ 1,2,4 } },
			{ 3,{ 3,4,2 } },
		} 
	};

	// Draw Limas
	matrix3D_t matrix_X = rotationX(sudut);
	matrix3D_t matrix_Y = rotationY(sudut);
	matrix3D_t matrix_Z = rotationZ(sudut);
	for (int i = 0; i<prisma1.numOfVertices; i++) {
		//teta=15.0;
		vector3D_t p;
		p.v[0] = prisma1.pnt[i].x;
		p.v[1] = prisma1.pnt[i].y;
		p.v[2] = prisma1.pnt[i].z;
		p = (matrix_Y)*(p); //p=operator *(matrix_Y,p);
		p = (matrix_X)*(p); //p=operator *(matrix_X,p);
		p = (matrix_Z)*(p); //p=operator *(matrix_Z,p);
		prisma1.pnt[i].x = p.v[0];
		prisma1.pnt[i].y = p.v[1];
		prisma1.pnt[i].z = p.v[2];
	}
	color_t color1;
	color1.r = 1.0;
	color1.g = 0.0;
	color1.b = 1.0;
	create3DObject(prisma1, color1);
	sudut++; 
	if (sudut >= 360.0) 
		sudut = 0.0;
	glFlush();
}

void drawKubusObject() 
{
	object3D_t kubus = {
		8,
		{
			{ -50.0, 50.0, 50.0 },
			{ 50.0, 50.0, 50.0 },
			{ 50.0, -50.0, 50.0 },
			{ -50.0, -50.0, 50.0 },
			{ -50.0, 50.0, -50.0 },
			{ 50.0, 50.0, -50.0 },
			{ 50.0, -50.0, -50.0 },
			{ -50.0, -50.0, -50.0 }
		},
		6,
		{
			{ 4, { 0, 3, 2, 1 } },
			{ 4, { 1, 2, 6, 5 } },
			{ 4, { 0, 1, 5, 4 } },
			{ 4, { 7, 6, 2, 3 } },
			{ 4, { 0, 4, 7, 3 } },
			{ 4, { 4, 5, 6, 7 } },
		}
	};

	// Draw Grid
	createGrid();
	// Draw Limas
	matrix3D_t matrix_X = rotationX(sudut);
	matrix3D_t matrix_Y = rotationY(sudut);
	matrix3D_t matrix_Z = rotationZ(sudut);

	for (int i = 0; i<kubus.numOfVertices; i++) {
		vector3D_t p;
		p.v[0] = kubus.pnt[i].x;
		p.v[1] = kubus.pnt[i].y;
		p.v[2] = kubus.pnt[i].z;
		p = (matrix_Y)*(p); //p=operator *(matrix_Y,p);
		p = (matrix_X)*(p); //p=operator *(matrix_X,p);
		p = (matrix_Z)*(p); //p=operator *(matrix_Z,p);
		kubus.pnt[i].x = p.v[0];
		kubus.pnt[i].y = p.v[1];
		kubus.pnt[i].z = p.v[2];
	}
	color_t color1;
	color1.r = 1.0;
	color1.g = 0.0;
	color1.b = 1.0;
	create3DObject(kubus, color1);
	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();
}

void drawKubusObject3Faces()
{
	object3D_t kubus = {
		8,
		{
			{ -50.0, 50.0, 50.0 },
			{ 50.0, 50.0, 50.0 },
			{ 50.0, -50.0, 50.0 },
			{ -50.0, -50.0, 50.0 },
			{ -50.0, 50.0, -50.0 },
			{ 50.0, 50.0, -50.0 },
			{ 50.0, -50.0, -50.0 },
			{ -50.0, -50.0, -50.0 }
		},
		12,
		{
			{ 3, { 0, 3, 2 } },
			{ 3, { 0, 2, 1 } },
			{ 3, { 1, 2, 6 } },
			{ 3, { 1, 6, 5} },
			{ 3, { 0, 1, 5 } },
			{ 3, { 0, 5, 4} },
			{ 3, { 7, 6, 2 } },
			{ 3, { 7, 2, 3} },
			{ 3, { 0, 4, 7 } },
			{ 3, { 0, 7, 3 } },
			{ 3, { 4, 5, 6 } },
			{ 3, { 4, 6, 7 } }
		}
	};

	// Draw Grid
	createGrid();
	// Draw Limas
	matrix3D_t matrix_X = rotationX(sudut);
	matrix3D_t matrix_Y = rotationY(sudut);
	matrix3D_t matrix_Z = rotationZ(sudut);

	for (int i = 0; i<kubus.numOfVertices; i++) {
		vector3D_t p;
		p.v[0] = kubus.pnt[i].x;
		p.v[1] = kubus.pnt[i].y;
		p.v[2] = kubus.pnt[i].z;
		p = (matrix_Y)*(p); //p=operator *(matrix_Y,p);
		p = (matrix_X)*(p); //p=operator *(matrix_X,p);
		p = (matrix_Z)*(p); //p=operator *(matrix_Z,p);
		kubus.pnt[i].x = p.v[0];
		kubus.pnt[i].y = p.v[1];
		kubus.pnt[i].z = p.v[2];
	}
	color_t color1;
	color1.r = 1.0;
	color1.g = 0.0;
	color1.b = 1.0;
	create3DObjectNew(kubus, color1);
	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();
}

void drawKubusObject3FacesNew()
{
	object3D_t kubus = {
		8,
		{
			{ -50.0, 50.0, 50.0 },
			{ 50.0, 50.0, 50.0 },
			{ 50.0, -50.0, 50.0 },
			{ -50.0, -50.0, 50.0 },
			{ -50.0, 50.0, -50.0 },
			{ 50.0, 50.0, -50.0 },
			{ 50.0, -50.0, -50.0 },
			{ -50.0, -50.0, -50.0 }
		},
		12,
		{
			{ 3,{ 0, 3, 2 } },
			{ 3,{ 0, 2, 1 } },
			{ 3,{ 1, 2, 6 } },
			{ 3,{ 1, 6, 5 } },
			{ 3,{ 0, 1, 5 } },
			{ 3,{ 0, 5, 4 } },
			{ 3,{ 7, 6, 2 } },
			{ 3,{ 7, 2, 3 } },
			{ 3,{ 0, 4, 7 } },
			{ 3,{ 0, 7, 3 } },
			{ 3,{ 4, 5, 6 } },
			{ 3,{ 4, 6, 7 } }
		}
	};

	color_t myColor[12] = {
		{1, 0, 0}, {1, 0, 0},
		{0, 1, 0}, { 0, 1, 0 },
		{ 0, 0, 1 },{ 0, 0, 1 },
		{ 1, 0, 0 },{ 1, 0, 0 },
		{ 0, 1, 0 },{ 0, 1, 0 },
		{ 0, 0, 1 },{ 0, 0, 1 },
	};

	vector3D_t normalVector, vec[100], vecbuff[100];
	point2D_t titik2d[100];
	float normalzi = 0.;

	// Draw Grid
	createGrid();
	matrix3D_t tilting = rotationX(sudut) * rotationY(-sudut);

	for (int i = 0; i<kubus.numOfVertices; i++)
	{
		vec[i] = point2vector(kubus.pnt[i]);
		vec[i] = tilting * vec[i];
	}

	setColor({ 0, 1, 0 });
	for (int i = 0; i<kubus.numOfFaces; i++)
	{
		for (int j = 0; j<kubus.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[kubus.fc[i].pnt[j]];
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j<kubus.fc[i].numOfVertices; j++)
				titik2d[j] = vector2point2d(vec[kubus.fc[i].pnt[j]]);
			drawPolygon(titik2d, kubus.fc[i].numOfVertices);
			//fillPolygon(titik2d, kubus.fc[i].numOfVertices, { 1, 1, 0 });
		}
	}

	setColor({ 0, 0, 1 });
	for (int i = 0; i<kubus.numOfFaces; i++)
	{
		for (int j = 0; j<kubus.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[kubus.fc[i].pnt[j]];
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j<kubus.fc[i].numOfVertices; j++)
				titik2d[j] = vector2point2d(vec[kubus.fc[i].pnt[j]]);
			drawPolygon(titik2d, kubus.fc[i].numOfVertices);
			//fillPolygon(titik2d, kubus.fc[i].numOfVertices, { 1, 1, 0 });
		}
	}
	
	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();
}

void drawLimasNew()
{
	object3D_t prisma = {
		5,
		{
			{ 0,100,0 },
			{ 100,0,0 },
			{ 0,0,100 },
			{ -100,0,0 },
			{ 0,0,-100 }
		},
		6,
		{
			{ 3,{ 0,2,1 } },
			{ 3,{ 0,3,2 } },
			{ 3,{ 0,4,3 } },
			{ 3,{ 0,1,4 } },
			{ 3,{ 1,2,4 } },
			{ 3,{ 3,4,2 } },
		}
	};

	color_t myColor[12] = {
		{ 1, 0, 0 },{ 1, 0, 0 },
		{ 0, 1, 0 },{ 0, 1, 0 },
		{ 0, 0, 1 },{ 0, 0, 1 },
		{ 1, 0, 0 },{ 1, 0, 0 },
		{ 0, 1, 0 },{ 0, 1, 0 },
		{ 0, 0, 1 },{ 0, 0, 1 },
	};

	vector3D_t normalVector, vec[100], vecbuff[100];
	point2D_t titik2d[100];
	float normalzi = 0.;

	// Draw Grid
	createGrid();
	matrix3D_t tilting = rotationX(sudut) * rotationY(-sudut);

	for (int i = 0; i<prisma.numOfVertices; i++)
	{
		vec[i] = point2vector(prisma.pnt[i]);
		vec[i] = tilting * vec[i];
	}

	setColor({ 0, 1, 0 });
	for (int i = 0; i<prisma.numOfFaces; i++)
	{
		for (int j = 0; j<prisma.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[prisma.fc[i].pnt[j]];
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j<prisma.fc[i].numOfVertices; j++)
				titik2d[j] = vector2point2d(vec[prisma.fc[i].pnt[j]]);
			drawPolygon(titik2d, prisma.fc[i].numOfVertices);
			//fillPolygon(titik2d, kubus.fc[i].numOfVertices, { 1, 1, 0 });
		}
	}

	setColor({ 0, 0, 1 });
	for (int i = 0; i<prisma.numOfFaces; i++)
	{
		for (int j = 0; j<prisma.fc[i].numOfVertices; j++)
			vecbuff[j] = vec[prisma.fc[i].pnt[j]];
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j<prisma.fc[i].numOfVertices; j++)
				titik2d[j] = vector2point2d(vec[prisma.fc[i].pnt[j]]);
			drawPolygon(titik2d, prisma.fc[i].numOfVertices);
			//fillPolygon(titik2d, kubus.fc[i].numOfVertices, { 1, 1, 0 });
		}
	}

	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();
}

void userdraw() {
	drawKubusObject3FacesNew();
	//drawLimasNew();
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
	glOrtho(-200, 200, -200, 200, 200, -200);
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
