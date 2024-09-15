#if !defined (CONTROLLER_H)
#define CONTROLLER_H
//-----------------------------------
// (c) Reliable Software 2000 -- 2005
//-----------------------------------

#include <Win/Procedure.h>
#include <Ctrl/Menu.h>
#include <Win/Utility.h>

namespace Notify { class Handler; }
namespace Keyboard { class Handler; }
namespace Win
{
	class Message;
	class UserMessage;
	class FileDrop;
}
namespace OwnerDraw
{
	class Item;
	class Handler;
}
namespace Property 
{
	BOOL CALLBACK SheetProcedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);
}
namespace Dialog
{
	BOOL CALLBACK Procedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);
}

namespace Win
{
    class Controller
    {
        friend LRESULT CALLBACK Win::Procedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        friend LRESULT CALLBACK Win::SubProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		friend BOOL CALLBACK Dialog::Procedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);
		friend BOOL CALLBACK Property::SheetProcedure (HWND win, UINT message, WPARAM wParam, LPARAM lParam);
		friend class Maker;

	protected:
		Controller ()
			: _h (0)
		{}
		virtual ~Controller ()
		{}
		void SetWindowHandle (Win::Dow::Handle win) { _h = win; }
		virtual bool MustDestroy () throw ()
			{ return false; }

		// For internal use only
		static void Attach (Win::Dow::Handle win, Controller * ctrl);
		bool Dispatch (UINT message, WPARAM wParam, LPARAM lParam, LRESULT & result) throw ();
	public:
		Win::Dow::Handle GetWindow () const { return _h; }
		virtual Notify::Handler * GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw ()
			{ return 0; }
		virtual Keyboard::Handler * GetKeyboardHandler () throw ()
			{ return 0; }
		virtual bool OnCreate (Win::CreateData const * create, bool & success) throw ()
			{ return false; }
		virtual bool OnInitDialog () throw (Win::Exception)
			{ return false; }
        virtual bool OnDestroy () throw ()
			{ return false; }
		virtual bool OnShutdown (bool isEndOfSession, bool isLoggingOff) throw () { return false; }
		virtual bool OnActivateApp (unsigned long prevThreadId) throw ()
			{ return false; }
		virtual bool OnDeactivateApp (unsigned long nextThreadId) throw ()
			{ return false; }
		virtual bool OnActivate (bool isClickActivate, bool isMinimized, Win::Dow::Handle prevWnd) throw ()
			{ return false; }
		virtual bool OnDeactivate (bool isMinimized, Win::Dow::Handle nextWnd) throw ()
			{ return false; }
		virtual bool OnMouseActivate (Win::HitTest hitTest, Win::MouseActiveAction & activate) throw ()
			{ return false; }
		virtual bool OnFocus (Win::Dow::Handle winPrev) throw ()
			{ return false; }
		virtual bool OnKillFocus (Win::Dow::Handle winNext) throw ()
			{ return false; }
		virtual bool OnEnable() throw ()
			{ return false; }
		virtual bool OnDisable() throw ()
			{ return false; }
		virtual bool OnSize (int width, int height, int flag) throw ()
			{ return false; }
		virtual bool OnClose () throw ()
			{ return false; }
		virtual bool OnEraseBkgnd(Win::Canvas canvas) throw()
			{ return false; }
		virtual bool OnPaint () throw ()
			{ return false; }
		virtual bool OnShowWindow (bool show) throw ()
			{ return false; }			
		virtual bool OnSettingChange (Win::SystemWideFlags flags, char const * sectionName) throw ()
			{ return false; }
		virtual bool OnVScroll (int code, int pos) throw ()
			{ return false; }
		virtual bool OnHScroll (int code, int pos) throw ()
			{ return false; }
		// Commands
		virtual bool OnCommand (int cmdId, bool isAccel) throw ()
			{ return false; }
		// Controls
		virtual bool OnControl (Win::Dow::Handle control, int id, int notifyCode) throw ()
			{ return false; }
		// Standard dialog buttons
		virtual bool OnApply () throw ()
			{ return false; }
		virtual bool OnCancel () throw ()
			{ return false; }
		virtual bool OnHelp () throw ()
			{ return false; }

