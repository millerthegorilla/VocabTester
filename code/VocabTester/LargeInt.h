#if !defined (LARGEINT_H)
#define LARGEINT_H
// (c) Reliable Software, 2003

class LargeInteger
{
public:
	LargeInteger () {}
	LargeInteger (LARGE_INTEGER li) : _value (li) {}
	LargeInteger (__int64 val) { _value.QuadPart = val; }
	LargeInteger (unsigned long low, long high)
	{
		_value.LowPart = low;
		_value.HighPart = high;
	}
	bool IsLarge () const { return _value.HighPart != 0; }
	bool IsZero () const { return _value.LowPart == 0 && _value.HighPart == 0; }
	unsigned long Low () const { return _value.LowPart; }
	unsigned long High () const { return _value.HighPart; }
	LARGE_INTEGER ToNative () const { return _value; }
	LARGE_INTEGER * ToPtr () { return &_value; }
	bool operator== (LargeInteger li) const
	{
		return _value.QuadPart == li._value.QuadPart;
	}
	bool operator != (LargeInteger li) const { return !operator== (li); }
	__int64 ToMath () const { return _value.QuadPart; }
	LargeInteger operator+= (unsigned long addend)
	{
		_value.QuadPart += addend;
		return *this;
	}
	LargeInteger operator+= (LargeInteger addend)
	{
		_value.QuadPart += addend._value.QuadPart;
		return *this;
	}
	LargeInteger operator *= (unsigned long multiplier)
	{
		_value.QuadPart *= multiplier;
		return *this;
	}
protected:
	LARGE_INTEGER _value;
};

#endif
