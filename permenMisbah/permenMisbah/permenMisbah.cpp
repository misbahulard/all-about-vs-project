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
	int numOfColors;
	color_t col[10000];
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

void readDataset(string filename, object3D_t &fileObj) {
	// File to read
	ifstream file(filename);
	string content;
	string result;

	int maxVertices = 0;
	int maxFaces = 0;

	// Loop until reach End of File
	int index = 1;
	while (getline(file, content))
	{
		// Get number of Vertices and Faces
		if (index == 2)
		{
			result = strWord(1, content);
			fileObj.numOfVertices = stoi(result);
			result = strWord(2, content);
			fileObj.numOfFaces = stoi(result);

			// Set for next looping
			maxVertices = 3 + fileObj.numOfVertices;
			maxFaces = maxVertices + fileObj.numOfFaces;
		}
		else if (index >= 3 && index < maxVertices)
		{
			// titik x y z
			float titik[3];
			for (int i = 1; i <= 3; i++)
			{
				result = strWord(i, content);
				titik[i - 1] = strtof(result.c_str(), 0);
			}

			// save point vertices
			point3D_t pnt = { titik[0], titik[1], titik[2] };
			fileObj.pnt[index - 3] = pnt;

			// save color
			float warna[3];
			for (int i = 4; i <= 6; i++)
			{
				result = strWord(i, content);
				warna[i - 4] = strtof(result.c_str(), 0);
			}
			color_t col = { warna[0], warna[1], warna[2] };
			fileObj.col[index - 3] = col;
		}
		else if (index >= maxVertices && index < maxFaces)
		{
			// num of point | titik pembentuk faces
			float titik2[10];
			float jumlahTitik = 0.0;

			// ambil jumlah titik pembentuk face
			result = strWord(1, content);
			jumlahTitik = strtof(result.c_str(), 0);

			for (int i = 1; i <= jumlahTitik + 1; i++)
			{
				result = strWord(i, content);
				titik2[i - 1] = strtof(result.c_str(), 0);
			}
			// set jumlah tiitk pembuat face
			fileObj.fc[index - 3 - fileObj.numOfVertices].numOfVertices = titik2[0];

			// Set titik pembentuk faces
			for (int i = 1; i < jumlahTitik + 1; i++)
			{
				fileObj.fc[index - 3 - fileObj.numOfVertices].pnt[i - 1] = titik2[i];
			}
		}

		index = index + 1;
	}
}

