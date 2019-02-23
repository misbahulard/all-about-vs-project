// GenerateFacesCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

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
	point3D_t pnt[600];
	int numOfColors;
	color_t col[10000];
	int numOfFaces;
	face_t fc[1200];
}object3D_t;

typedef struct {
	int total;
	float arr[4];
} point;

typedef struct {
	int num;
	point data[1000];
} dataCluster;

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

void readDatasetVerticesOnly(string filename, object3D_t &fileObj) {
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

void errorCentroid(dataCluster c[], dataCluster c_old[], int k, float &error)
{
	float distance = 0;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			distance += pow((c[i].data[0].arr[j] - c_old[i].data[0].arr[j]), 2);
		}
	}

	error = sqrt(distance);
}

void euclideanDistance(point data, dataCluster c[], int k, float distances[])
{
	float distance;

	for (int i = 0; i < k; i++)
	{
		distance = 0;
		for (int j = 0; j < 3; j++)
		{
			distance += pow((data.arr[j] - c[i].data[0].arr[j] ), 2);
		}
		distances[i] = sqrt(distance);
	}

	/*cout << "DISTANCES: " << endl;
	for (int i = 0; i < k; i++)
	{
		cout << i << ": " << distances[i] << endl;
	}*/
}

void generateFaces(string file, int n)
{
	object3D_t obj;
	dataCluster dc;
	int k = n;
	dataCluster c[10];
	dataCluster c_old[10];
	int clusters[10000];

	readDatasetVerticesOnly(file, obj);

	dc.num = obj.numOfVertices;
	for (int i = 0; i < obj.numOfVertices; i++)
	{
		dc.data[i].total = 3;
		dc.data[i].arr[0] = obj.pnt[i].x;
		dc.data[i].arr[1] = obj.pnt[i].y;
		dc.data[i].arr[2] = obj.pnt[i].z;
	}

	cout << "My Awesome points" << endl;
	for (int i = 0; i < dc.num; i++)
	{
		for (int j = 0; j < dc.data->total; j++)
		{
			cout << dc.data[i].arr[j] << endl;
		}
		cout << "---------" << endl;
	}

	// Generate Centroid
	int r = 0;
	for (int i = 0; i < k; i++)
	{
		// Random integer
		int min = 0;
		int max = dc.num;

		random_device rd;
		mt19937 rng(rd());
		uniform_int_distribution<int> uni(min, max);
		int r = uni(rng);

		cout << "\nMy r: " << r << endl;
		c[i].num = 1;
		for (int j = 0; j < dc.data[r].total; j++)
		{
			c[i].data[0].total = dc.data[r].total;
			c[i].data[0].arr[j] = dc.data[r].arr[j];
		}
	}

	cout << "\nMy Centroid" << endl;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < c[i].data[0].total; j++)
		{
			cout << c[i].data[0].arr[j] << endl;
		}
		cout << "---------" << endl;
	}

	// simpan centroid lama untuk dihitung error
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < c[i].data[0].total; j++)
		{
			c_old[i].data[0].total = 3;
			c_old[i].data[0].arr[j] = 0;
		}
	}

	float error;
	errorCentroid(c, c_old, k, error);

	cout << "\nERROR: " << error << endl;

	while (error != 0) {
		
		float distances[10];
		for (int i = 0; i < dc.num; i++)
		{
			int cluster;

			cout << "\nDIST: " << i << endl;
			euclideanDistance(dc.data[i], c, k, distances);

			cout << "DISTANCES: " << endl;
			for (int x = 0; x < k; x++)
			{
				cout << x << ": " << distances[x] << endl;
			}

			// Cari masukkan ke kluster mana
			int min = distances[0];
			cluster = 0;
			for (int x = 0; x < k; x++)
			{
				if (min > distances[x])
				{
					min = distances[x];
					cluster = x;
				}
			}
			cout << "Cluster ke: " << cluster << endl;

			// simpan index ke cluster
			clusters[i] = cluster;
		}

		// simpan centroid lama untuk dihitung error
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < c[i].data[0].total; j++)
			{
				c_old[i].data[0].total = c[i].data[0].total;
				c_old[i].data[0].arr[j] = c[i].data[0].arr[j];
			}
		}

		// hitung ulang centroid
		for (int i = 0; i < k; i++)
		{
			point points[1000];
			int counter = 0;
			for (int j = 0; j < dc.num; j++)
			{
				if (clusters[j] == i)
				{
					// simpan titik
					for (int x = 0; x < dc.data[x].total; x++)
					{
						points[counter].arr[x] = dc.data[j].arr[x];
					}
					counter++;
				}
			}

			cout << "MY COUNTER: " << counter << endl;

			if (counter != 0)
			{
				float x = 0, y = 0, z = 0;
				float xmean = 0, ymean = 0, zmean = 0;

				for (int j = 0; j < counter; j++)
				{
					x += points[j].arr[0];
					y += points[j].arr[1];
					z += points[j].arr[2];
				}

				xmean = x / counter;
				ymean = y / counter;
				zmean = z / counter;

				c[i].data[0].arr[0] = xmean;
				c[i].data[0].arr[1] = ymean;
				c[i].data[0].arr[2] = zmean;
			}
		}

		cout << "\New Centroid" << endl;
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < c[i].data[0].total; j++)
			{
				cout << c[i].data[0].arr[j] << endl;
			}
			cout << "---------" << endl;
		}

		errorCentroid(c, c_old, k, error);

		cout << "\nERROR: " << error << endl;
	}

	// tampilkan

	for (int i = 0; i < dc.num; i++)
	{
		cout << "Data: " << dc.data[i].arr[0] << ", " << dc.data[i].arr[1] << ", " << dc.data[i].arr[2] << " | Cluster: " << clusters[i] << endl;
	}

	dataCluster cData[10];

	for (int i = 0; i< k; i++)
	{
		cout << "K: " << i << endl;
		int jmlData = 0;
		for (int j = 0; j < dc.num; j++)
		{
			if (i == clusters[j])
			{
				cData[i].data[jmlData].arr[0] = dc.data[i].arr[0];
				cData[i].data[jmlData].arr[1] = dc.data[i].arr[1];
				cData[i].data[jmlData].arr[2] = dc.data[i].arr[2];
				jmlData++;
			}
		}
		cData[i].num = jmlData;
	}

	cout << "\n\nDATA CLUSTERED" << endl;

	for (int x = 0; x < k; x++)
	{
		cout << "K: " << x << endl;
		for (int i = 0; i < cData[x].num; i++)
		{
			cout << "Data: " << cData[x].data[i].arr[0] << ", " << cData[x].data[i].arr[1] << ", " << cData[x].data[i].arr[2] << endl;
		}
	}


	int counter = 0;

	for (int x = 0; x < k; x++)
	{
		for (int i = 0; i < cData[x].num; i++)
		{
			for (int j = i + 1; j < cData[x].num; j++)
			{
				if (j + 1 < cData[x].num)
				{
					obj.fc[counter].numOfVertices = 3;
					obj.fc[counter].pnt[0] = i;
					obj.fc[counter].pnt[1] = j + 1;
					obj.fc[counter].pnt[2] = j;
					counter++;
				}
				else {
					break;
				}
			}
		}
	}

	obj.numOfFaces = counter;

	cout << "FACES: " << obj.numOfFaces << endl;



	/*cout << "FACES" << endl;
	for (int i = 0; i<obj.numOfFaces; i++)
	{
		cout << "fc ke " << i << ": " << obj.fc[i].pnt[0] << " - " << obj.fc[i].pnt[1] << " - " << obj.fc[i].pnt[2] << endl;
	}*/

	//writeObjectToFile("kmeans.off", obj);


	ofstream myfile("kmeans.off");
	float roundDigit = 100;
	if (myfile.is_open())
	{
		myfile << "COFF\n";
		myfile << obj.numOfVertices << " "
			<< obj.numOfFaces << " "
			<< 0 << endl;
		for (int i = 0; i < obj.numOfVertices; i++)
		{
			myfile << roundf(obj.pnt[i].x * roundDigit) / roundDigit << " "
				<< roundf(obj.pnt[i].y * roundDigit) / roundDigit << " "
				<< roundf(obj.pnt[i].z * roundDigit) / roundDigit << " ";
			myfile << obj.col[i].r << " "
				<< obj.col[i].g << " "
				<< obj.col[i].b << " ";
			myfile << 255 << endl;
		}
		for (int i = 0; i < obj.numOfFaces; i++)
		{
			myfile << obj.fc[i].numOfVertices;
			for (int j = 0; j < obj.fc[i].numOfVertices; j++)
			{
				myfile << " " << obj.fc[i].pnt[j];
			}
			if (i != obj.numOfFaces - 1)
			{
				myfile << endl;
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";

}

int main()
{
	generateFaces("kubus.off", 5);
    return 0;
}

