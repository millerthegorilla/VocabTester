// (c) Reliable Software 2004

#include <WinLibBase.h>
#include "Date.h"

int MonthDays [] =
{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

inline bool IsLeapYear (int year)
{
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int Date::DaysInMonth (int month, int year)
{
	int days = MonthDays [month];
	if (month == 2 && IsLeapYear (year))
		++days;
	return days;
}

void Date::Now ()
{
	Win::Time today;
	Reset (today.Month (), today.Day (), today.Year ());
}

void Date::Reset (int month, int day, int year)
{
	Assert (IsValid (month, day, year));

	_year  = year;
	_month = month;
	_day   = day;
}

bool Date::IsPast () const
{
	Assert (IsValid (_month, _day, _year));
	Win::Time today;

	if (_year != today.Year ())
		return _year < today.Year ();

	if (_month != today.Month ())
		return _month < today.Month ();

	return _day <= today.Day ();
}

bool Date::IsValid (int month, int day, int year)
{
	return year >= 1 
		&& month >= 1 
		&& month <= 12 
		&& day >= 1 
		&& day <= DaysInMonth (month, year);
}

void Date::AddDays (int days)
{
	Assert (days >= 0);
	Assert (IsValid (_month, _day, _year));
	_day += days;
	while (_day > DaysInMonth (_month, _year))
	{
		_day -= DaysInMonth (_month, _year);
		++_month;
		if (_month > 12)
		{
			++_year;
			_month = 1;
		}
	}
	Assert (IsValid (_month, _day, _year));
}

std::string Date::ToString (bool shortFormat) const
{
	Assert (IsValid (_month, _day, _year));
	SYSTEMTIME sysTime;
	memset (&sysTime, 0, sizeof (SYSTEMTIME));
	sysTime.wDay = _day;
	sysTime.wMonth = _month;
	sysTime.wYear = _year;
	// Format date using locale information
	std::string date;
	unsigned long flags = shortFormat ? DATE_SHORTDATE : DATE_LONGDATE;
	int dateLen = ::GetDateFormat (LOCALE_USER_DEFAULT, flags, &sysTime, 0, 0, 0);
	if (dateLen > 0)
	{
		date.resize (dateLen);
		::GetDateFormat (LOCALE_USER_DEFAULT, flags, &sysTime, 0, &date [0], date.size ());
		// APIs appended '\0' at the end of date
		date.replace (dateLen - 1, dateLen, 1, ' ');
	}
	return date;
}
