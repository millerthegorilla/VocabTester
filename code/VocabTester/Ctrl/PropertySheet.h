#if !defined (PROPERTYSHEET_H)
#define PROPERTYSHEET_H
// ----------------------------------
// (c) Reliable Software, 1999 - 2005
// ----------------------------------

#include <Win/Controller.h>
#include <Win/Notification.h>

#include <stack>

class NamedValues;

namespace Win
{
	class Exception;
	class CritSection;
}

namespace Notify
{
	class Handler;
}

namespace PropPage
{
	class Controller;

	class Sheet
	{
	public:
		Sheet (Win::Dow::Handle win, std::string const & caption); 
		Sheet (Win::Instance hInst, std::string const & caption);

		void SetNoContextHelp () { _header.SetNoContextHelp (); }
		void SetNoApplyButton () { _header.SetNoApplyButton (); }
		void SetWizardStyle () { _header.SetWizardStyle (); }
		void SetStartPage (unsigned long startPage) { _header.SetStartPage (startPage); }

		void AddPage (int dlgResource, Controller & ctrl,
					  bool hasHelp = true, char const * title = 0);	//REVISIT: remove this method once wizard controlles are fixed
		void AddPage (PropPage::Controller const & ctrl, std::string const & title);

		// Returns true when user ended with the OK button
		bool Display (Win::CritSection * critSect = 0);

	protected:
		class Header;

		class Page : public PROPSHEETPAGE
		{
		public:
			Page (PropPage::Controller const & ctrl, Header const & header);

			void SetTitle (std::string const & title)
			{
				dwFlags |= PSP_USETITLE;
				pszTitle = title.c_str ();
			}
			void SetNoApplyButton () { dwFlags |= PSH_NOAPPLYNOW; }
			void SetHasHelp () { dwFlags |= PSP_HASHELP; }

			long GetResourceId () const
			{
				Assert ((dwFlags & PSP_DLGINDIRECT) == 0);
				return reinterpret_cast<long>(pszTemplate);
			}
		};

		class Header : public PROPSHEETHEADER
		{
		public:
			Header (Win::Dow::Handle win, std::string const & caption); 
			Header (Win::Instance hInst, std::string const & caption);

			void SetNoContextHelp ();
			void SetNoApplyButton () { dwFlags |= PSH_NOAPPLYNOW; }
			void SetWizardStyle () { dwFlags |= PSH_WIZARD; }
			void SetStartPage (unsigned long startPage) { nStartPage = startPage; }
			void Attach (std::vector<PROPSHEETPAGE> const & pages)
			{
				nPages = pages.size ();
				ppsp = &pages [0];
			}

			bool IsWizard () const { return (dwFlags & PSH_WIZARD) != 0; }
			bool IsModeless () const { return (dwFlags & PSH_MODELESS) != 0; }
			bool NoApplyButton () const { return (dwFlags & PSH_NOAPPLYNOW) != 0; }

			Win::Instance GetInstance () const { return hInstance; }
			char const * GetCaption () const { return pszCaption; }
			unsigned long GetStartPage () const { return nStartPage; }

		private:
			void Init (std::string const & caption);
			std::string _caption;
		};

	protected:
		void Init (Win::Dow::Handle win, std::string const & caption);

	protected:
		Header						_header;
		std::vector<PROPSHEETPAGE>	_pages;
		std::list<std::string>		_titles; // list items are not reallocated!
	};

	class Navigator
	{
	public:
		virtual void Next (long result)
		{
			_pageStack.push (result);
		}
		virtual long Prev ()
		{
			Assert (!_pageStack.empty ());
			_pageStack.pop ();
			Assert (!_pageStack.empty ());
			return _pageStack.top ();
		}
	protected:
		std::stack<long> _pageStack;
	};

	class Wizard : public PropPage::Sheet, public PropPage::Navigator
	{
	public:
		Wizard (Win::Dow::Handle win, std::string const & caption)
			: Sheet (win, caption)
		{
			SetWizardStyle ();
			SetNoApplyButton ();
		}
		Wizard (Win::Instance hInst, std::string const & caption)
			: Sheet (hInst, caption)
		{
			SetWizardStyle ();
			SetNoApplyButton ();
		}

		// Returns true when user ended with OK button
		bool Run (Win::CritSection * critSect = 0);
	};
};

namespace Notify
{
	// generic Windows WM_NOTIFY handler for property page
	class PageHandler : public Notify::Handler
	{
	public:
		// Revisit: use actual ID of the control
		PageHandler () : Notify::Handler (-1) {}
		virtual void OnSetActive (long & result) throw (Win::Exception)	{}
		virtual void OnKillActive (long & result) throw (Win::Exception) {}
		virtual void OnNext (long & result) throw (Win::Exception) {}
		virtual void OnPrev (long & result) throw (Win::Exception) {}
		virtual void OnFinish (long & result) throw (Win::Exception) {}
		virtual void OnCancel (long & result) throw (Win::Exception) {}
		virtual void OnApply (long & result) throw (Win::Exception) {}
		virtual void OnReset () throw (Win::Exception) {}
		virtual void OnHelp () const throw (Win::Exception) {}
	protected:
		bool OnNotify (NMHDR * hdr, long & result) throw (Win::Exception);
	};

