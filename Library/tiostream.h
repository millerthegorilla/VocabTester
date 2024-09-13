#if !defined(TIOSTREAM_H)
#define TIOSTREAM_H

#include <iostream>
#include <tchar.h>

#if defined(UNICODE)

#define tcout std::wcout
#define tcerr std::wcerr

#else // ANSI

#define tcout std::cout
#define tcerr std::cerr

#endif // UNICODE

#endif // TIOSTREAM_H