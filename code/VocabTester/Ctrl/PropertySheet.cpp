// --------------------------------
// (c) Reliable Software, 1999-2005
// --------------------------------

#include <WinLibBase.h>
#include "PropertySheet.h"

#include <Sys/Dll.h>
#include <Sys/Synchro.h>
#include <Sys/SysVer.h>
#include <Ctrl/Output.h>

namespace PropPage
{
	BOOL CALLBACK Procedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;
		Win::Dow::Handle win (hwnd);
		Controller * ctrl = win.GetLong<Controller *> ();
		try
		{
			if (ctrl)
			{
				if (ctrl->Dispatch (message, wParam, lParam, result))
					return TRUE;
			}
			else if (message == WM_INITDIALOG)
			{
				PROPSHEETPAGE const * page = reinterpret_cast<PROPSHEETPAGE *> (lParam);
				ctrl = reinterpret_cast<PropPage::Controller *> (page->lParam);
				Controller::Attach (win, ctrl);
				if (ctrl->Dispatch (message, wParam, lParam, result))
					return TRUE;
			}
		}
		catch (Win::Exception e)
		{
			Out::Sink::DisplayException (e, win);
			// exit property sheet / wizard
			Win::Dow::Handle sheet (::GetParent (win.ToNative ()));
			::PropSheet_PressButton (sheet.ToNative (), PSBTN_CANCEL);
		}
		return FALSE;
	}

	Sheet::Header::Header (Win::Dow::Handle win, std::string const & caption)
	{
		Init (caption);
		hwndParent = win.ToNative ();
		hInstance = win.GetInstance ();
	}

	Sheet::Header::Header (Win::Instance hInst, std::string const & caption)
	{
		Init (caption);
		hInstance = hInst;
	}

	void Sheet::Header::SetNoContextHelp ()
	{
		SystemVersion sysVer;
		if (sysVer.IsWin95 () || (sysVer.IsWinNT () && sysVer.MajorVer () == 4))
		{
			//	Revisit: Emergency fix for Bug #711
			return;
		}
		dwFlags |= PSH_NOCONTEXTHELP;
	}

	void Sheet::Header::Init (std::string const & caption)
	{
		_caption = caption;
		::ZeroMemory (this, sizeof (PROPSHEETHEADER));

		Dll comctl32Dll ("comctl32.dll");
		DllVersion ver (comctl32Dll);
		if (ver.IsOk ())
		{
			if (ver.GetMajorVer () == 4 && ver.GetMinorVer () <= 70)
				dwSize  = PROPSHEETHEADER_V1_SIZE;
			else
				dwSize  = sizeof (PROPSHEETHEADER);
		}
		else
		{
			throw Win::Exception ("Cannot determine the Property Sheet control version");
		}
		dwFlags = PSH_PROPSHEETPAGE;
		pszCaption = _caption.c_str ();
	}

	Sheet::Page::Page (PropPage::Controller const & ctrl, Header const & header)
	{
		::ZeroMemory (this, sizeof (PROPSHEETPAGE));
		dwSize = sizeof (PROPSHEETPAGE);
		dwFlags = PSH_PROPSHEETPAGE;
		hInstance = header.GetInstance ();
		pfnDlgProc = PropPage::Procedure;
		pszTemplate = MAKEINTRESOURCE (ctrl.GetPageId ()); 
		lParam = reinterpret_cast<LPARAM>(&ctrl);

		if (ctrl.SupportsContextHelp ())
			SetHasHelp ();

		if (header.NoApplyButton ())
			SetNoApplyButton ();
	}

	Sheet::Sheet (Win::Dow::Handle win, std::string const & caption)
		: _header (win, caption)
	{}
	
	Sheet::Sheet (Win::Instance hInst, std::string const & caption)
		: _header (hInst, caption)
	{}

	// REVISIT: remove this memthod once wizard page controllers are fixed.		
	void Sheet::AddPage (int dlgResource, Controller & ctrl, bool hasHelp, char const * title)
	{
		PROPSHEETPAGE page;
		::ZeroMemory (&page, sizeof (PROPSHEETPAGE));
		page.dwSize = sizeof (PROPSHEETPAGE);
		page.dwFlags = PSH_PROPSHEETPAGE;
		page.hInstance = _header.GetInstance ();
		page.pfnDlgProc = PropPage::Procedure;
		page.pszTemplate = MAKEINTRESOURCE (dlgResource); 
		page.lParam = reinterpret_cast<LPARAM>(&ctrl);

		if (hasHelp)
			page.dwFlags |= PSP_HASHELP;

		if (title)
		{
			page.dwFlags |= PSP_USETITLE;
			page.pszTitle = title;
		}
		else if (_header.IsWizard ())
		{
			// For wizards use sheet caption if page doesn't specify its own title
			page.dwFlags |= PSP_USETITLE;
			page.pszTitle = _header.GetCaption ();
		}

		if (_header.NoApplyButton ())
			page.dwFlags |= PSH_NOAPPLYNOW;

		_pages.push_back (page);
	}

	void Sheet::AddPage (PropPage::Controller const & ctrl, std::string const & title)
	{
		Page page (ctrl, _header);

		if (!title.empty ())
		{
			// own the string
			_titles.push_back (title);
			page.SetTitle (_titles.back ());
		}
		else if (_header.IsWizard ())
		{
			// For wizards use sheet caption if page doesn't specify its own title
			page.SetTitle (_header.GetCaption ());
		}

		_pages.push_back (page);
	}

	bool Sheet::Display (Win::CritSection * critSect)
	{
		Assert (!_header.IsModeless ());
		_header.Attach (_pages);
		// Exit critical section while running property sheet
		Assert (Dbg::IsMainThread ());
		Win::UnlockPtr unlock (critSect);
		int result = ::PropertySheet (&_header);
		if (result == -1)
			throw Win::Exception ("Property Sheet creation failed.");
		return result > 0;	// result == 0 -- user ended with Cancel button
	}

	bool Wizard::Run (Win::CritSection * critSect)
	{ 
		PROPSHEETPAGE const & firstPage = _pages [_header.GetStartPage ()];
		_pageStack.push (reinterpret_cast<long>(firstPage.pszTemplate));
		return Display (critSect); 
	}
}

namespace Notify
{
	bool PageHandler::OnNotify (NMHDR * hdr, long & result) throw (Win::Exception)
	{
		// hdr->code
		// hdr->idFrom;
		// hdr->hwndFrom;
		switch (hdr->code)
		{
		case PSN_SETACTIVE:
			OnSetActive (result);
			return true;
		case PSN_KILLACTIVE:
			OnKillActive (result);
			return true;
		case PSN_WIZNEXT :
			OnNext (result);
			return true;
		case PSN_WIZBACK :
			OnPrev (result);
			return true;
		case PSN_WIZFINISH :
			OnFinish (result);
			return true;
		case PSN_QUERYCANCEL:
			OnCancel (result);
			return true;
		case PSN_APPLY:
			OnApply (result);
			return true;
		case PSN_RESET: // no return value
			OnReset ();
			return true;
		case PSN_HELP: // no return value
			OnHelp ();
			return true;
		};
		return false;
	}
}
