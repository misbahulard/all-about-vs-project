// LinearProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <conio.h>

main()
{
	int x[3], y[3], z[3];
	float hz, hy, hx, hslx, hsly;
	printf("Mencari Persamaan Linier\ndengan bentuk :");
	printf("x1 + y1 = z1");
	printf("x2 + y2 = z2");
	printf("========================");
	for (int i = 1; i<3; i++)
	{
		printf("Masukkan Nilai x", +i); 
		scanf("%f", x[i]);
		printf("Masukkan Nilai y", +i); 
		scanf("%f", y[i]);
		printf("Masukkan Nilai z", +i); 
		scanf("%f", z[i]);
	}
	printf("Sehingga menjadi :");
	printf("x = ", +x[1] || "y = ", +y[1] || "z = ", +z[1]);
	printf("x = ", +x[2] || "y = ", +y[2] || "z = ", +z[2]);
	if (x[1] == x[2])
	{
		int m;
		hy = y[1] - y[2];
		hz = z[1] - z[2];
		hsly = hz / hy;
		m = hsly * y[1];
		hx = z[1] - m;
		hslx = hx / x[1];
	}
	else
	{
		int m, mx, my;
		float n, x1, y1, z1, x2, y2, z2;
		m = x[2] * x[1];
		mx = m / x[1];
		my = m / x[2];
		y1 = y[1] * mx;
		z1 = z[1] * mx;
		y2 = y[2] * my;
		z2 = z[2] * my;
		hy = y1 - y2;
		hz = z1 - z2;
		hsly = hz / hy;
		n = hsly * y[1];
		hx = z[1] - n;
		hslx = hx / x[1];
	}
	printf("Jawabannya adalah :");
	printf("Nilai x = ", +hslx);
	printf("Nilai y = ", +hsly);
}