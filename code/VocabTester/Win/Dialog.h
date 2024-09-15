#if !defined DIALOG_H
#define DIALOG_H
//-------------------------------------
//  (c) Reliable Software, 1996 -- 2005
//-------------------------------------

#include <Win/Controller.h>
#include <Win/MsgLoop.h>
#include <Win/WinResource.h>

class NamedValues
{
public:
	virtual std::string GetValue (std::string const & name) const = 0;
};

namespace Notify { class Handler; }
namespace Help { class Engine; }

namespace Dialog
{
	BOOL CALLBACK Procedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);

	class Handle: public Win::Dow::Handle
	{
	public:
		Handle (Win::Dow::Handle win = 0) 
			: Win::Dow::Handle (win) 
		{}
		void Reposition ()
		{
			Win::Message msg (DM_REPOSITION);
			SendMsg (msg);
		}
		// takes rect in dlg template units and maps to pixels
		void MapRectangle (Win::Rect & rect);
	};

	class Controller: public Win::Controller
	{
		friend BOOL CALLBACK Procedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);
	public:

		// Alternative dialog input (alternative to GUI)
		virtual bool GetDataFrom (NamedValues const * source)
			{ return false; }

		// Standard dialog button methods
		virtual bool OnApply () throw () = 0;
		virtual bool OnCancel () throw ()
		{
			EndCancel ();
			return true;
		}
		virtual bool OnHelp () throw ();

		// Dialog end methods
		virtual void EndOk () throw () = 0;
		virtual void EndCancel () throw () = 0;

		int GetDlgId () const { return _dlgId; }

		void AttachHelp (Help::Engine * helpEngine) throw () 
		{
			_helpEngine = helpEngine;
		}

	protected:
		Controller (int dlgId)
			: _helpEngine (0),
			  _dlgId (dlgId)
		{}
	private:
		Help::Engine *		_helpEngine;
		int					_dlgId;
	};

	//-------- User part ------------

	class Template
	{
		// This is the beginning of the extended dialog template
		struct Ex {
			WORD dlgVer;
			WORD signature;
			DWORD helpID;
			DWORD exStyle;
			DWORD style;
			WORD cDlgItems;
			short x;
			short y;
			short cx;
			short cy;
		};

	public:
		Template () : _data (0), _version (0) {}
		void Load (Win::Dow::Handle winParent, int dlgId)
		{
			Resource dlg1Res (winParent, dlgId, RT_DIALOG);
			_data = NULL; //dlg1Res.Lock<void> ();
			_version = ToEx ()->dlgVer;
			WORD signature = ToEx ()->signature;
			if (signature == 0xffff)
			{
				if (_version != 1)
				{
					throw Win::Exception ("Unknown version of dialog template");
				}
			}
			else
				_version = 0;
		}

		DLGTEMPLATE * ToNative () const { return static_cast<DLGTEMPLATE *> (_data); }
		int Width () const { return (_version == 1)? ToEx ()->cx: ToTmpl ()->cx; }
		int Height () const { return (_version == 1)? ToEx ()->cy: ToTmpl ()->cy; }
	private:
		DLGTEMPLATE const * ToTmpl () const { return static_cast<DLGTEMPLATE const *> (_data); }
		Template::Ex const * ToEx () const { return static_cast<Ex const *> (_data); }
	private:
		void *	_data;
		int		_version;
	};

	// Sizes and units
	inline int BaseUnitX () { return LOWORD (::GetDialogBaseUnits ()); }
	inline int BaseUnitY () { return HIWORD (::GetDialogBaseUnits ()); }
	inline int FrameThickX () { return ::GetSystemMetrics (SM_CXDLGFRAME); }
	inline int FrameThickY () { return ::GetSystemMetrics (SM_CYDLGFRAME); }
	inline int CaptionHeight () { return ::GetSystemMetrics(SM_CYCAPTION); }

	// User controller inherits from this
	class ModalController: public Controller
	{
	protected:
		ModalController (int dlgId)
			: Dialog::Controller (dlgId)
		{}
		void EndOk () throw ()
		{
			::EndDialog (GetWindow ().ToNative (), 1);
			Win::ClearError ();
		}
		void EndCancel () throw ()
		{
			::EndDialog (GetWindow ().ToNative (), 0);
			Win::ClearError ();
		}
	};

	// User controller inherits from this, adding specific data and methods.
	// Notice: ModelessController has to be kept around util
	// the user closes the dialog!
	// The controller provides the communication channel 
	// between the dialog and the main program.
	class ModelessController: public Controller
	{
	protected:
		ModelessController (Win::MessagePrepro & prepro, int dlgId) 
			: Dialog::Controller (dlgId),
			  _prepro (prepro)
		{}

		void EndOk () throw ()
		{
			Destroy ();
			Win::ClearError ();
		}
		void EndCancel () throw ()
		{
			Destroy ();
			Win::ClearError ();
		}
		void Destroy () throw () { ::DestroyWindow (GetWindow ().ToNative ()); }

	protected:
		Win::MessagePrepro & _prepro;
	};

	// User constructs this object and passes it user-defined controller
	class Modal
	{
	public:
		Modal (Win::Dow::Handle winParent, Win::Instance hInstance, ModalController & ctrl)
		{
			_result = ::DialogBoxParam (hInstance,
										MAKEINTRESOURCE (ctrl.GetDlgId ()),
										winParent.ToNative (),
										static_cast<DLGPROC>(Dialog::Procedure),
										reinterpret_cast<LPARAM>(&ctrl));
		}

		Modal (Win::Dow::Handle winParent, ModalController & ctrl)
		{
			_result = ::DialogBoxParam (winParent.GetInstance (),
										MAKEINTRESOURCE (ctrl.GetDlgId ()),
										winParent.ToNative (),
										static_cast<DLGPROC>(Dialog::Procedure),
										reinterpret_cast<LPARAM>(&ctrl));
		}

		Modal (Win::Dow::Handle winParent,
			   Win::Instance hInstance, 
			   Dialog::Template const & templ, 
			   ModalController & ctrl)
		{
			_result = ::DialogBoxIndirectParam (hInstance,
												templ.ToNative (),
												winParent.ToNative (),
												static_cast<DLGPROC>(Dialog::Procedure),
												reinterpret_cast<LPARAM>(&ctrl));
		}

		bool IsOK () { return (_result == -1)? false: _result != 0; }

	private:
		int _result;
	};

	// User constructs this object and passes it user-defined controller
	class ModelessMaker
	{
	public:
		ModelessMaker (ModelessController & ctrl)
			: _ctrl (ctrl)
		{}
		// Call only if you don't want to use winParent's instance
		void SetInstance (Win::Instance instance)
		{
			_instance = instance;
		}
		Dialog::Handle Create (Win::Dow::Handle winParent)
		{
			if (_instance.IsNull ())
				_instance = winParent.GetInstance ();
			Win::Dow::Handle h = ::CreateDialogParam (_instance,
											 MAKEINTRESOURCE (_ctrl.GetDlgId ()),
											 winParent.ToNative (),
											 static_cast<DLGPROC>(Dialog::Procedure),
											 reinterpret_cast<LPARAM>(&_ctrl));
			if (h.IsNull ())
				throw Win::Exception ("Internal error: Cannot create modeless dialog.");
			return h;
		}
		Dialog::Handle Create (Win::Dow::Handle winParent, Dialog::Template const & templ)
		{
			if (_instance.IsNull ())
				_instance = winParent.GetInstance ();
			Win::Dow::Handle h = ::CreateDialogIndirectParam (_instance,
											 templ.ToNative (),
											 winParent.ToNative (),
											 static_cast<DLGPROC>(Dialog::Procedure),
											 reinterpret_cast<LPARAM>(&_ctrl));
			if (h.IsNull ())
				throw Win::Exception ("Internal error: Cannot create modeless dialog.");
			return h;
		}
	private:
		ModelessController &	_ctrl;
		Win::Instance			_instance;
	};
}

#endif
