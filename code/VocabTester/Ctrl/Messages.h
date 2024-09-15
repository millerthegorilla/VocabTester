#if !defined (MESSAGES_H)
#define MESSAGES_H
//-------------------------------------
// (c) Reliable Software 1998, 99, 2000
//-------------------------------------

// Reserved by Reliable Software Library
const unsigned int MSG_RS_LIBRARY		= 0x4000;

// wParam = New position wrt parent's left edge
const unsigned int UM_MOVESPLITTER		= MSG_RS_LIBRARY + 1;

// Margin control selection: wParam, lParam from mouse messages
const unsigned int UM_STARTSELECTLINE	= MSG_RS_LIBRARY + 2;
const unsigned int UM_SELECTLINE		= MSG_RS_LIBRARY + 3;
const unsigned int UM_ENDSELECTLINE		= MSG_RS_LIBRARY + 4;

// Folder watcher sends this message to its notify sink window
// wParam = 0 - change report; 1 - folder is ready to be removed from the watch list
// lParam = char const * folder
const unsigned int UM_FOLDER_CHANGE		= MSG_RS_LIBRARY + 5;

// Inter process communication message
// wParam = registered message send
// lParam = handle to global memory buffer
const unsigned int UM_INTERPROCESS_PACKAGE = MSG_RS_LIBRARY + 6;

// Last message used by RS LIBRARY
const unsigned int UM_RS_LIBRARY_LAST	= MSG_RS_LIBRARY + 6;

#endif
