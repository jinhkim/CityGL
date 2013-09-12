/***********************************
Name: Jin Kim
Student #: 66068081
EECE 478 City
************************************/
#include <fstream>
#include "PPM.hpp"
#include <iostream>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <fstream>
#include <math.h>
#include <vector>
#include "Parser.h"
#include "Building.h"

using namespace std;	

const float DEG2RAD = 3.14159/180;

float x=0.0, y=20.0, z=-20.0;		//x, y, and z position of camera
float lx = 0.0f, ly = -30.0f, lz=30.0f;	//x, y, z direction of camera
float mx = 0.0, my = 0.0, mz = 0.0;  //x,y,z position of object
float angle = 0.0;	//x, z angle of the camera
float Yangle = 0.0; //y angle of camera
float m_angle = 0.0;

float lr,lg,lb;

float xC=0.0f,yC=0.0f,zC=0.0f;

int ppmCounter;

vector<float> rightVector(3);

float deltaAngle = 0.0f;
float deltaYAngle = 0.0f;
float xRot = 0.0f, yRot = 0.0f; //rotation x,y in degrees
float xOrigin = 0.0f, yOrigin = 0.0f;
float xCam = 0.0f, yCam = 0.0f;
float deltaMouseX = 0.0f, deltaMouseY = 0.0f;
float deltaMove = 0.0f;
float deltaX = 0.0f;
float deltaY = 0.0f;
int xInitial = -1; //x position of mouse cursor
int yInitial = -1; //y position of mouse cursor
int windowW, windowH; //width and height of window

int currentTime, previousTime=0;
int frameCount;
double fps;

GLuint *textures_t;
GLuint buildingList = 0;
GLuint ground = 0;
GLvoid *font = GLUT_BITMAP_HELVETICA_18;

bool leftPressX, leftPressY, rightPress, middlePress, 
	 defaultView, firstRun;

int lightning=0;

//vector<vector<float>> m_vertices,m_triangles,m_normals;

float** m_vertices,m_triangles,m_normals;

//vector<string> textureNames;

string *textureNames;

bool rendering = true;
bool init = false;
bool useTextures, useNormals;

Building** buildings;
int num_buildings;

char ss[50];

void setOrtho(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, windowW, windowH, 0);
	glMatrixMode(GL_MODELVIEW);
}

