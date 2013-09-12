#ifndef _BUILDING_H_
#define _BUILDING_H_
#include "Parser.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <string>

using namespace std;

typedef struct {
	float x;
	float y;
	float z;
}coord3D;

class Building
{
public:
	Building(void);
	Building(char *modelFile, char *texturePath);
	~Building(void);

	void initTextures(void);
	void renderModel(void);

	//get/set methods
	void setModelName(string name);
	void setVertices(float **vert, int size);
	void setTextures(GLuint *textures, int size);
	void setTriangles(float **tri, int size);
	void setNormals(float **nor, int size);
	void setTextureNames(string *texNames, int size);
	void setTexturePath(string pathname);

	void setScale(float x, float y, float z);
	void setRot(float x, float y, float z);
	void setOrigin(float x, float y, float z);
	void setTranslate(float x, float y, float z);

	float **getVertices();
	GLuint *getTextures();
	float **getTriangles();
	float **getNormals();
	string *getTextureNames();

	coord3D getScale();
	coord3D getRot();
	coord3D getOrigin();
	coord3D getTranslate();
	
private:
	//vector<vector<float>> vertices;
	//vector<vector<float>> triangles;
	//vector<vector<float>> normals;
	//vector<string> textureNames;

	coord3D scale;
	coord3D rot;
	coord3D origin;
	coord3D translate;

	string modelName;

	float **vertices;
	int vertSize;

	float **triangles;
	int triSize;

	float **normals;
	int normSize;

	string *textureNames;
	int texNameSize;

	GLuint *textures;
	int texturesSize;

	string texturePath;

};
#endif