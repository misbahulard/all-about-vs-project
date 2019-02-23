// ConsoleCoff.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

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
	point3D_t pnt[10000];
	int numOfColors;
	color_t col[10000];
	int numOfFaces;
	face_t fc[3200];
} object3D_t;


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

int main()
{
	// File to read
	ifstream file("limas.off");
	string content;
	string result;

	int maxVertices = 0;
	int maxFaces = 0;

	object3D_t obj;

	// Loop until reach End of File
	int index = 1;
	while (getline(file, content))
	{
		// Get number of Vertices and Faces
		if (index == 2)
		{
			result = strWord(1, content);
			obj.numOfVertices = stoi(result);
			result = strWord(2, content);
			obj.numOfFaces = stoi(result);

			// Set for next looping
			maxVertices = 3 + obj.numOfVertices;
			maxFaces = maxVertices + obj.numOfFaces;
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
			obj.pnt[index - 3] = pnt;

			// save color
			float warna[3];
			for (int i = 4; i <= 6; i++)
			{
				result = strWord(i, content);
				warna[i - 4] = strtof(result.c_str(), 0);
			}
			color_t col = { warna[0], warna[1], warna[2] };
			obj.col[index - 3] = col;
		}
		else if ( index >= maxVertices && index < maxFaces)
		{
			// num of point | titik pembentuk faces
			float titik2[4];
			for (int i = 1; i <= 4; i++)
			{
				result = strWord(i, content);
				titik2[i - 1] = strtof(result.c_str(), 0);
			}
			// set jumlah tiitk pembuat face
			obj.fc[index - 3 - obj.numOfVertices].numOfVertices = titik2[0];
			// Set titik pembentuk faces
			obj.fc[index - 3 - obj.numOfVertices].pnt[0] = titik2[1];
			obj.fc[index - 3 - obj.numOfVertices].pnt[1] = titik2[2];
			obj.fc[index - 3 - obj.numOfVertices].pnt[2] = titik2[3];
		}

		index = index + 1;
	}

	/*for (int i = 0; i < obj.numOfVertices; i++)
	{
		cout << fixed << "x: " << obj.pnt[i].x << " y: " << obj.pnt[i].y << " z: " << obj.pnt[i].z << endl;
	}*/

	/*for (int i = 0; i < obj.numOfVertices; i++)
	{
		cout << fixed << "x: " << obj.col[i].r << " y: " << obj.col[i].g << " z: " << obj.col[i].b << endl;
	}*/

	for (int i = 0; i < obj.numOfFaces; i++)
	{
		for (int j = 0; j < obj.fc[i].numOfVertices; j++) {
			cout << obj.fc[i].pnt[j] << " ";
		}
		cout << endl;
	}

    return 0;
}

