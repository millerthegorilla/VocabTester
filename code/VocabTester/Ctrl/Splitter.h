#if !defined (SPLITTER_H)
#define SPLITTER_H
//
// Reliable Software (c) 1998
//

#include <Graph/Pen.h>

namespace Splitter
{
	// Warning: If the parent of the Splitter has the WS_CLIPCHILDREN style
	// the dragging of the Splitter will be invisible
	void RegisterClass (Win::Instance hInst);
	void MakeWindow (Win::Dow::Handle & hwndSplitter /* out */, Win::Dow::Handle hwndParent, int childId);
};

class SplitController
{
friend LRESULT CALLBACK WndProcSplitter (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	SplitController (Win::Dow::Handle hwnd, CREATESTRUCT * pCreat);

    Win::Dow::Handle Hwnd () const { return _hwnd; }
	void Size (int cx, int cy) { _cx = cx; _cy = cy; }
	void Paint ();

    void LButtonDown (POINTS pt);
    void LButtonUp (POINTS pt);
    void LButtonDrag (POINTS pt);
	void CaptureChanged ();
private:
	Win::Dow::Handle	_hwnd;
	Win::Dow::Handle	_hwndParent;
	Win::Style			_style;

	int			_cx;
	int			_cy;

	// just because user waves over splitter with mouse button down doesn't mean they're dragging it;
	// mouse down may have started in another control!!!
	bool		_dragging; 
	
	int			_dragStart;
	int			_dragX;
	int			_dragY;
	Pen::Pens3d	_pens;
};

#endif
