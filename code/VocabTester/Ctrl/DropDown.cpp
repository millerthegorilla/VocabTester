//-------------------------------------------
//  DropDown.cpp
//  (c) Reliable Software, 1998-2002
//-------------------------------------------

#include <WinLibBase.h>
#include "DropDown.h"

using namespace Win;

// parent receives child messages, canvas is where the child paints itself
DropDown::DropDown (Win::Dow::Handle parent, int id, Win::Dow::Handle canvas)
	: _parent (parent),
	  _edit (0),
	  _id (id),
	  _dropDownListClosed (true),
	  _selectionValid (false)
{
	ComboBoxMaker comboMaker (parent, id, ComboBox::Style::DropDown);
	comboMaker.Style () << Win::Style::Ex::AddClientEdge;
	Reset (comboMaker.Create (canvas));
	// Get the combobox edit window handle
	::EnumChildWindows (ToNative (), DropDown::FindEditWindow, reinterpret_cast<LPARAM>(&_edit));
	if (_edit.IsNull ())
		throw Win::Exception ("Cannot create drop down control");
	// Change the window procedure for edit control to
	// DropDownWndProc (subclass edit window)
	_orgEditWndProc = _edit.GetLong<WNDPROC> (GWL_WNDPROC);
	_edit.SetLong<WNDPROC> (DropDown::DropDownWndProc, GWL_WNDPROC);
	_edit.SetLong<DropDown *> (this);
}

DropDown::~DropDown ()
{
	// Stop subclassing combobox edit control
	_edit.SetLong<WNDPROC> (_orgEditWndProc, GWL_WNDPROC);
	_edit.SetLong<DropDown *> (0);
}

void DropDown::OnEnter ()
{
	if (_selectionValid)
		return;		// ENTER on the drop down list
	// ENTER in the edit control -- translate to CBN_SELENDOK if necessary
	if (GetEditTextLength () > 0)
	{
		// Notify parent about New selection from
		// the edit control.
		_parent.PostMsg (WM_COMMAND, MAKEWPARAM (_id, CBN_SELENDOK), reinterpret_cast<LPARAM>(ToNative ()));
	}
}

// Returns true if selection is ready for processing
bool DropDown::OnNotify (int notifyCode, bool & selectionFromDropDownList)
{
	selectionFromDropDownList = false;
	switch (notifyCode)
	{
	case CBN_DROPDOWN:
		_dropDownListClosed = false;
		break;
	case CBN_EDITCHANGE:
		_selectionValid = false;	// User types text in the edit box
		break;
	case CBN_SELCHANGE:
		if (_dropDownListClosed)
		{
			// User have selected item from the drop down list by clicking on it
			selectionFromDropDownList = true;
			return true;	// Process selection
		}
		else
		{
			_selectionValid = true;	// User browses drop down list using keyboard
		}
		break;
	case CBN_SELENDOK:
		// Selection process ends -- by mouse click or by keyboard ENTER
		if (_selectionValid || !_dropDownListClosed)
		{
			// Display selected item in the edit window
			int selIdx = GetSelectionIdx ();
			std::string selection = RetrieveText (selIdx);
			SetEditText (selection.c_str ());
		}
		else
		{
			// Process text in the edit box - send by subclased edit control
			return true;
		}
		break;
	case CBN_SELENDCANCEL:
		_selectionValid = false;
		break;
	case CBN_CLOSEUP:
		_dropDownListClosed = true;
		selectionFromDropDownList = _selectionValid;
		return _selectionValid;
	}
	return false;
}

BOOL CALLBACK DropDown::FindEditWindow (HWND hwnd, LPARAM lParam)
{
	char className [32];
	if (::GetClassName (hwnd, className, sizeof (className)) != 0)
	{
		if (stricmp (className, "EDIT") == 0)
		{
			HWND * pEditHwnd = reinterpret_cast<HWND *>(lParam);
			*pEditHwnd = hwnd;
			// Stop EnumChildwindows
			return FALSE;
		}
	}
	// Continue EnumChildWindows
	return TRUE;
}

LRESULT CALLBACK DropDown::DropDownWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win::Dow::Handle win (hwnd);
	DropDown * pDropDown = win.GetLong<DropDown *> ();
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			pDropDown->OnEnter ();
		}
		break;
	}
	return pDropDown->CallOrgWndProc (hwnd, msg, wParam, lParam);
}
