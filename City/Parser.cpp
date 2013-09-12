/***********************************
Name: Jin Kim
Student #: 66068081
************************************/

#include "Parser.h"
#include "PPM.hpp"
#include "Building.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

enum parser_state { 
	UNKNOWN,
	NAME,
	TEXTURES,
	VERTICES,
	NORMALS,
	TRIANGLES
};

Parser::Parser(void){

}

Parser::~Parser(){
	delete [] vertices;
	delete [] textures;
	delete [] triangles;
	delete [] normals;
	delete [] textureNames;
}

//check if char is useful
bool Parser::isWhitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
		return true;
	else
		return false;
}

// check if the char is a number
bool Parser::isNumber(char c)
{
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' 
		|| c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
		return true;
	else
		return false;
}


//takes a string and tokenizes it using the delimiters parameter
//outputs a vector of tokens
vector<string> Parser::TokenizeString(string someStr, const char * delimiters){
	vector<string> vec;
	char * cstr, *p;
	string str = someStr;
	cstr = new char[str.size()+1];

	//copy original string into a c-string
	strcpy(cstr, str.c_str());

	p = strtok(cstr,delimiters);
	while(p != NULL){
		vec.push_back(p);
		p = strtok(NULL,delimiters);
	}

	delete[] cstr;
	return vec;
}

string Parser::getDirectory(string path){
	string p = path;
	int pathIndex = 0;
	for(pathIndex=p.length()-1;pathIndex>0;pathIndex--){
		if(p[pathIndex] == char(47)){
			break;
		}
	}
	p = p.substr(0,pathIndex+1);
	return p;
}

int Parser::getNumBuildings(){return numBuildings;}

