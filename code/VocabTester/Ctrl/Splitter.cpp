//
// Reliable Software (c) 1998, 99, 2000
//

#include <WinLibBase.h>
#include "Splitter.h"
#include "Messages.h"

#include <Win/Message.h>
#include <Win/WinMaker.h>
#include <Win/WinClass.h>
#include <Graph/Canvas.h>
#include <Graph/CanvTools.h>
#include <Graph/Font.h>

LRESULT CALLBACK WndProcSplitter (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void Splitter::RegisterClass (Win::Instance hInst)
{
	Win::Class::Maker splitterClass ("RsSplitterClass", hInst, WndProcSplitter);
	splitterClass.SetSysCursor (IDC_SIZEWE);
	splitterClass.SetBgSysColor (COLOR_3DFACE);
	splitterClass.Register ();
}

void Splitter::MakeWindow (Win::Dow::Handle & hwndSplitter, Win::Dow::Handle hwndParent, int childId)
{
	Win::ChildMaker splitterMaker ("RsSplitterClass", hwndParent, childId);
	hwndSplitter = splitterMaker.Create ();
	hwndSplitter.Show ();
}


LRESULT CALLBACK WndProcSplitter (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Win::Dow::Handle win (hwnd);
    SplitController * pCtrl = win.GetLong<SplitController *> ();
    switch (message)
    {
    case WM_CREATE:
        try
        {
            pCtrl = new SplitController (hwnd, reinterpret_cast<CREATESTRUCT *>(lParam));
            win.SetLong<SplitController *> (pCtrl);
        }
        catch (...)
        {
            return -1;
        }
        return 0;
	case WM_SIZE:
		pCtrl->Size (LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_PAINT:
		pCtrl->Paint ();
		return 0;
    case WM_LBUTTONDOWN:
        pCtrl->LButtonDown (MAKEPOINTS (lParam));
        return 0;
    case WM_LBUTTONUP:
        pCtrl->LButtonUp (MAKEPOINTS (lParam));
        return 0;
    case WM_MOUSEMOVE:
        /*if (wParam & MK_LBUTTON)*/
        if (pCtrl->_dragging)
            pCtrl->LButtonDrag (MAKEPOINTS (lParam));
        return 0;
	case WM_CAPTURECHANGED:
		pCtrl->CaptureChanged ();
		return 0;
	// Revisit: Abort drag when user presses ESC
    case WM_DESTROY:
        win.SetLong<SplitController *> (0);
        delete pCtrl;
        return 0;
	}
    return DefWindowProc (hwnd, message, wParam, lParam);
}

SplitController::SplitController (Win::Dow::Handle hwnd, CREATESTRUCT * pCreat)
    : _hwnd (hwnd), 
	  _hwndParent (pCreat->hwndParent),
	  _dragging (false)
{}

void SplitController::Paint ()
{
	Win::PaintCanvas canvas (_hwnd);
	{
		Pen::Holder pen (canvas, _pens.Light ());
		canvas.Line (0, 0, 0, _cy - 1);
	}
	{
		Pen::Holder pen (canvas, _pens.Hilight ());
		canvas.Line (1, 0, 1, _cy - 1);
	}
	{
		Pen::Holder pen (canvas, _pens.Shadow ());
		canvas.Line (_cx - 2, 0, _cx - 2, _cy - 1);
	}
	{
		Pen::Holder pen (canvas, _pens.DkShadow ());
		canvas.Line (_cx - 1, 0, _cx - 1, _cy - 1);
	}
}

void SplitController::LButtonDown (POINTS pt)
{
	_style = _hwndParent.GetStyle ();
	Win::Style tmpStyle (_style);
	tmpStyle.DontClipChildren ();
	_hwndParent.SetStyle (tmpStyle);

    _hwnd.CaptureMouse ();
	// Find x offset of splitter
	// with respect to parent client area
	Win::Point ptOrg;
	_hwndParent.ClientToScreen (ptOrg);
	int xParent = ptOrg.x;
	int yParent = ptOrg.y;
	ptOrg.x = 0;
	ptOrg.y = 0;
	_hwnd.ClientToScreen (ptOrg);
	int xChild = ptOrg.x;
	int yChild = ptOrg.y;

	_dragStart = xChild - xParent + _cx / 2 - pt.x;

	_dragX = _dragStart + pt.x;
	_dragY = yChild - yParent;
	// Draw a divider using XOR mode
	Win::UpdateCanvas canvas (_hwndParent);
	Win::ForegrMode mode (canvas, R2_NOTXORPEN);
	canvas.Line (_dragX, _dragY, _dragX, _dragY + _cy - 1);
	_dragging = true;
}

void SplitController::LButtonDrag (POINTS pt)
{
	// Erase previous divider and draw New one
	Win::UpdateCanvas canvas (_hwndParent);
	Win::ForegrMode mode (canvas, R2_NOTXORPEN);
	canvas.Line (_dragX, _dragY, _dragX, _dragY + _cy - 1);
	_dragX = _dragStart + pt.x;
	canvas.Line (_dragX, _dragY, _dragX, _dragY + _cy - 1);
}

void SplitController::LButtonUp (POINTS pt)
{
	// Calling ReleaseCapture will send us the WM_CAPTURECHANGED
    _hwnd.ReleaseMouse ();
	Win::UserMessage msg (UM_MOVESPLITTER, _dragStart + pt.x);
	_hwndParent.SendMsg (msg);
	_dragging = false;
}

void SplitController::CaptureChanged ()
{
	// We are losing capture
	// End drag selection -- for whatever reason
	// Erase previous divider
	Win::UpdateCanvas canvas (_hwndParent);
	Win::ForegrMode mode (canvas, R2_NOTXORPEN);
	canvas.Line (_dragX, _dragY, _dragX, _dragY + _cy - 1);
	_hwndParent.SetStyle (_style);
	_dragging = false;
}
