#if !defined (MESSAGE_H)
#define MESSAGE_H
//---------------------------
// (c) Reliable Software 1998
//---------------------------

#include <windows.h>

// Reserved by Reliable Software Library
const UINT MSG_RS_LIBRARY		= WM_USER + 0x4000;

// wParam = new position wrt parent's left edge
const UINT MSG_MOVESPLITTER		= MSG_RS_LIBRARY + 1;


#endif