Building* Parser::parseModel(string modelFile, string texturePath){
	Building* ret_building = NULL; // building object to be returned
	string line; // used to collect .model file line by line
	vector<string> tokens; // holds significant tokens from each line

	// .model file information holders
	string model_name = "";
	string model_num_textures = "";
	string* textureNames;
	// 2d arrays
	float **vertices;
	float **normals;
	float **triangles;

	int columns,data_col,data_row;

	cout<<"parsing model"<<endl;
	ifstream model_file (modelFile.c_str());
	if (model_file.is_open())
	{	
		// .model file is open, parse state is UNKNOWN
		ret_building = new Building();
		parser_state p_state = UNKNOWN;

		while ( model_file.good() )
		{
			getline (model_file,line); // get next line in file
			string tok = "";
			for (int i=0; i < line.length(); i++) // for all chars in the line
			{
				if (!isWhitespace(line[i])) { // if it isn't whitespace
					if (line[i] == '#') // check for comments
						break;
					tok = tok + line[i]; // add char to current token

				} else { // if it is whitespace
					if (tok.compare("") != 0) // check that current token isn't empty
						tokens.push_back(tok); // add to token vector
					tok = ""; // reset current token
				}
			}
			if (tok.compare("") != 0) // make sure the final token is added
				tokens.push_back(tok);

			// iterate through tokens on that line
			for (vector<string>::iterator iter = tokens.begin(); iter != tokens.end();++iter)
			{
				string s = *iter; // current token
				if (s.compare(">") == 0) continue;
				switch (p_state) { // check parser state
					case UNKNOWN:
						/***
							parser is in UNKNOWN state:
								look for info header (i.e '<name' or '<name>')
								once found, change parser state and prepare info holders
						***/
						if (s.compare("<name>") == 0 || s.compare("<name") == 0) 
							p_state = NAME;

						if (s.compare("<textures") == 0) {
							cout<<"parsing textures"<<endl;
							p_state = TEXTURES;
							string num = "";
							do { // find the 'size' param in info header
								iter++;
								string find_size = *iter;
								
								for (int letter=0; letter < find_size.size(); letter++) {
									if (isNumber(find_size[letter]))
										num += find_size[letter];
								}
							} while(iter != tokens.end()-1);
							textureNames = new string[atoi(num.c_str())]; // create new string array based on header info
							data_row = 0; // initiate row index to zero
						}

						if (s.compare("<vertices") == 0) {
							cout<<"parsing vertices"<<endl;
							p_state = VERTICES;
							string num = "";
							do { // find the 'size' param in info header
								iter++;
								string find_size = *iter;
								
								for (int letter=0; letter < find_size.size(); letter++) {
									if (isNumber(find_size[letter]))
										num += find_size[letter];
								}
							} while(iter != tokens.end()-1);
							data_row = 0; //initiate row and column indexs
							data_col = 0;
							columns = 3;
							// create 2d array using size info from header and 'columns'
							vertices = new float*[atoi(num.c_str())];
							for (int rows=0; rows < atoi(num.c_str()); rows++)
								vertices[rows] = new float[columns];						
						}

						if (s.compare("<normals") == 0) {
							cout<<"parsing normals"<<endl;
							p_state = NORMALS;
							string num = "";
							do { // find the 'size' param in info header
								iter++;
								string find_size = *iter;
								
								for (int letter=0; letter < find_size.size(); letter++) {
									if (isNumber(find_size[letter]))
										num += find_size[letter];
								}
							} while(iter != tokens.end()-1);
							data_row = 0; //initiate row and column indexs
							data_col = 0;
							columns = 3;
							// create 2d array using size info from header and 'columns'
							normals = new float*[atoi(num.c_str())];
							for (int rows=0; rows < atoi(num.c_str()); rows++)
								normals[rows] = new float[columns];
						}

						if (s.compare("<triangles") == 0) {
							cout<<"parsing triangles"<<endl;
							p_state = TRIANGLES;
							string num = "";
							do { // find the 'size' param in info header
								iter++;
								string find_size = *iter;
						
								for (int letter=0; letter < find_size.size(); letter++) {
									if (isNumber(find_size[letter]))
										num += find_size[letter];
								}
							} while(iter != tokens.end()-1);
							data_row = 0; //initiate row and column indexs
							data_col = 0;
							columns = 11;
							// create 2d array using size info from header and 'columns'
							triangles = new float*[atoi(num.c_str())];
							for (int rows=0; rows < atoi(num.c_str()); rows++)
								triangles[rows] = new float[columns];
						}
						break;

					case NAME:
						/****
							parser is in NAME state:
								It will continue to look for the section closing statement
								(i.e. '</name' or '</name>'), until it does it will add each
								token to the current name string with a trailing space. Once
								the closing statement has been found, the last unnecessary space
								must be deleted and the info is saved in the building object.
						****/
						if (s.compare("</name>") == 0 || s.compare("</name") == 0) {
							if (model_name.size() > 0)
								model_name.erase(model_name.size()-1); // remove last " "
							else
								model_name = "NO NAME";
							p_state = UNKNOWN; // return parser to UNKNOWN state
							ret_building->setModelName(model_name); // save name to building object
							break;
						}
						model_name = model_name + s + " "; // add current token to model_name with " "
						break;

					case TEXTURES:
						/****
							parser is in TEXTURES state:
								It will continue to look for the section closing statement
								(i.e. '</textures' or '</textures>'), until it does it will add each
								token to the current texture array and increment the row index value.
								Once the closing statement has been found, the info is saved in the building object.
						****/
						if (s.compare("</textures>") == 0 || s.compare("</textures") == 0) {
							p_state = UNKNOWN;
							ret_building->setTextureNames(textureNames,data_row);
							texnamesize = data_row;
							break;
						}
						textureNames[data_row] = s;
						data_row++;
						break;

					case VERTICES:
						/****
							parser is in VERTICES state:
								It will continue to look for the section closing statement
								(i.e. '</vertices' or '</vertices>'), until it does it will add each
								token to the current vertice array and increment the column index value.
								If the column index value reaches the maximum number of columns, the row
								index is incremented and the column index is reset. Once the closing statement 
								has been found, the info is saved in the building object.
						****/
						if (s.compare("</vertices>") == 0 || s.compare("</vertices") == 0) {
							p_state = UNKNOWN;
							ret_building->setVertices(vertices,data_row);
							vertsize = data_row;
							break;
						}
						vertices[data_row][data_col] = atof(s.c_str());
						data_col++;
						if (data_col == columns) {
							data_col = 0;
							data_row++;
						}
						break;

					case NORMALS:
						/****
							parser is in NORMALS state:
								See VERTICIES above...
						****/
						if (s.compare("</normals>") == 0 || s.compare("</normals") == 0) {
							p_state = UNKNOWN;
							ret_building->setNormals(normals,data_row);
							norsize = data_row;
							break;
						}
						normals[data_row][data_col] = atof(s.c_str());
						data_col++;
						if (data_col == columns) {
							data_col = 0;
							data_row++;
						}
						break;

					case TRIANGLES:
						/****
							parser is in NORMALS state:
								See VERTICIES above...
						****/
						if (s.compare("</triangles>") == 0 || s.compare("</triangles") == 0) {
							p_state = UNKNOWN;
							ret_building->setTriangles(triangles,data_row);
							trisize = data_row;
							break;
						}
						triangles[data_row][data_col] = atof(s.c_str());
						data_col++;
						if (data_col == columns) {
							data_col = 0;
							data_row++;
						}
						break;

				} // end switch
			} // end for all important tokens on that line

			tokens.clear();
		} // end while file still has lines to get
		model_file.close();
	} else
		throw "That .model file cannot be opened.";
	ret_building->setTexturePath(texturePath);
	cout<<"parsing model complete"<<endl;
	return ret_building;
}