void writeObjectToFile(string filePath, object3D_t object) {
	ofstream myfile(filePath);
	float roundDigit = 100;
	if (myfile.is_open())
	{
		myfile << "COFF\n";
		myfile << object.numOfVertices << " "
			<< object.numOfFaces << " "
			<< 0 << endl;
		for (int i = 0; i < object.numOfVertices; i++)
		{
			myfile << roundf(object.pnt[i].x * roundDigit) / roundDigit << " "
				<< roundf(object.pnt[i].y * roundDigit) / roundDigit << " "
				<< roundf(object.pnt[i].z * roundDigit) / roundDigit << " ";
			myfile << object.col[i].r << " "
				<< object.col[i].g << " "
				<< object.col[i].b << " ";
			myfile << 255 << endl;
		}
		for (int i = 0; i < object.numOfFaces; i++)
		{
			myfile << object.fc[i].numOfVertices;
			for (int j = 0; j < object.fc[i].numOfVertices; j++)
			{
				myfile << " " << object.fc[i].pnt[j];
			}
			if (i != object.numOfFaces - 1)
			{
				myfile << endl;
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}

void writeMultipleObjectToFile(string filePath, object3D_t &object, object3D_t &object2, object3D_t &object3) {
	ofstream myfile(filePath);
	float roundDigit = 100;
	int numOfVertices = (object.numOfVertices + object2.numOfVertices + object3.numOfVertices);
	int numOfFaces = (object.numOfFaces + object2.numOfFaces + object3.numOfFaces);

	if (myfile.is_open())
	{
		myfile << "COFF\n";
		myfile << numOfVertices << " "
			<< numOfFaces << " "
			<< 0 << endl;

		for (int i = 0; i < object.numOfVertices; i++)
		{
			myfile << roundf(object.pnt[i].x * roundDigit) / roundDigit << " "
				<< roundf(object.pnt[i].y * roundDigit) / roundDigit << " "
				<< roundf(object.pnt[i].z * roundDigit) / roundDigit << " ";
			myfile << object.col[i].r << " "
				<< object.col[i].g << " "
				<< object.col[i].b << " ";
			myfile << 255 << endl;
		}

		for (int i = 0; i < object2.numOfVertices; i++)
		{
			myfile << roundf(object2.pnt[i].x * roundDigit) / roundDigit << " "
				<< roundf(object2.pnt[i].y * roundDigit) / roundDigit << " "
				<< roundf(object2.pnt[i].z * roundDigit) / roundDigit << " ";
			myfile << object2.col[i].r << " "
				<< object2.col[i].g << " "
				<< object2.col[i].b << " ";
			myfile << 255 << endl;
		}

		for (int i = 0; i < object3.numOfVertices; i++)
		{
			myfile << roundf(object3.pnt[i].x * roundDigit) / roundDigit << " "
				<< roundf(object3.pnt[i].y * roundDigit) / roundDigit << " "
				<< roundf(object3.pnt[i].z * roundDigit) / roundDigit << " ";
			myfile << object3.col[i].r << " "
				<< object3.col[i].g << " "
				<< object3.col[i].b << " ";
			myfile << 255 << endl;
		}

		for (int i = 0; i < object.numOfFaces; i++)
		{
			myfile << object.fc[i].numOfVertices;
			for (int j = 0; j < object.fc[i].numOfVertices; j++)
			{
				myfile << " " << object.fc[i].pnt[j];
			}
			if (i != object.numOfFaces - 1)
			{
				myfile << endl;
			}
		}

		// berikan enter
		myfile << endl;

		for (int i = 0; i < object2.numOfFaces; i++)
		{
			myfile << object2.fc[i].numOfVertices;
			for (int j = 0; j < object2.fc[i].numOfVertices; j++)
			{
				myfile << " " << (object2.fc[i].pnt[j] + object.numOfVertices);
			}
			if (i != object2.numOfFaces - 1)
			{
				myfile << endl;
			}
		}

		// berikan enter
		myfile << endl;

		for (int i = 0; i < object3.numOfFaces; i++)
		{
			myfile << object3.fc[i].numOfVertices;
			for (int j = 0; j < object3.fc[i].numOfVertices; j++)
			{
				myfile << " " << (object3.fc[i].pnt[j] + object.numOfVertices + object2.numOfVertices);
			}
			if (i != object3.numOfFaces - 1)
			{
				myfile << endl;
			}
		}

		myfile.close();
	}
	else cout << "Unable to open file";
}

void MakeKerucut(object3D_t &kerucut, int n, float h, float r, float start, float end)
{
	kerucut.numOfVertices = n + 2;
	kerucut.numOfColors = n + 2;
	kerucut.numOfFaces = n * 2;

	kerucut.pnt[0].x = 0;
	kerucut.pnt[0].y = 0;
	kerucut.pnt[0].z = end;
	kerucut.col[0].r = 200;
	kerucut.col[0].g = 0;
	kerucut.col[0].b = 200;

	// Point
	for (int i = 0; i < kerucut.numOfVertices; i++)
	{
		float s = i * 2 * M_PI / n;
		kerucut.pnt[i + 1].x = r * cos(s);
		kerucut.pnt[i + 1].y = r * sin(s);
		kerucut.pnt[i + 1].z = start;
		kerucut.col[i + 1].r = rand() % 256;
		kerucut.col[i + 1].g = 100;
		kerucut.col[i + 1].b = 200;
	}

	kerucut.pnt[kerucut.numOfVertices - 1] = { 0, 0, end };

	// Faces
	for (int i = 1; i < n; i++)
	{
		kerucut.fc[i - 1] = { 3,{ 0, i + 1, i } };
	}

	kerucut.fc[n - 1] = { 3,{ 0, 1, n } };

	// Face alas

	for (int i = 1; i <= n; i++)
	{
		kerucut.fc[i + n - 1] = { 3,{ kerucut.numOfVertices - 1, i, i + 1 } };
	}

	kerucut.fc[kerucut.numOfFaces - 1] = { 3,{ kerucut.numOfVertices - 1, n, 1 } };
}

void makeTabung(object3D_t &obj, int n, float h, float r) {
	obj.numOfVertices = (n * 2) + 2;
	obj.numOfColors = (n * 2) + 2;
	obj.numOfFaces = n * 3;

	obj.pnt[0].x = 0;
	obj.pnt[0].y = 0;
	obj.pnt[0].z = -h;

	obj.col[0].r = rand() % 256;
	obj.col[0].g = rand() % 256;
	obj.col[0].b = rand() % 256;

	// Point
	for (int i = 0; i < obj.numOfVertices; i++)
	{
		float s = i * 2 * M_PI / n;
		if (i < (obj.numOfVertices / 2) - 1)
		{
			obj.pnt[i + 1].x = r * cos(s);
			obj.pnt[i + 1].y = r * sin(s);
			obj.pnt[i + 1].z = -h;
		}
		else
		{
			obj.pnt[i + 1].x = r * cos(s);
			obj.pnt[i + 1].y = r * sin(s);
			obj.pnt[i + 1].z = h;
		}

		obj.col[i + 1].r = rand() % 256;
		obj.col[i + 1].g = rand() % 256;
		obj.col[i + 1].b = rand() % 256;
	}

	obj.pnt[obj.numOfVertices - 1] = { 0, 0, h };

	// Faces
	for (int i = 1; i < n; i++)
	{
		obj.fc[i - 1] = { 4,{ i, i + 1, n + i + 1, n + i } };
	}

	obj.fc[n - 1] = { 4,{ n, 1, n + 1, obj.numOfVertices - 2 } };

	// Face alas

	// bawah
	for (int i = 1; i < n; i++)
	{
		obj.fc[i + n - 1] = { 3,{ 0, i, i + 1 } };
	}

	obj.fc[(n * 2) - 1] = { 3,{ 0, n, 1 } };

	// atas
	for (int i = n + 1; i < n * 2; i++)
	{
		obj.fc[i + n - 1] = { 3,{ obj.numOfVertices - 1, i + 1, i } };
	}

	obj.fc[obj.numOfFaces - 1] = { 3,{ obj.numOfVertices - 1, n + 1, obj.numOfVertices - 2 } };
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

void readMyObject()
{
	object3D_t obj;
	readDataset("permen.off", obj);

	vector3D_t normalVector, vec[10000], vecbuff[10000];
	point2D_t titik2d[10000];
	float normalzi = 0.;
	color_t warna[10000];

	matrix3D_t tilting = rotationX(sudut) * rotationY(sudut);

	for (int i = 0; i<obj.numOfVertices; i++)
	{
		vec[i] = point2vector(obj.pnt[i]);
		vec[i] = tilting * vec[i];
	}

	// Invisible
	for (int i = 0; i<obj.numOfFaces; i++)
	{
		for (int j = 0; j < obj.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[obj.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j < obj.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[obj.fc[i].pnt[j]]);
				warna[j] = obj.col[obj.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, obj.fc[i].numOfVertices, warna);
			//drawPolygon(titik2d, obj.fc[i].numOfVertices, { 0, 0, 255 });
			//fillPolygon(titik2d, obj.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, obj.fc[i].numOfVertices);
		}
	}

	// Visible
	for (int i = 0; i<obj.numOfFaces; i++)
	{
		for (int j = 0; j < obj.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[obj.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j < obj.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[obj.fc[i].pnt[j]]);
				warna[j] = obj.col[obj.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, obj.fc[i].numOfVertices, warna);
			//fillPolygon(titik2d, obj.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, obj.fc[i].numOfVertices);
		}
	}

	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();
}

void drawMyObject()
{
	object3D_t tabung;
	makeTabung(tabung, 6, 0.6, 0.2);

	object3D_t kerucut1;
	MakeKerucut(kerucut1, 6, 0.3, 0.3, -0.8, -0.6);

	object3D_t kerucut2;
	MakeKerucut(kerucut2, 6, 0.3, 0.3, 0.8, 0.6);
	
	writeMultipleObjectToFile("permen.off", kerucut2, tabung, kerucut1);

	vector3D_t normalVector, vec[1000], vecbuff[1000];
	point2D_t titik2d[1000];
	float normalzi = 0.;
	color_t warna[1000];

	// Draw Grid
	createGrid();

	// KERUCUT 2

	matrix3D_t tilting = rotationX(sudut) * rotationY(sudut);

	for (int i = 0; i<kerucut2.numOfVertices; i++)
	{
		vec[i] = point2vector(kerucut2.pnt[i]);
		vec[i] = tilting * vec[i];
	}

	// Invisible
	for (int i = 0; i<kerucut2.numOfFaces; i++)
	{
		for (int j = 0; j < kerucut2.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[kerucut2.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j < kerucut2.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[kerucut2.fc[i].pnt[j]]);
				warna[j] = kerucut2.col[kerucut2.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, kerucut2.fc[i].numOfVertices, warna);
			//drawPolygon(titik2d, kerucut2.fc[i].numOfVertices, { 0, 0, 255 });
			//fillPolygon(titik2d, kerucut2.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, kerucut2.fc[i].numOfVertices);
		}
	}

	// Visible
	for (int i = 0; i<kerucut2.numOfFaces; i++)
	{
		for (int j = 0; j < kerucut2.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[kerucut2.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j < kerucut2.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[kerucut2.fc[i].pnt[j]]);
				warna[j] = kerucut2.col[kerucut2.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, kerucut2.fc[i].numOfVertices, warna);
			//fillPolygon(titik2d, kerucut2.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, kerucut2.fc[i].numOfVertices);
		}
	}

	// Tabung

	normalVector, vec[1000], vecbuff[1000];
	titik2d[1000];
	normalzi = 0.;
	warna[1000];

	 tilting = rotationX(sudut) * rotationY(sudut);

	for (int i = 0; i<tabung.numOfVertices; i++)
	{
		vec[i] = point2vector(tabung.pnt[i]);
		vec[i] = tilting * vec[i];
	}

	// Invisible
	for (int i = 0; i<tabung.numOfFaces; i++)
	{
		for (int j = 0; j < tabung.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[tabung.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j < tabung.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[tabung.fc[i].pnt[j]]);
				warna[j] = tabung.col[tabung.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, tabung.fc[i].numOfVertices, warna);
			//drawPolygon(titik2d, tabung.fc[i].numOfVertices, { 0, 0, 255 });
			//fillPolygon(titik2d, tabung.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, tabung.fc[i].numOfVertices);
		}
	}

	// Visible
	for (int i = 0; i<tabung.numOfFaces; i++)
	{
		for (int j = 0; j < tabung.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[tabung.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j < tabung.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[tabung.fc[i].pnt[j]]);
				warna[j] = tabung.col[tabung.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, tabung.fc[i].numOfVertices, warna);
			//fillPolygon(titik2d, tabung.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, tabung.fc[i].numOfVertices);
		}
	}

	// KERUCUT 1

	tilting = rotationX(sudut) * rotationY(sudut);

	for (int i = 0; i<kerucut1.numOfVertices; i++)
	{
		vec[i] = point2vector(kerucut1.pnt[i]);
		vec[i] = tilting * vec[i];
	}

	// Invisible
	for (int i = 0; i<kerucut1.numOfFaces; i++)
	{
		for (int j = 0; j < kerucut1.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[kerucut1.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi < 0.)
		{
			for (int j = 0; j < kerucut1.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[kerucut1.fc[i].pnt[j]]);
				warna[j] = kerucut1.col[kerucut1.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, kerucut1.fc[i].numOfVertices, warna);
			//drawPolygon(titik2d, kerucut1.fc[i].numOfVertices, { 0, 0, 255 });
			//fillPolygon(titik2d, kerucut1.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, kerucut1.fc[i].numOfVertices);
		}
	}

	// Visible
	for (int i = 0; i<kerucut1.numOfFaces; i++)
	{
		for (int j = 0; j < kerucut1.fc[i].numOfVertices; j++) {
			vecbuff[j] = vec[kerucut1.fc[i].pnt[j]];
		}
		normalVector = (vecbuff[1] - vecbuff[0]) ^ (vecbuff[2] - vecbuff[0]);
		normalzi = normalVector.v[2];
		if (normalzi > 0.)
		{
			for (int j = 0; j < kerucut1.fc[i].numOfVertices; j++) {
				titik2d[j] = vector2point2d(vec[kerucut1.fc[i].pnt[j]]);
				warna[j] = kerucut1.col[kerucut1.fc[i].pnt[j]];
			}
			gradatePolygon(titik2d, kerucut1.fc[i].numOfVertices, warna);
			//fillPolygon(titik2d, kerucut1.fc[i].numOfVertices, { 255, 0, 0 });
			//drawPolyline(titik2d, kerucut1.fc[i].numOfVertices);
		}
	}

	sudut++;
	if (sudut >= 360.0)
		sudut = 0.0;
	glFlush();

}

void userdraw()
{
	//drawMyObject();
	readMyObject();
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
	glOrtho(-1, 1, -1, 1, 1, -1);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	//Inisialisasi Toolkit 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("My Delicious Candy");
	initialize();
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}
