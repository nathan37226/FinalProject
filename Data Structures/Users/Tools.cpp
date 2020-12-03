#include "Tools.h"

DateTools::DateTools()
{
	_Year = 0;
	_Month = 0;
	_Day = 0;
}

DateTools::DateTools(int year, int month, int day)
{
	_Year = year;
	_Month = month;
	_Day = day;
}

DateTools::DateTools(string date)
{
	int dateNumber = StringToInt(date);
	_Year = dateNumber / 10000;
	_Month = (dateNumber - _Year * 10000) / 100;
	_Day = dateNumber % 100;
}

DateTools::DateTools(int date)
{
	_Year = date / 10000;
	_Month = (date - _Year * 10000) / 100;
	_Day = date % 100;
}


DateTools::~DateTools()
{
}

int DateTools::GetYear()
{
	return _Year;
}

int DateTools::GetMonth()
{
	return _Month;
}

int DateTools::GetDay()
{
	return _Day;
}

void DateTools::SetYear(int year)
{
	_Year = year;
}

void DateTools::SetMonth(int month)
{
	_Month = month;
}

void DateTools::SetDay(int day)
{
	_Day = day;
}

string DateTools::IntToString(int number)
{
	string _String;
	stringstream ss;
	ss << number;
	ss >> _String;
	return _String;
}

string DateTools::getCurrentTime()
{
	string currentTime;
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	currentTime.append(IntToString(systemTime.wYear)).append("/");
	currentTime.append(IntToString(systemTime.wMonth)).append("/");
	currentTime.append(IntToString(systemTime.wDay)).append(" ");
	currentTime.append(IntToString(systemTime.wHour)).append(":");
	currentTime.append(IntToString(systemTime.wMinute)).append(":");
	currentTime.append(IntToString(systemTime.wSecond)).append(":");
	currentTime.append(IntToString(systemTime.wMilliseconds));
	return currentTime;
}

