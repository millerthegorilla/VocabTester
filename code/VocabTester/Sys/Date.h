#if !defined (DATE_H)
#define DATE_H
// -----------------------------------
// (c) Reliable Software, 2003 -- 2004
// -----------------------------------

#include <Sys/SysTime.h>

class Date
{
public:
	Date ()	{ Clear ();	}
	Date (int month, int day, int year)
	{
		Reset (month, day, year);
	}
	int Year  () const { return _year;  }
	int Month () const { return _month; }
	int Day   () const { return _day;   }

	void Now ();
	void Reset (int month, int day, int year);
	void Clear ()
	{
		_year  = 0;
		_month = 0;
		_day   = 0;
	}
	bool IsSet () const
	{
		return _year  != 0 ||
			   _month != 0 ||
			   _day   != 0;
	}
	bool IsPast () const;
	static bool IsValid (int month, int day, int year);
	static int DaysInMonth (int month, int year);
	void AddDays (int days);
	std::string ToString (bool shortFormat = false) const;

protected:
	int _year;
	int _month;
	int _day;
};

#endif
