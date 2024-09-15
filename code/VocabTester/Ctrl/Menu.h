#if !defined (WINLIB_MENU_H)
#define WINLIB_MENU_H
//-----------------------------------
// (c) Reliable Software 1998-03
//-----------------------------------
#include "Command.h"
#include <Bit.h>

namespace Menu
{
	class Handle;

	typedef Win::AutoHandle<Handle> AutoHandle;

	class Handle: public Win::Handle<HMENU>
	{
	public:
		Handle (HMENU hMenu)
			: Win::Handle<HMENU> (hMenu)
		{}

		Handle (Win::Dow::Handle win)
			: Win::Handle<HMENU> (::GetMenu (win.ToNative ()))
		{}

		void Attach (Win::Dow::Handle win)
		{
			if (!::SetMenu (win.ToNative (), ToNative ()))
				throw Win::Exception ("Internal error: Cannot activate menu.");
		}
    	static void Refresh (Win::Dow::Handle win)
    	{
        	DrawMenuBar (win.ToNative ());
    	}
		bool IsEmpty () const
		{
			return ::GetMenuItemCount (H ()) == 0;
		}
		void AddItem (int itemId, char const * item)
		{
			if (!::AppendMenu (ToNative (), MF_STRING, itemId, item))
				throw Win::Exception ("Internal error: Cannot add menu item.");
		}
		void AddSeparator ()
		{
			if (!::AppendMenu (ToNative (), MF_SEPARATOR, 0, 0))
				throw Win::Exception ("Internal error: Cannot add menu separator.");
		}
		void Clear ();

		void AddPopup (AutoHandle & popup, char const * item)
		{
			if (!::AppendMenu (H (), MF_POPUP, reinterpret_cast<unsigned int>(popup.release ()), item))
				throw Win::Exception ("Internal error: Cannot add popup menu.");
		}
		bool IsSubMenu (Handle hMenu, int barItemNo)
		{
			Handle subMenu = ::GetSubMenu (H (), barItemNo);
			return hMenu == subMenu;
		}
		// Item manipulation
		void Enable (int id)
		{
			::EnableMenuItem (ToNative (), id, MF_BYCOMMAND | MF_ENABLED);
		}
		void Disable (int id)
		{
			::EnableMenuItem (ToNative (), id, MF_BYCOMMAND | MF_GRAYED);
		}
		void Check (int id)
		{
			::CheckMenuItem (ToNative (), id, MF_BYCOMMAND | MF_CHECKED);
		}
		void UnCheck (int id)
		{
			::CheckMenuItem (ToNative (), id, MF_BYCOMMAND | MF_UNCHECKED);
		}
		// Whole menu manipulation
		void Enable ()
		{
			int count = ::GetMenuItemCount (ToNative ());
			for (int iItem = 0; iItem < count; ++iItem)
			{
				::EnableMenuItem (ToNative (), iItem, MF_BYPOSITION | MF_ENABLED);
			}
		}
		void Disable ()
		{
			int count = ::GetMenuItemCount (ToNative ());
			for (int iItem = 0; iItem < count; ++iItem)
			{
				::EnableMenuItem (ToNative (), iItem, MF_BYPOSITION | MF_GRAYED);
			}
		}
	};

	//-----------------------
	// Menu construction
	//-----------------------

	class Bar : public AutoHandle
	{
	public:
		Bar ()
			: AutoHandle (::CreateMenu ())
		{}
	};

	class Popup : public AutoHandle
	{
	public:
		Popup ()
			: AutoHandle (::CreatePopupMenu ())
		{}
	};

