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

bool DateTools::IsLeapYear()
{
	return ((_Year % 4 == 0 && _Year % 100 != 0) || _Year % 400 == 0);
}

bool DateTools::IsLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

int DateTools::GetMouthDay(int month)
{
	int dayCount = 0;
	switch (month)
	{
	case 1:
		dayCount = 31;
		break;
	case 2:
		if (IsLeapYear())
		{
			dayCount = 29;
		}
		else
		{
			dayCount = 28;
		}
		break;
	case 3:
		dayCount = 31;
		break;
	case 4:
		dayCount = 30;
		break;
	case 5:
		dayCount = 31;
		break;
	case 6:
		dayCount = 30;
		break;
	case 7:
		dayCount = 31;
		break;
	case 8:
		dayCount = 31;
		break;
	case 9:
		dayCount = 30;
		break;
	case 10:
		dayCount = 31;
		break;
	case 11:
		dayCount = 30;
		break;
	case 12:
		dayCount = 31;
		break;
	default:
		dayCount = -1;
	}
	return dayCount;
}

int DateTools::GetDateRank()
{
	if (_Year <= 0 || _Month < 1 || _Month > 12 || _Day < 1 || _Day > GetMouthDay(_Month))
	{
		return -1;
	}
	int dayCount = 0;

	if (_Month == 1)
	{
		return _Day;
	}
	else if (_Month == 2)
	{
		return 31 + _Day;
	}
	else
	{
		dayCount = 31 + 28;
		for (int i = 3; i < _Month; i++)
		{
			dayCount += GetMouthDay(i);
		}
		if (IsLeapYear())
		{
			dayCount += (_Day + 1);
		}
		else
		{
			dayCount += _Day;
		}
	}
	return dayCount;
}

int DateTools::DateToInt()
{
	return _Year * 10000 + _Month * 100 + _Day * 1;
}

int DateTools::CalculateGapDay(DateTools& date)
{
	int beginYear, beginMonth, beginDay, endYear, endMonth, endDay;
	int gap = 0;
	if (DateToInt() > date.DateToInt())
	{
		beginYear = date.GetYear();
		beginMonth = date.GetMonth();
		beginDay = date.GetDay();
		endYear = _Year;
		endMonth = _Month;
		endDay = _Day;
	}
	else
	{
		beginYear = _Year;
		beginMonth = _Month;
		beginDay = _Day;
		endYear = date.GetYear();
		endMonth = date.GetMonth();
		endDay = date.GetDay();
	}
	while (true)
	{
		if (beginYear == endYear && beginMonth == endMonth && beginDay == endDay)
		{
			break;
		}
		beginDay++;
		gap++;
		if (beginMonth == 12 && beginDay == 32)
		{
			beginYear++;
			beginMonth = 1;
			beginDay = 1;
			continue;
		}
		if ((beginMonth == 1 || beginMonth == 3 || beginMonth == 5 || beginMonth == 7 || beginMonth == 8 || beginMonth == 10) && beginDay == 32)
		{
			beginMonth++;
			beginDay = 1;
			continue;
		}
		if ((beginMonth == 4 || beginMonth == 6 || beginMonth == 9 || beginMonth == 11) && beginDay == 31)
		{
			beginMonth++;
			beginDay = 1;
			continue;
		}
		if (beginMonth == 2 && IsLeapYear(beginYear) && beginDay == 30)
		{
			beginMonth++;
			beginDay = 1;
			continue;
		}
		if (beginMonth == 2 && !IsLeapYear(beginYear) && beginDay == 29)
		{
			beginMonth++;
			beginDay = 1;
			continue;
		}
	}
	return gap;
}

DateTools DateTools::GetDaysAfter(int dayCount)
{
	int year = _Year;
	int month = _Month;
	int day = _Day;
	for (int i = 0; i < dayCount; i++)
	{
		day++;
		if (month == 12 && day == 32)
		{
			year++;
			month = 1;
			day = 1;
			continue;
		}
		if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10) && day == 32)
		{
			month++;
			day = 1;
			continue;
		}
		if ((month == 4 || month == 6 || month == 9 || month == 11) && day == 31)
		{
			month++;
			day = 1;
			continue;
		}
		if (month == 2 && IsLeapYear(year) && day == 30)
		{
			month++;
			day = 1;
			continue;
		}
		if (month == 2 && !IsLeapYear(year) && day == 29)
		{
			month++;
			day = 1;
			continue;
		}
	}
	return DateTools(year, month, day);
}

void DateTools::DaysAfter(int dayCount)
{
	for (int i = 0; i < dayCount; i++)
	{
		_Day++;
		if (_Month == 12 && _Day == 32)
		{
			_Year++;
			_Month = 1;
			_Day = 1;
			continue;
		}
		if ((_Month == 1 || _Month == 3 || _Month == 5 || _Month == 7 || _Month == 8 || _Month == 10) && _Day == 32)
		{
			_Month++;
			_Day = 1;
			continue;
		}
		if ((_Month == 4 || _Month == 6 || _Month == 9 || _Month == 11) && _Day == 31)
		{
			_Month++;
			_Day = 1;
			continue;
		}
		if (_Month == 2 && IsLeapYear(_Year) && _Day == 30)
		{
			_Month++;
			_Day = 1;
			continue;
		}
		if (_Month == 2 && !IsLeapYear(_Year) && _Day == 29)
		{
			_Month++;
			_Day = 1;
			continue;
		}
	}
}

string DateTools::ToString()
{
	return IntToString(_Year) + "/" + IntToString(_Month) + "/" + IntToString(_Day);
}

string DateTools::IntToString(int number)
{
	string _String;
	stringstream ss;
	ss << number;
	ss >> _String;
	return _String;
}

int DateTools::StringToInt(string number)
{
	int _Number = 0;
	stringstream ss;
	ss << number;
	ss >> _Number;
	return _Number;
}

void DateTools::splitString(string srcString, char splitFlag, vector<string>& stringVector)
{
	stringstream ss(srcString);
	string splitString;
	while (getline(ss, splitString, splitFlag))
	{
		stringVector.push_back(splitString);
	}
}

string DateTools::getCurrentTime()
{
	string currentTime;
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	currentTime.append(IntToString(systemTime.wYear)).append("/");
	currentTime.append(IntToString(systemTime.wMonth)).append("/");
	currentTime.append(IntToString(systemTime.wDay)).append("_");
	currentTime.append(IntToString(systemTime.wHour)).append(":");
	currentTime.append(IntToString(systemTime.wMinute)).append(":");
	currentTime.append(IntToString(systemTime.wSecond)).append(":");
	currentTime.append(IntToString(systemTime.wMilliseconds));
	return currentTime;
}

DateTools DateTools::getCurrentDate()
{
	DateTools currentDate;
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	currentDate.SetYear(systemTime.wYear);
	currentDate.SetMonth(systemTime.wMonth);
	currentDate.SetDay(systemTime.wDay);
	return currentDate;
}

istream& operator >> (istream& in, DateTools& date)
{
	in >> date._Year >> date._Month >> date._Day;
	return in;
}

ostream& operator<<(ostream& out, DateTools& date)
{
	out << date.ToString();
	return out;
}