		virtual bool OnInitPopup (Menu::Handle menu, int pos) throw ()
			{ return false; }
		virtual bool OnExitMenuLoop (bool isContextMenu) throw ()
			{ return false; }
		virtual bool OnInitSystemPopup (Menu::Handle menu, int pos) throw ()
			{ return false; }
		virtual bool OnPopupSelect (int idx, Menu::State state, Menu::Handle menu) throw ()
			{ return false; }
		virtual bool OnSysMenuSelect (int id, Menu::State state, Menu::Handle menu) throw ()
			{ return false; }
		virtual bool OnMenuSelect (int id, Menu::State state, Menu::Handle menu) throw ()
			{ return false; }
		virtual bool OnContextMenu (Win::Dow::Handle wnd, int xPos, int yPos) throw ()
			{ return false; }
		virtual bool OnTimer (int id) throw ()
			{ return false; }
		virtual bool OnDialogIdle(Win::Dow::Handle dlg) throw()
			{ return false; }
		// Mouse
		virtual bool OnMouseMove (int x, int y, Win::KeyState kState) throw ()
			{ return false; }
		virtual bool OnWheelMouse (int zDelta) throw ()
			{ return false; }
		virtual bool OnLButtonDown (int x, int y, Win::KeyState kState) throw ()
			{ return false; }
		virtual bool OnRButtonDown (int x, int y, Win::KeyState kState) throw ()
			{ return false; }
		virtual bool OnLButtonUp (int x, int y, Win::KeyState kState) throw ()
			{ return false; }
		virtual bool OnLButtonDblClick (int x, int y, Win::KeyState kState) throw ()
			{ return false; }
		virtual bool OnCaptureChanged (Win::Dow::Handle newCaptureWin) throw ()
			{ return false; }
		// Drop target
		virtual bool OnDropFiles (Win::FileDrop const & dropFile) throw ()
			{ return false; }
		// Keyboard
		virtual bool OnChar (int vKey, int flags) throw ()
			{ return false; }
		virtual bool OnCharToItem(int vKey, int nCaret, Win::Dow::Handle listbox) throw()
			{ return false; }
		virtual bool OnVKeyToItem(int vKey, int nCaret, Win::Dow::Handle listbox) throw()
			{ return false; }
		// DDE
		virtual bool OnDdeInitiate (Win::Dow::Handle client, std::string const & app, std::string const & topic) throw ()
			{ return false; }
		// User messages
		virtual bool OnUserMessage (UserMessage & msg) throw ()
			{ return false; }
		// Registered messages
		virtual bool OnRegisteredMessage (Message & msg) throw ()
			{ return false; }
		// Interprocess communication
		virtual bool OnInterprocessPackage (unsigned int msg, char const * package, unsigned int errCode, long & result) throw ()
			{ return false; }

		//	Specialized drawing
		virtual bool OnMenuDraw (OwnerDraw::Item & draw) throw ()
			{ return false; }
		virtual bool OnItemDraw (OwnerDraw::Item & draw, int ctrlId) throw ();
		void AddDrawHandler (OwnerDraw::Handler * handler);
		void RemoveDrawHandler (Win::Dow::Handle winParent, int ctrlId) throw ();
		// Revisit: use draw handlers to deal with these
		virtual bool OnMeasureListBoxItem(int idControl, int idx, unsigned& height) throw ()
			{ return false; }
		virtual bool OnPreDrawEdit(Win::Dow::Handle win, Win::Canvas canvas, Brush::Handle& hbr) throw ()
			{ return false; }
		virtual bool OnPreDrawStatic(Win::Dow::Handle win, Win::Canvas canvas, Brush::Handle& hbr) throw ()
			{ return false; }
		virtual bool OnPreDrawListBox(Win::Dow::Handle win, Win::Canvas canvas, Brush::Handle& hbr) throw ()
			{ return false; }

		virtual bool OnSetFont(Font::Handle font, bool fRedraw) throw ()
			{ return false; }
		virtual bool OnTrainingCard(int, int) throw()
			{ return false; }

	protected:
		Win::Dow::Handle	_h;
		std::list<OwnerDraw::Handler *>	_drawHandlers;
		typedef std::list<OwnerDraw::Handler *>::iterator HandlerIter;
    };

	class TopController : public Controller
	{
		friend class Maker;

	protected:
		bool MustDestroy () throw ()
			{ return true; }
	};

	class SubController : public Controller
	{
	public:
		SubController ()
			: _prevProc (0),
			  _prevController (0)
		{} 
		void Init (HWND h, ProcPtr prevProc, Controller * prevCtrl)
		{
			SetWindowHandle (h);
			_prevProc = prevProc;

			Assert(prevCtrl == 0);	//	subclassing of windows with controllers not yet supported
			_prevController = prevCtrl;
		}
		LRESULT CallPrevProc (UINT message, WPARAM wParam, LPARAM lParam)
		{
			return ::CallWindowProc (_prevProc, _h.ToNative (), message, wParam, lParam);
		}
		ProcPtr GetPrevProc ()
		{
			return _prevProc;
		}
		Controller * GetPrevController ()
		{
			return _prevController;
		}
	protected:
		ProcPtr			_prevProc;
		Controller *	_prevController;
	};
}

#endif