	class WizardPageHandler : public Notify::PageHandler
	{
	public:
		WizardPageHandler (PropPage::Navigator & navi)
			: _navigator (navi)
		{}
		void OnPrev (long & result) throw (Win::Exception)
		{
			result = -1;
			if (GoPrevious ())
				result = _navigator.Prev ();
		}
		void OnNext (long & result) throw (Win::Exception)
		{
			result = -1;
			if (GoNext (result))
			{
				Assert (result != -1);
				_navigator.Next (result);
			}
		}
	protected:
		virtual bool GoNext (long & nextPage) = 0;
		virtual bool GoPrevious () = 0;
	protected:
		PropPage::Navigator & _navigator;
	};
}

namespace PropPage
{
	BOOL CALLBACK Procedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);

	class Controller: public Win::Controller
	{
		friend BOOL CALLBACK Procedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		Controller (int pageId, bool supportsContextHelp)
			: _pageId (pageId),
			  _supportsContextHelp (supportsContextHelp)
		{}

		int GetPageId () const { return _pageId; }
		bool SupportsContextHelp () const { return _supportsContextHelp; }

	public:
		enum Button
		{
			Finish = PSWIZB_FINISH,
			Apply = PSBTN_APPLYNOW,
			Back = PSBTN_BACK,
			Cancel = PSBTN_CANCEL,
			Help = PSBTN_HELP,
			Ok = PSBTN_OK,
			Next = PSBTN_NEXT
		};

	protected:
		void CancelToClose ()
		{
			::PropSheet_CancelToClose (_h.GetParent ().ToNative ());
		}

		void PressButton (Controller::Button button)
		{
			::PropSheet_PressButton (_h.GetParent ().ToNative (), button); 
		}

	private:
		int		_pageId;
		bool	_supportsContextHelp;
	};

	// specializations of PropPage::Controller
	class SheetController : public PropPage::Controller, public Notify::PageHandler
	{
	public:
		SheetController (int pageId, bool supportsContextHelp = false)
			: PropPage::Controller (pageId, supportsContextHelp)
		{}

		Notify::Handler * GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw () 
		{ 
			return this; 
		}

		virtual void GetDataFrom (NamedValues const * source) { /* Default implementation does nothing */ }
	};

	class WizardController : public PropPage::Controller, public Notify::WizardPageHandler
	{
	public:
		WizardController (PropPage::Navigator & navi)
			: PropPage::Controller (-1, false),	// REVISIT: wizard page controllers need to take page id and context help flag
			  Notify::WizardPageHandler (navi)
		{}
		Notify::Handler * GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw () 
		{ 
			return this; 
		}

		enum Buttons
		{
			Next = PSWIZB_NEXT,
			NextBack = PSWIZB_NEXT | PSWIZB_BACK,
			BackFinish = PSWIZB_BACK | PSWIZB_FINISH,
			Finish = PSWIZB_FINISH,
		};

		void SetButtons (WizardController::Buttons buttons) const
		{ 
			::PropSheet_SetWizButtons (_h.GetParent ().ToNative (), buttons); 
		}
	};

	class CtrlSet
	{
	private:
		struct CtrlInfo
		{
			CtrlInfo ()
				: _ctrl (0)
			{}

			CtrlInfo (SheetController * ctrl, std::string const & pageCaption)
				: _ctrl (ctrl),
				  _pageCaption (pageCaption)
			{}

			SheetController *	_ctrl;
			std::string			_pageCaption;
		};

	public:
		CtrlSet (std::string const & caption)
			: _caption (caption)
		{}

		void AddCtrl (SheetController & ctrl, std::string const & pageCaption)
		{
			CtrlInfo ctrlInfo (&ctrl, pageCaption);
			_controllers.push_back (ctrlInfo);
		}

		std::string const & GetCaption () const { return _caption; }
		virtual int GetStartPage () const { return 0; }
		virtual bool IsValidData () const { return false; }

		virtual bool GetDataFrom (NamedValues const * source)
		{
			for (Sequencer seq (*this); !seq.AtEnd (); seq.Advance ())
				seq.GetPageCtrl ().GetDataFrom (source);

			return IsValidData ();
		}

		class Sequencer
		{
		public:
			Sequencer (CtrlSet const & ctrlSet)
				: _cur (ctrlSet._controllers.begin ()),
				  _end (ctrlSet._controllers.end ())
			{}

			bool AtEnd () const { return _cur == _end; }
			void Advance () { ++_cur; }

			PropPage::SheetController & GetPageCtrl () { return *_cur->_ctrl; }
			std::string const & GetPageCaption () const { return _cur->_pageCaption; }

		private:
			std::vector<CtrlInfo>::const_iterator	_cur;
			std::vector<CtrlInfo>::const_iterator	_end;
		};

		friend class Sequencer;

	private:
		std::string				_caption;
		std::vector<CtrlInfo>	_controllers;
	};
}

#endif