void restoreProj(){
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//renders a string
void renderText(float x, float y, float z, void *font, char *text){
	char *c;
	glDisable(GL_LIGHTING);
	glRasterPos3f(x,y,z);
	for(c = text+'\0'; *c != '\0'; c++){
		glutBitmapCharacter(font,*c);
	}
	glEnable(GL_LIGHTING);
}

void renderTextOnScreen(float x, float y, float z, void *font_style, char *text_t){
	setOrtho();
	glPushMatrix();
	glLoadIdentity();
	renderText(x,y,z,font_style,text_t);
	glPopMatrix();
	restoreProj();
}

void getFramerate(){
	frameCount++;

	//  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
	sprintf(ss,"FPS: %4.2f",fps);
	//int text_size = (sizeof(ss)/sizeof(char));
	renderTextOnScreen(5,30,0,font,ss);
}


void changeSize(int w, int h) {

	windowW = w;
	windowH = h;

	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,0.1,100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	y += deltaMove * ly * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeXPos(float deltaX){
	x += deltaX * rightVector[0] * 0.1f;
	y += deltaX * rightVector[1] * 0.1f;
	z += deltaX * rightVector[2] * 0.1f;
}

void computeYPos(float deltaY){
	//x += deltaY * 0.1f;
	y += deltaY * 0.1f;
	//z += deltaY * 0.1f;
}

////computes direction of camera
//void computeDir(float deltaAngle) {
//
//	angle += deltaAngle;
//	lx = sin(angle);
//	ly = cos(angle);
//	lz = -cos(angle);
//}

vector<float> crossProduct(float ax,float ay, float az, float bx, float by, float bz){
	//assert(forwardVector.size() == 3);
	vector<float> ans(3);
	ans[0] = ay*bz - az*by;
	ans[1] = az*bx - ax*bz;
	ans[2] = ax*by - ay*bx; 
	return ans;
}

void computeRot(float xRot){
	//xRot += deltaMouseAngle * 0.1f;
	//cout<<xRot<<endl;
}

//void textures(){
//	GLubyte *imgData;
//	int w,h,i,index=0;
//
//	textures_t = new GLuint[textureNames.size()];
//	glGenTextures(textureNames.size(), &textures_t[0]);
//
//	for(i=0;i<textureNames.size();i++){
//		imgData = (GLubyte *) PPM::Read(textureNames[i],w,h);
//
//		glBindTexture(GL_TEXTURE_2D,textures_t[i]);
//		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,imgData);
//		delete imgData;
//	}
//}

 
void drawCircle(float radius, float r, float g, float b)
{
	if(lightning)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
   glBegin(GL_TRIANGLES);
 
   glColor3f(r,g,b);

   for (int i=0; i < 360; i=i+36)
   {
      float degInRad = i*DEG2RAD;
      glVertex3f(cos(degInRad)*radius,0,sin(degInRad)*radius);
	  glVertex3f(0,0,0);
	  glVertex3f(cos(degInRad+(36*DEG2RAD))*radius,0,sin(degInRad+(36*DEG2RAD))*radius);
   }

   glEnd();
}

void recordFrames(string ppmName){ // Paste this above void Display()
	unsigned char * pixels;
	pixels = new unsigned char[windowW*windowH*3];
	glReadPixels(0,0,windowW,windowH,GL_RGB,GL_UNSIGNED_BYTE,pixels);
	PPM::Write(ppmName,pixels,windowW,windowH);
	ppmCounter++;
	delete pixels;
}

void moveAroundWorld(){
	xC+=0.01f;
	x = 50*sin(xC);
	lx = -x;

	zC+=0.01f;
	z = 50*cos(zC);
	lz = -z;
}


void renderScene(void) {

	char ppmFileName[10];
	if(ppmCounter < 10){
		sprintf_s(ppmFileName, "000%d.ppm", ppmCounter);
	} else if(ppmCounter < 100){
		sprintf_s(ppmFileName, "00%d.ppm", ppmCounter);
	} else if(ppmCounter < 1000){
		sprintf_s(ppmFileName, "0%d.ppm", ppmCounter);
	} else if(ppmCounter < 10000){
		sprintf_s(ppmFileName, "%d.ppm", ppmCounter);
	}

	//string temp = "../video/";
	//temp.append(ppmFileName);
	//recordFrames(temp);

	//moveAroundWorld();

	if(firstRun){
		defaultView = false;
		xOrigin = xRot;
		yOrigin = yRot;
	}
	
	if (deltaMove)
		computePos(deltaMove);
	//if (deltaAngle)
		//computeDir(deltaAngle);
	if (deltaX){
		//calc right vector
		rightVector = crossProduct(lx,ly,lz,0,1,0);
		computeXPos(deltaX);
	}
	if (deltaY){
		computeYPos(deltaY);
	}
	
	//if(true){
	//	lr = 5.0f;
	//	lg = 5.0f;
	//	lb = 5.0f;
	//} else {
	//	lr = 0.0f;
	//	lg = 0.0f;
	//	lb = 0.0f;
	//}

	//init lighting stuff
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color(0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0.0f, 20.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	////Lightning lights
	GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos1[] = {0.0f, 20.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	//GLfloat lightColor2[] = {lr, lg, lb, 1.0f}; //Color (0.5, 0.5, 0.5)
 //   GLfloat lightPos2[] = {-25.0f, 10.0f, 25.0f, 1.0f}; //Positioned at (4, 0, 8)
 //   glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor1);
 //   glLightfv(GL_LIGHT2, GL_POSITION, lightPos1);

	//GLfloat lightColor3[] = {lr, lg, lb, 1.0f}; //Color (0.5, 0.5, 0.5)
 //   GLfloat lightPos3[] = {-25.0f, 10.0f, -25.0f, 1.0f}; //Positioned at (4, 0, 8)
 //   glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor2);
 //   glLightfv(GL_LIGHT3, GL_POSITION, lightPos2);

	//GLfloat lightColor4[] = {lr, lg, lb, 1.0f}; //Color (0.5, 0.5, 0.5)
 //   GLfloat lightPos4[] = {25.0f, 10.0f, -25.0f, 1.0f}; //Positioned at (4, 0, 8)
 //   glLightfv(GL_LIGHT4, GL_DIFFUSE, lightColor3);
 //   glLightfv(GL_LIGHT4, GL_POSITION, lightPos3);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	gluLookAt(	x, y, z,		//camera position
			x+lx, y+ly,  z+lz,	//point the camera is looking at (target)
			0.0f, 1.0f,  0.0f);	//up vector


	glPushMatrix();
	// Draw the positive side of the lines x,y,z
	glBegin(GL_LINES);
		glColor3f (0.0, 1.0, 0.0); // Green for x axis
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);
		glColor3f(1.0,0.0,0.0); // Red for y axis
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
		glColor3f(0.0,0.0,1.0); // Blue for z axis
		glVertex3f(0,0,0); 
		glVertex3f(0,0,10);
	glEnd();
	glPopMatrix();

	glScalef(0.3,0.3,0.3);

	if(!defaultView){
		//rotate model
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	} else{
		glRotatef(-xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(-yRot, 0.0f, 1.0f, 0.0f);
	}
	glColor3f(0.5f,0.5f,0.5f);
	glTranslatef(mx-3.0,my,mz+3.0);

	/***************************************************/
	int g;
	for(g = 0; g < num_buildings; g++){
		glPushMatrix();
		glTranslatef(buildings[g]->getTranslate().x, 
			buildings[g]->getTranslate().y, 
			buildings[g]->getTranslate().z);
		glScalef(buildings[g]->getScale().x, 
			buildings[g]->getScale().y,
			buildings[g]->getScale().z);
		glRotatef(buildings[g]->getRot().z, 0, 0, 1);
		glRotatef(buildings[g]->getRot().y, 0, 1, 0);
		glRotatef(buildings[g]->getRot().x, 1, 0, 0);
		glTranslatef(buildings[g]->getOrigin().x, 
			buildings[g]->getOrigin().y,
			buildings[g]->getOrigin().z);
		if(lightning==0){
			glEnable(GL_TEXTURE_2D);
			lr = 0.6;
			lg = 0.6;
			lb = 0.6;
		}
		else{
			glDisable(GL_TEXTURE_2D);
			lr = 0.9;
			lg = 0.9;
			lb = 0.9;
		}
		buildings[g]->renderModel();
		glPopMatrix();
	}
	glPushMatrix();
	drawCircle(120.0,lr,lg,lb);
	glPopMatrix();
	//glPushMatrix();
	//glCallList(ground);
	//int g;
	//for(g = 0; g < num_buildings; g++){
	//	glCallList(buildingList+g);
	//}
	//glPopMatrix();

	/***************************************************/

	glColor3f(1.0,1.0,0.0);
	//renderTextOnScreen(10,windowH-40,0,font,"Jin Kim #66068081");
	glPopMatrix();

	glColor3f(1.0,1.0,1.0);
	getFramerate();

	glutSwapBuffers();
}




/*********************************
	KEYBOARD EVENTS
**********************************/
void pressKey(unsigned char key, int x, int y){
	switch (key) {
		case 'a' : deltaX = -2.0f; break;
		case 'd' : deltaX = 2.0f; break;
		case 'w' : deltaMove = 2.0f; break;
		case 's' : deltaMove = -2.0f; break;
		case 'r' : deltaY = 2.0f; break;
		case 'f' : deltaY = -2.0f; break;
		case '0' : defaultView = true; break;
		case 't' : lightning = 1; break;
		case 27  : exit(0);
	}
}
void releaseKey(unsigned char key, int x, int y){
	switch (key) {
		case 'a' : deltaX = 0.0f; break; 
		case 'd' : deltaX = 0.0f; break;
		case 'w' : deltaMove = 0.0f; break;
		case 's' : deltaMove = 0.0f; break;
		case 'r' : deltaY = 0.0f; break;
		case 'f' : deltaY = 0.0f; break;
		case '0' : defaultView = false; break;
		case 't' : lightning = 0; break;

	}
}

void specialKeys(int key, int xx, int yy){
	switch(key){
		case GLUT_KEY_LEFT:
			deltaAngle -= 0.1f; 
			//lx = sin(angle + deltaAngle);
			//lz = -cos(angle + deltaAngle);
			break;
		case GLUT_KEY_RIGHT:
			deltaAngle += 0.1f; 
			//lx = sin(angle + deltaAngle);
			//lz = -cos(angle + deltaAngle);
			break;
	}
}

void specialUpKeys(int key, int xx, int yy){
	switch(key){
		case GLUT_KEY_LEFT:
			deltaAngle = 0.0f; 
			break;
		case GLUT_KEY_RIGHT:
			deltaAngle = 0.0f; 
			break;
	}
}


/*********************************
	MOUSE EVENTS
**********************************/
void mouseButton(int button,int state, int x, int y){
	
	//only update when left mouse button is pressed
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		leftPressX = true;
		leftPressY = true;
		xInitial = x;
		yInitial = y;
	} 
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		rightPress = true;
		deltaMouseX = x - yRot;
		deltaMouseY = -y + xRot;
	} 
	else if(state == GLUT_UP){ 
		leftPressX = false;
		leftPressY = false;
		rightPress = false;

		angle += deltaAngle;
		Yangle += deltaYAngle;

		deltaAngle = 0;
		deltaYAngle = 0;
	}
}
void mouseMotion(int x, int y) {

	//only rotate if mouse button is pressed
	if(rightPress){
		yRot = (x - deltaMouseX)*0.5f;
		xRot = (y + deltaMouseY)*0.5f;
	}
	//rotation around y-axis
	if(leftPressX){
		deltaAngle = (x - xInitial) * 0.005f;

		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	} 
	//rotation around x-axis
	if(leftPressY){
		deltaYAngle = (y - yInitial) * 0.005f;

		ly = -sin(Yangle + deltaYAngle);
	} 

}

void buildLists(){
	ground = glGenLists(1);
	glNewList(ground,GL_COMPILE);
	glPushMatrix();
		drawCircle(120,0.1,1.0,0.1);
	glPopMatrix();
	glEndList();

	buildingList = glGenLists(num_buildings);
	if(!buildingList){
		cout<<"error"<<endl;
		exit(0);
	}
	int g;
	for(g = 0; g < num_buildings; g++){
		glNewList(buildingList+g,GL_COMPILE);
			glTranslatef(buildings[g]->getTranslate().x, 
				buildings[g]->getTranslate().y, 
				buildings[g]->getTranslate().z);
			glScalef(buildings[g]->getScale().x, 
				buildings[g]->getScale().y,
				buildings[g]->getScale().z);
			glRotatef(buildings[g]->getRot().z, 0, 0, 1);
			glRotatef(buildings[g]->getRot().y, 0, 1, 0);
			glRotatef(buildings[g]->getRot().x, 1, 0, 0);
			glTranslatef(buildings[g]->getOrigin().x, 
				buildings[g]->getOrigin().y,
				buildings[g]->getOrigin().z);
			buildings[g]->renderModel();
		glEndList();
	}
}


/**************************************************
Main Loop

The first argument of this program is the path to either 
a .obj file or a .model file. The second parameter is the
directory which holds the .ppm texture files.

In the constructor to Parser, there is a boolean value 
isModel, which is false if you want to use a .obj file,
and true if you want to use a .model file

Currently, there are issues parsing .obj files correctly.
Parsing and rendering .model files, however, work fine.
***************************************************/
int main(int argc, char** argv){

	int windowWidth = 720,
		windowHeight = 480;

	useTextures = true;

	//Parser pp("../KoernerLibrary/KoernerLibrary.model", true);
	//Building koernerLib("../KoernerLibrary/KoernerLibrary.model",
	//					"../KoernerLibrary/");

	Parser pp;
	ifstream ff;
	cout<<__FILE__<<endl;

	buildings = pp.parseCity("../univercity.city");
	num_buildings = pp.getNumBuildings();

	//buildingList = glGenLists(num_buildings);
	//if(!buildingList){
	//	cout<<"error"<<endl;
	//	return 0;
	//}


	//pp.printTrianglesToFile("triangles.txt");
	//pp.printVerticesToFile("vertices.txt");

	//m_vertices = pp.getVertices();
	//m_triangles = pp.getTriangles();
	//textureNames = pp.getTextureList();
	//m_normals = pp.getNormals();

	

	//koernerLib.setVertices(pp.getVertices());
	//koernerLib.setTriangles(pp.getTriangles());
	//koernerLib.setNormals(pp.getNormals());

	//m_vertices = koernerLib.getVertices();
	//m_triangles = koernerLib.getTriangles();
	//textureNames = koernerLib.getTextureNames();
	//m_normals = koernerLib.getNormals();


	if(rendering){

		//initializing callback functions
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		//glutInitWindowPosition(800,100);
		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-windowWidth)/2,
                       (glutGet(GLUT_SCREEN_HEIGHT)-windowHeight)/2);
		glutInitWindowSize(windowWidth,windowHeight);
		glutCreateWindow("Assignment 1");
		//glutFullScreen();

		//rendering callback functions
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);
		glutIdleFunc(renderScene);

		//keyboard events
		glutKeyboardFunc(pressKey);
		glutKeyboardUpFunc(releaseKey);
		glutSpecialFunc(specialKeys);
		glutSpecialUpFunc(specialUpKeys);

		//mouse events
		glutMouseFunc(mouseButton);
		glutMotionFunc(mouseMotion);

		//init OpenGL
		glEnable(GL_DEPTH_TEST);

		//init lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);

		//init textures
		//int i;
		//GLubyte *temp;
		//unsigned char **imgArray;
		//imgArray = new unsigned char *imgArray[(textureNames.size()+1)];


		//glEnable(GL_TEXTURE_2D);
		//for(i=0;i<textureNames.size();i++){
		//	imgArray[i] = textures(m_triangles[i][4]);
		//}

		if(useTextures){
			for(int s = 0; s < num_buildings; s++){
				buildings[s]->initTextures();
			}
		}

		//buildLists();



		//main glut event loop
		glutMainLoop();
	}
		
	return 1;
}