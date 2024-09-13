#if !defined (WINSTRING_H)
#define WINSTRING_H
//
// (c) Reliable Software 1997
//

#include <windows.h>

class ResString
{
    enum { MAX_RESSTRING=255 };
public:
    ResString (HINSTANCE hInst, int resId);
	ResString () { _buf [0] = '\0'; }
    operator char const * () { return _buf; }
private:
    char _buf [MAX_RESSTRING + 1];
};

#endif
