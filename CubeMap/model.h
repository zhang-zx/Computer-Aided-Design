#include "Vector3D.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SOIL.h>
#include <GLUT/glut.h>

#ifndef _H_MODEL_
#define _H_MODEL_

struct Vertex
{
	double x, y, z;
};

struct TexCoord
{
    double u, v;
};

struct Face
{
	int vertices[3];
	Vector3D normal;
};

class Model
{
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
    
    std::vector<TexCoord> texcoords;

	Vertex center;
	double scaleD;

public:
	Model();
	~Model();

	bool LoadHeightMap(std::string fileName, int xscale, int yscale, int zscale);

	void Render(int sign);
};

#endif
