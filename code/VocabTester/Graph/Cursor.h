#if !defined CURSOR_H
#define CURSOR_H
//------------------------------
// (c) Reliable Software 1997-00
//------------------------------
#include <Win/GdiHandles.h>

namespace Cursor
{
	class Shape : public Handle
	{
	public:
		Shape (HCURSOR cursor)
			: Handle (cursor)
		{}
		Shape (Win::Instance hInst, int id)
			: Handle (::LoadCursor (hInst, MAKEINTRESOURCE (id)))
		{}
	};

	class System : public Handle
	{
	public:
		System (char * cursorId)
			: Handle (::LoadCursor (0, cursorId))
		{
			if (IsNull ())
				throw Win::Exception ("Internal error: Cannot load cursor.");
		}
	};

	class Hourglass : public System
	{
	public:
		Hourglass () : System (IDC_WAIT) {}
	};

	class Arrow : public System
	{
	public:
		Arrow () : System (IDC_ARROW) {}
	};

	class Holder
	{
	public:
		Holder () : _oldCursor (0) {}
		Holder (Handle newCursor)
		{
			Set (newCursor);
		};
		~Holder ()
		{
			Restore ();
		}
		void Set (Handle cursor)
		{
			_oldCursor = ::SetCursor (cursor.ToNative ());
		}
		void Restore ()
		{
			::SetCursor (_oldCursor.ToNative ());
		}

	private:
		Handle _oldCursor;
	};
}

#endif
