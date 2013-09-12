#pragma once
#include "Building.h"
#include "Parser.h"
#include <string>


using namespace std;

class City
{
public:
	City(string cityFile);
	~City(void);

private:
	vector<Building> models;

};
