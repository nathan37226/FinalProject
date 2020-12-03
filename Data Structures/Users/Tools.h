#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
using namespace std;

//The tools of date

class DateTools
{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//		Constructor and Destructor
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DateTools();
	DateTools(int year, int month, int day);
	DateTools(string date);
	DateTools(int date);
	~DateTools();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	 Getter
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetYear();
	int GetMonth();
	int GetDay();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	 Setter
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetYear(int year);
	void SetMonth(int month);
	void SetDay(int day);
	
	string IntToString(int number);
	int StringToInt(string number);
	string getCurrentTime();
	

private:
	int _Year;
	int _Month;
	int _Day;
};

#include "Tools.cpp"
#endif
