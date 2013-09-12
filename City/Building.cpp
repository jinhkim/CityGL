#include "Building.h"
#include "Parser.h"
#include "PPM.hpp"
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;


Building::Building(void){
	vertices = NULL;
	triangles = NULL;
	normals = NULL;
	textureNames = NULL;
	texturePath = "";
}

Building::~Building(void)
{
	delete [] vertices;
	delete [] textures;
	delete [] triangles;
	delete [] normals;
	delete [] textureNames;
}

void Building::initTextures(){

	GLubyte *imgData;
	int w,h,i,index=0;

	textures = new GLuint[texNameSize];
	glGenTextures(texNameSize, &textures[0]);

	for(int i=0; i < texNameSize; i++){
		textureNames[i] = texturePath + textureNames[i];
	}

	for(i=0;i<texNameSize;i++){
		imgData = (GLubyte *) PPM::Read(textureNames[i],w,h);

		glBindTexture(GL_TEXTURE_2D,textures[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,imgData);
		delete imgData;
	}
}

void Building::renderModel(){
	//glNewList(listIndex,GL_COMPILE);
	//glPushMatrix();


		for(int i=0;i<triSize;i++){
			glBindTexture(GL_TEXTURE_2D,textures[(int)triangles[i][4]]);
			glBegin(GL_TRIANGLES);
			glNormal3f(normals[(int)triangles[i][3]][0],
				normals[(int)triangles[i][3]][1],
				normals[(int)triangles[i][3]][2]);
			glTexCoord2f(triangles[i][5],triangles[i][6]);
			glVertex3f(vertices[(int)triangles[i][0]][0],
				vertices[(int)triangles[i][0]][1],
				vertices[(int)triangles[i][0]][2]);
			glTexCoord2f(triangles[i][7],triangles[i][8]);
			glVertex3f(vertices[(int)triangles[i][1]][0],
				vertices[(int)triangles[i][1]][1],
				vertices[(int)triangles[i][1]][2]);
			glTexCoord2f(triangles[i][9],triangles[i][10]);
			glVertex3f(vertices[(int)triangles[i][2]][0],
				vertices[(int)triangles[i][2]][1],
				vertices[(int)triangles[i][2]][2]);
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	//glPopMatrix();
	//glEndList();
	
	
	//delete imgArray;
}

//SET methods
void Building::setModelName(string name){
	modelName = name;
}
void Building::setVertices(float **vert, int size){
	vertices = vert;
	vertSize = size;
}
void Building::setTextures(GLuint* tex, int size){
	textures = tex;
	texturesSize = size;
}
void Building::setTriangles(float **tri, int size){
	triangles = tri;
	triSize = size;
}
void Building::setNormals(float **nor, int size){
	normals = nor;
	normSize = size;
}
void Building::setTextureNames(string *texNames, int size){
	textureNames = texNames;
	texNameSize = size;
}
void Building::setTexturePath(string pathname){
	texturePath = pathname;
}

void Building::setScale(float x, float y, float z){
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
void Building::setRot(float x, float y, float z){
	rot.x = x;
	rot.y = y;
	rot.z = z;
}
void Building::setOrigin(float x, float y, float z){
	origin.x = x;
	origin.y = y;
	origin.z = z;
}
void Building::setTranslate(float x, float y, float z){
	translate.x = x;
	translate.y = y;
	translate.z = z;
}


//GET methods
float **Building::getVertices(){return vertices;}
GLuint* Building::getTextures(){return textures;}
float **Building::getTriangles(){return triangles;}
float **Building::getNormals(){return normals;}
string *Building::getTextureNames(){return textureNames;}

coord3D Building::getScale(){return scale;}
coord3D Building::getRot(){return rot;}
coord3D Building::getOrigin(){return origin;}
coord3D Building::getTranslate(){return translate;}