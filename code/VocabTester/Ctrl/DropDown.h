#if !defined (DROPDOWN_H)
#define DROPDOWN_H
//-------------------------------------------
//  DropDown.h
//  (c) Reliable Software, 1998 -- 2002
//-------------------------------------------

#include <Ctrl/ComboBox.h>

namespace Win
{
	// This is combo box with subclassed edit control
	// Revisit: use subcontroller
	class DropDown : public Win::ComboBox
	{
	public:
		// parent receives child messages, canvas is where the child paints itself
		DropDown (Win::Dow::Handle parent, int id, Win::Dow::Handle canvas);
		~DropDown ();

		void OnEnter ();
		bool OnNotify (int notifyCode, bool & isSelection);
		static bool GotFocus (int notifyCode) { return notifyCode == CBN_SETFOCUS; }
		static bool LostFocus (int notifyCode) { return notifyCode == CBN_KILLFOCUS; }
		LRESULT CallOrgWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			return ::CallWindowProc (_orgEditWndProc, hwnd, msg, wParam, lParam);
		}

	private:
		static BOOL CALLBACK FindEditWindow (HWND hwnd, LPARAM lParam);
		static LRESULT CALLBACK DropDownWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		Win::Dow::Handle	_parent;
		Win::Dow::Handle	_edit;
		int					_id;
		WNDPROC				_orgEditWndProc;
		bool				_dropDownListClosed;
		bool				_selectionValid;
	};
}

#endif
