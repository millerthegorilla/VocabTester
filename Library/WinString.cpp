//
// (c) Reliable Software 1997
//

#include "WinString.h"

ResString::ResString (HINSTANCE hInst, int resId)
{
    if (!::LoadString (hInst, resId, _buf, MAX_RESSTRING + 1))
        throw "Load String failed";
}

