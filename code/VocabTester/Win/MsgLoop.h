#if !defined (MSGLOOP)
#define MSGLOOP
//------------------------------
// (c) Reliable Software 1998-03
//------------------------------
#include <Win/Message.h>
#include <Ctrl/Accelerator.h>

namespace Win
{
	class MessagePrepro
	{
	public:
		MessagePrepro ()
			: _breakMsg (WM_NULL)
		{}
		MessagePrepro (unsigned int breakMsg)
			: _breakMsg (breakMsg)
		{}
		MessagePrepro (Win::Message const & breakMsg)
			: _breakMsg (breakMsg.GetMsg ())
		{}
		void SetKbdAccelerator (Win::Dow::Handle hwndTop, Accel::Handle hAccel) 
		{
			_hwndAccel = hwndTop;
			_hAccel = hAccel;
		}
		void ResetKbdAccelerator () 
		{
			_hwndAccel.Reset ();
			_hAccel.Reset ();
		}
		// Use with modeless dialogs
		void SetDialogFilter (Win::Dow::Handle hwndDlg, 
								Win::Dow::Handle hwndDlgAccel = Win::Dow::Handle (), 
								Accel::Handle hDlgAccel = Accel::Handle ()) 
		{ 
			_hwndDlg = hwndDlg;
			_hwndDlgAccel = hwndDlgAccel;
			_hDlgAccel = hDlgAccel;
			//  use dialog accelerator , only if the target window for dialog accelerator is valid
			Assert (hDlgAccel.IsNull () || !hwndDlgAccel.IsNull ());
		}
		void ResetDialogFilter () 
		{
			_hwndDlg.Reset ();
			_hDlgAccel.Reset ();
			_hwndDlgAccel.Reset ();
		}
		int Pump ();
		int Pump (Win::Dow::Handle hwnd);
		int PumpHidden (Win::Dow::Handle hwnd);
		void PumpPeek ();
		void PumpPeek (Win::Dow::Handle hwnd);
		void PumpDialog ();

	private:
		MessagePrepro (MessagePrepro const & prepro);
		MessagePrepro const & operator= (MessagePrepro const & prepro);
		bool TranslateDialogMessage (MSG & msg);

	private:
		Win::Dow::Handle	_hwndAccel;		// target window for dialog accelerator
		Accel::Handle		_hAccel;		// main accellerator
		Win::Dow::Handle	_hwndDlg;		// dialog window
		Accel::Handle		_hDlgAccel;		// dialog accelerator		                        
		Win::Dow::Handle	_hwndDlgAccel;	// target window for dialog accelerator		
		unsigned int		_breakMsg;
	};

	class ModalMessagePrepro
	{
	public:
		ModalMessagePrepro (unsigned int breakMsg)
			: _breakMsg (breakMsg)
		{}
		ModalMessagePrepro (Win::Message const & breakMsg)
			: _breakMsg (breakMsg.GetMsg ())
		{}
		int Pump (Win::Dow::Handle hwnd);
		int Pump ();

	private:
		ModalMessagePrepro (ModalMessagePrepro const & prepro);
		ModalMessagePrepro const & operator= (ModalMessagePrepro const & prepro);

	private:
		unsigned int	_breakMsg;
	};
}

#endif