Building** Parser::parseCity(string city_file){
	vector<Building> someBuildings;
	ifstream cityFile;
	vector<string> temp;
	vector<vector<string>> buildings;
	const char * delimiter = " <>=\t\r\n";
	cityFile.open(city_file.c_str());
	cout<<"parsing city file.. "<<endl;

	//parse the file
	if(cityFile.is_open()){
		while(cityFile.good()){

			getline(cityFile,line);
			
			temp = TokenizeString(line,delimiter);

			
			if(temp.size()>0){
				if(temp[0][0] == '#')
					continue;
				else {
					buildings.push_back(temp);
				} 
			}

		}
		cout<<"Done."<<endl;
		
		cityFile.close();
	}
	else {
		cout<<"Error: Could not open city file" << endl;
	}
	cityName = buildings[0][0];
	num_models = atoi(buildings[1][0].data());
/*****************Parse into vars*******************/
	Building **tempBuilding;
	string modelPath,modelDir;
	int cc = 2,rr = 0;
	assert(buildings.size() > 2);

	tempBuilding = new Building*[buildings.size()-2];

	//for each building model, parse its model file and store vars
	while(cc < buildings.size()){
		//get path to model
		modelPath = buildings[cc][0]; //this is the path of the model
		modelDir = getDirectory(modelPath);

		tempBuilding[cc-2] = parseModel(modelPath,modelDir);

		//get scale coords
		tempBuilding[cc-2]->setScale(atof(buildings[cc][1].data()),
								   atof(buildings[cc][2].data()),
								   atof(buildings[cc][3].data()));

		//get rotate coords
		tempBuilding[cc-2]->setRot(atof(buildings[cc][4].data()),
							     atof(buildings[cc][5].data()),
								 atof(buildings[cc][6].data()));

		//get origin coords
		tempBuilding[cc-2]->setOrigin(atof(buildings[cc][7].data()),
									atof(buildings[cc][8].data()),
									atof(buildings[cc][9].data()));

		//get translate coords
		tempBuilding[cc-2]->setTranslate(atof(buildings[cc][10].data()),
									   atof(buildings[cc][11].data()),
									   atof(buildings[cc][12].data()));
		cc++;
	}
	numBuildings = buildings.size()-2;
	cout<<"Finished Parsing."<<endl;

	return tempBuilding;
}



float **Parser::getVertices(void){return vertices;}
float **Parser::getTriangles(void){return triangles;}
float **Parser::getNormals(void){return normals;}
string *Parser::getTextureList(void){return textureNames;}

void Parser::printVertices(void){

	int i,j;
	for(i = 0; i<vertsize;i++){
		for(j = 0; j<3;j++){
			cout<<vertices[i][j]<<" ";
		}
		cout<<endl;
	}
}
void Parser::printVerticesToFile(string filename){

	int i,j;
	ofstream outputFile;
	outputFile.open(filename.c_str());

	if(outputFile.bad()){
		cout<<"\nError: cannot write to file. Aborting."<<endl;
		exit(0);
	}

	for(i = 0; i<vertsize;i++){
		for(j = 0; j<3;j++){
			outputFile<<vertices[i][j]<<" ";
		}
		outputFile<<endl;
	}
	outputFile.close();
}
void Parser::printTriangles(void){

	int i,j;
	for(i = 0; i<trisize;i++){
		for(j = 0; j<3;j++){
			cout<<triangles[i][j]<<" ";
		}
		cout<<endl;
	}
}
void Parser::printTrianglesToFile(string filename){

	int i,j;
	ofstream outputFile;
	outputFile.open(filename.c_str());

	if(outputFile.bad()){
		cout<<"\nError: cannot write to file. Aborting."<<endl;
		exit(0);
	}

	for(i = 0; i<trisize;i++){
		for(j = 0; j<3;j++){
			outputFile<<triangles[i][j]<<" ";
		}
		outputFile<<endl;
	}
	outputFile.close();
}
