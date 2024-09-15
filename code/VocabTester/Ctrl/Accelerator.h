#if !defined ACCELERATOR_H
#define ACCELERATOR_H

//-----------------------------------------
// (c) Reliable Software 1997, 98, 99, 2000
//-----------------------------------------

#include <Ctrl/Command.h>
#include <Win/Handles.h>

namespace Accel
{
	class Handle: public Win::Handle<HACCEL>
	{
	public:
		Handle (HACCEL h = 0) : Win::Handle<HACCEL> (h) {}
		Handle (Win::Instance hInst, int id)
			: Win::Handle<HACCEL> (::LoadAccelerators (hInst, MAKEINTRESOURCE (id)))
		{
			if (IsNull ())
				throw Win::Exception ("Internal error: Cannot load accelerator table");
		}		
	};

	class Item
	{
	public:
		BYTE    flags;
		WORD    key;
		char const * cmdName;
	};

	// To activate an accelerator, call MessagePrepro::SetKbdAccelerator
	// and process OnCommand in the controller
	class Maker
	{
	public:
		Maker (Accel::Item const * templ, Cmd::Vector & cmdVector);
		AutoHandle Create ();
	private:
		std::vector<ACCEL> _accelTable;
	};

	enum
	{
		Plain =		FVIRTKEY | FNOINVERT,		// The key itself
		WithAlt =	Plain | FALT,				// ALT + key
		WithCtrl =	Plain | FCONTROL,			// Ctrl + key
		WithShift =	Plain | FSHIFT,				// Shift + key
		WithCtrlAndAlt = WithCtrl | FALT,		// Ctrl + Alt + key
		WithCtrlAndShift = WithCtrl | FSHIFT,	// Ctrl + Shift + key
	};
};

#endif
