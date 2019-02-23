// MatricesTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>

using namespace std;

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

int main()
{
	printf("Matrices Test\n");
	
	point2D_t pnt1 = { 5, 0 };
	vector2D_t vec1 = { {1, 2, 1} };

	matrix2D_t mat1 = translationMTX(4, 6);
	matrix2D_t mat2 = scalingMTX(4, 6);
	matrix2D_t mat3 = rotationMTX(90);

	vector2D_t vecRes1 = point2vector(pnt1);
	vector2D_t vecRes2 = point2vector(pnt1);
	vector2D_t vecRes3 = point2vector(pnt1);

	/*cout << "\nMatriks translasi" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << mat1.m[i][j] << " ";
		}
		cout << endl;
	}*/

	cout << "\nSetelah dikali dengan matriks translasi: " << endl;
	vecRes1 = operator * (mat1, vecRes1);

	for (int i = 0; i < 3; i++)
	{
		cout << vecRes1.v[i] << endl;
	}

	cout << "\nSetelah dikasli dengan matriks skalasi: " << endl;

	vecRes2 = operator * (mat2, vecRes2);

	for (int i = 0; i < 3; i++)
	{
		cout << vecRes2.v[i] << endl;
	}

	cout << "\nSetelah dikasli dengan matriks rotasi: " << endl;

	vecRes3 = operator * (mat3, vecRes3);

	for (int i = 0; i < 3; i++)
	{
		cout << vecRes3.v[i] << endl;
	}


	point2D_t pnt = { 0,0 };
	vector2D_t vec = point2vector(pnt);

	matrix2D_t matRotasi = rotationMTX(90);
	//matrix2D_t matTranslasi = translationMTX(0, 0);
	//matrix2D_t matFinal = operator*(matRotasi, matTranslasi);
	//vec = operator*(matFinal, vec);
	vec = operator*(matRotasi, vec);
	pnt = vector2point(vec);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << matRotasi.m[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\n\n" << endl;
	cout << vec.v[0] << ", " << vec.v[1] << ", " << vec.v[2] << endl;
	cout << pnt.x << ", " << pnt.y << endl;

	do {
		cout << '\n' << "Press the Enter key to continue.";
	} while (cin.get() != '\n');

    return 0;
}

