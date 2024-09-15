#if !defined STRTIME_H
#define STRTIME_H
//
// (c) Reliable Software 1997 -- 2003
//
#include <ctime>

unsigned long const Second = 1;
unsigned long const Minute = 60 * Second;
unsigned long const Hour = 60 * Minute;
unsigned long const Day = 24 * Hour;
unsigned long const Week = 7 * Day;

class StrTime
{
public:
    StrTime ()
    {
        time (&_value);
		Init (_value);
    }
    StrTime (time_t timeValue)
    {
		Init (timeValue);
	}
	void Init (time_t timeValue)
	{
		_value = timeValue;
		char * timeStr = ctime (&_value);
		if (timeStr != 0)
		{
			strcpy (_string, timeStr);
			// The string result produced by ctime contains exactly 26 characters and
			// has the following format:
			//             1         2
			//   012345678901234567890123 4 5
			//   Wed Jan 02 02:03:55 1980\n\0
			// Remove the endin '\n'
			_string [24] = '\0';
		}
		else
		{
			_string [0] = '\0';
		}
    }
    char const * GetString () const { return _string; }
    time_t GetValue ()  const { return _value;  }
	bool IsOlderThen (unsigned long period) const
	{
		time_t now;
		time (&now);
		double elapsedTime = difftime (now, _value);
		return elapsedTime > period;
	}

private:
    time_t	_value;
    char	_string [28];
};

#if 0
class StrTime
{
public:
    StrTime (char const * localeName = "")
    {
        time (&_value);
		Init (_value, localeName);
    }
    StrTime (time_t timeValue, char const * localeName = "")
    {
		Init (timeValue, localeName);
	}
	void Init (time_t timeValue, char const * localeName = "")
	{
		_value = timeValue;
		tm * tmValue = localtime (&timeValue);

		std::locale loc (localeName);
		std::time_put<char> const & tp = std::use_facet<std::time_put<char> >(loc);
		std::ostringstream out;
		//out.imbue (loc);
		char format [] = "%a, %c";
		tp.put (std::ostream::_Iter (out.rdbuf ()), out, ' ', 
			tmValue, format, format + sizeof (format));
		_string = out.str ();
    }
    char const * GetString () const { return _string.c_str (); }
    time_t GetValue ()  const { return _value;  }
	bool IsOlderThen (unsigned long period) const
	{
		time_t now;
		time (&now);
		double elapsedTime = difftime (now, _value);
		return elapsedTime > period;
	}

private:
    time_t	_value;
	std::string _string;
};
#endif

#endif