	class Tracker: public Popup
	{
	public:
		void TrackMenu (Win::Dow::Handle winOwner, int x, int y)
		{
			if (!IsEmpty ())
			{
				if (::TrackPopupMenuEx (H (),
									TPM_LEFTALIGN | TPM_RIGHTBUTTON,
									x, y,
									winOwner.ToNative (),
									0) == FALSE)
				{
//					revisit:  different Windows platforms return different errors
//						e.g. Win95 appears to return 0,
//							 Win98 & WinME return ERROR_INVALID_PARAMETER,
//							 undetermined what NT 4 and Win 2K return
//					if (::GetLastError () != ERROR_POPUP_ALREADY_ACTIVE)
//						Assert (!"Popup menu tracking failed");
					::SetLastError (0);
				}
			}
		}
	};

	//----------------------------
	// Templates for menu creation
	//----------------------------

	enum ItemKind
	{
		POP,
		CMD,
		SEPARATOR,
		END
	};

	class Item
	{
	public:
		ItemKind		kind;			// Menu item flags
		char const *	displayName;	// Menu item display string
		char const *	cmdName;		// Command name or menu bar item name
		Item const * 	popup;			// Menu popup definition
	};

	//----------------------
	// Drop Down Menu System
	// Builds the complete menu using a template
	//----------------------

	class DropDown
	{
	public:
		//	the DisplayNameSource class allows clients to dynamically change
		//	menu items (during calls to RefreshPopup)
		class DisplayNameSource
		{
		public:
			//	return an empty string to use the default display name
			virtual std::string GetItemDisplayName(char const *cmdName) = 0;
		};

		DropDown (Item const * templ, Cmd::Vector & cmdVector);
		void AttachToWindow (Win::Dow::Handle win);
		void SetDisplayNameSource(DisplayNameSource *pDisplayNameSource)
		{
			_pDisplayNameSource = pDisplayNameSource;
		}
		void RefreshPopup (Menu::Handle menu, int barItemNo) throw ();
		Item const * GetPopupTemplate (char const * name) const;
		Cmd::Vector & GetCommandVector () { return _cmdVector; } 
	private:
		Menu::AutoHandle CreatePopup (Menu::Item const * popupItems);

	private:
		Menu::Bar			_bar;
		int					_barItemCnt;
		Item const *		_template;
		Cmd::Vector &		_cmdVector;
		DisplayNameSource * _pDisplayNameSource;
	};

	class Context : public Tracker
	{
	public:
		//	the DisplayNameSource class allows clients to dynamically change
		//	menu items (during calls to RefreshPopup)
		class DisplayNameSource
		{
		public:
			//	return an empty string to use the default display name
			virtual std::string GetItemDisplayName(char const *cmdName) = 0;
		};
		Context (std::vector<Menu::Item const *> const & templ, Cmd::Vector & cmdVector);
		void RefreshPopup (Menu::Handle menu, int barItemNo) throw ();
		void CreateContext () throw ();
	private:
		Menu::AutoHandle CreatePopup (Menu::Item const * popupItems, Cmd::Vector & cmdVector);
		std::vector<Menu::Item const *>	const	&_templ;
		Cmd::Vector								&_cmdVector;
        DisplayNameSource						*_pDisplayNameSource;
	};

	class State : public BitField<DWORD>
	{
	public:
		State (int state)
			: BitField<DWORD>(state)
		{}

		bool IsBitmap () const { return IsSet (MF_BITMAP); }
		bool IsChecked () const { return IsSet (MF_CHECKED); }
		bool IsDisabled () const { return IsSet (MF_DISABLED); }
		bool IsGreyed () const { return IsSet (MF_GRAYED); }
		bool IsHilite () const { return IsSet (MF_HILITE); }
		bool IsMouseSelect () const { return IsSet (MF_MOUSESELECT); }
		bool IsOwnerDraw () const { return IsSet (MF_OWNERDRAW); }
		bool IsPopup () const { return IsSet (MF_POPUP); }
		bool IsSeparator () const { return IsSet (MF_SEPARATOR); }
		bool IsSysMenu () const { return IsSet (MF_SYSMENU); }
		bool IsDismissed () const { return _value == 0xf0000000; }
	};
}

#endif
