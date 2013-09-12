/***********************************
Name: Jin Kim
Student #: 66068081
************************************/
#ifndef _PARSER_H_
#define _PARSER_H_
#include "Building.h"
#include <string>
#include <vector>
#include <fstream>


using namespace std;

class Building;

class Parser
{
public:
	Parser(void);
	Parser(string filename, bool isModel);
	vector<string> TokenizeString(string str, const char * delimiters);

	string getDirectory(string pathname);

	
	Building** parseCity(string cityFile);

	float **getVertices(void);
	float **getTriangles(void);
	float **getNormals(void);
	string *getTextureList(void);

	void printVertices(void);
	void printVerticesToFile(string filename);
	void printTriangles(void);
	void printTrianglesToFile(string filename);
	void parseObjFile(string filename);
	void printModelFile(string filename, string buildingName);
	~Parser(void);

	int getNumBuildings();

	bool isWhitespace(char c);
	bool isNumber(char c);

private:
	Building* parseModel(string modelFile, string texturePath);
	int numBuildings;

	ifstream modelFile;
	ofstream outputFile;
	string line;

	float **vertices;
	int vertsize;

	float **textures;
	int texsize;

	float **triangles;
	int trisize;

	float **normals;
	int norsize;

	string *textureNames;
	int texnamesize;

	string cityName;
	int num_models;
};
#endif