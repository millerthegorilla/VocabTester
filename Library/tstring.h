#if !defined(TSTRING_H)
#define TSTRING_H

#include <string>
#include <sstream>
#include <tchar.h>

#if defined(UNICODE)

typedef std::wstring        tstring;
typedef std::wostringstream tostringstream;
typedef std::wistringstream tistringstream;

#else // ANSI

typedef std::string        tstring;
typedef std::ostringstream tostringstream;
typedef std::istringstream tistringstream;

#endif // UNICODE

#endif // TSTRING_H