#if !defined CONTROL_H
#define CONTROL_H
//------------------------------------
//  control.h
//  (c) Reliable Software, 1996
//------------------------------------

#include "..\Library\Window.h"
#include "..\Library\Controller.h"
#include "..\Library\Splitter.h"
#include "SplitBar.h"

class WinController : public Win::Controller
{

public:
    WinController (HWND hwnd);
    ~WinController ();
    void Size (int cx, int cy);
	void MoveSplitter (int x);
	const int GetSplitterId () const { return _vSplitter->GetId();}
private:

	enum { splitWidth = 8 };	// width of splitter

    // User Interface
	Win::Dow        _hwnd;          //Main controller window
	Win::Dow		_leftWin;
	Win::Dow	    _rightWin;
	Win::VSplitterBar *_vSplitter;
	Win::VSplitterBarMaker  _vBarMaker;
	splitterNotifier  _splitNotifier;
	int				_cx;
	int				_cy;
	int				_splitRatio;	// in per cent
};

#endif
