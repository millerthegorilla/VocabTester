//-----------------------------------
// (c) Reliable Software 1998 -- 2004
//-----------------------------------

#include <WinLibBase.h>
#include "Menu.h"

using namespace Menu;

template<>
void Win::Disposal<Menu::Handle>::Dispose (Menu::Handle h) throw ()
{
	::DestroyMenu (h.ToNative ());
}

DropDown::DropDown (Item const * templ, Cmd::Vector & cmdVector)
    : _barItemCnt (0),
	  _template (templ),
	  _cmdVector (cmdVector),
	  _pDisplayNameSource(0)
{
    // Count menu bar items
	while (_template [_barItemCnt].kind != END)
		_barItemCnt++;

    // Create menu
    for (int curMenu = 0; curMenu < _barItemCnt; curMenu++)
    {
        Assert (_template [curMenu].kind == Menu::POP);
        Menu::AutoHandle curPopup = CreatePopup (_template [curMenu].popup);
		_bar.AddPopup (curPopup, _template [curMenu].displayName);
    }
}

void DropDown::AttachToWindow (Win::Dow::Handle hwnd)
{
    if (!::SetMenu (hwnd.ToNative (), _bar.ToNative ()))
        throw Win::Exception ("Internal error: Cannot attach menu.");
}

void DropDown::RefreshPopup (Menu::Handle menu, int barItemNo) throw ()
{	
	if (barItemNo < 0 || barItemNo >= _barItemCnt)
		return;
	if (!_bar.IsSubMenu (menu, barItemNo))// if context menu
		return;
	menu.Clear ();
	Menu::Item const * popup = _template [barItemNo].popup;
	int curItem = 0;
	bool rememberAddSeparator = false;
	std::string dynamicDisplayName;
    while (popup [curItem].kind != END)
    {
		Menu::ItemKind kind = popup [curItem].kind;
        if (kind == CMD)
        {
            // Associate controller method with menu item
			Cmd::Status cmdStatus = _cmdVector.Test (popup [curItem].cmdName);
			if (cmdStatus != Cmd::Invisible)
			{
				if (rememberAddSeparator)
				{
					// Add separators only if needed
					if (!menu.IsEmpty ())
						menu.AddSeparator ();
					rememberAddSeparator = false;
				}

				char const *displayName = popup [curItem].displayName;

				if (_pDisplayNameSource != 0)
				{
					try
					{
						dynamicDisplayName =
							_pDisplayNameSource->GetItemDisplayName(popup[curItem].cmdName);
						if (!dynamicDisplayName.empty())
						{
							//	client wants to override displayName
							displayName = dynamicDisplayName.c_str();
						}
					}
					catch (...)
					{
						//	we'll just use the default display name
					}
				}

				menu.AddItem (_cmdVector.Cmd2Id (popup [curItem].cmdName), 
							  displayName);

				int itemId = _cmdVector.Cmd2Id (popup [curItem].cmdName);
				switch (cmdStatus)
				{
				case Cmd::Disabled:
					menu.Disable (itemId);
					break;
				case Cmd::Enabled:
					menu.Enable (itemId);
					menu.UnCheck (itemId);
					break;
				case Cmd::Checked:
					menu.Enable (itemId);
					menu.Check (itemId);
					break;
				}				
			}
        }
		else if (kind == POP)
		{
			if (rememberAddSeparator)
			{
				// Add separators only if needed
				if (!menu.IsEmpty ())
					menu.AddSeparator ();
				rememberAddSeparator = false;
			}
			Menu::AutoHandle subPopup = CreatePopup (popup [curItem].popup);
			menu.AddPopup (subPopup, popup [curItem].displayName);
		}
        else
        {
            Assert (kind == SEPARATOR);			
			rememberAddSeparator = true;
        }
		curItem++;
    }
}

void Menu::Handle::Clear ()
{
	int countItem = ::GetMenuItemCount (ToNative ());
	for (int k = countItem - 1; k >= 0; --k)
		::DeleteMenu (ToNative (), k, MF_BYPOSITION);
}

Menu::AutoHandle DropDown::CreatePopup (Menu::Item const * popupItems)
{
    Assert (popupItems != 0);
    
	Menu::Popup popupMaker;
	int curItem = 0;
    while (popupItems [curItem].kind != END)
    {
        if (popupItems [curItem].kind == CMD)
        {
            // Associate controller method with menu item
			popupMaker.AddItem (_cmdVector.Cmd2Id (popupItems [curItem].cmdName), 
								popupItems [curItem].displayName);
        }
		else if (popupItems [curItem].kind == Menu::POP)
		{
			popupMaker.AddItem (-1, popupItems [curItem].displayName);
		}
        else
        {
            Assert (popupItems [curItem].kind == SEPARATOR);
			popupMaker.AddSeparator ();
        }
		curItem++;
    }
    return (popupMaker);
}

Menu::Item const * DropDown::GetPopupTemplate (char const * menuName) const
{
	int curMenu = 0;
	while (_template [curMenu].kind != END)
	{
		if (strcmp (_template [curMenu].cmdName, menuName) == 0)
			break;
		curMenu++;
	}
	Assert (_template [curMenu].kind == Menu::POP);
	return _template [curMenu].popup;
}

Context::Context (std::vector<Menu::Item const *> const & templ, Cmd::Vector & cmdVector) :
				_templ(templ),
				_cmdVector(cmdVector),
				_pDisplayNameSource(0)
{
	CreateContext();
}

void Context::CreateContext() throw ()
{
	for (std::vector<Menu::Item const *>::const_iterator iter = _templ.begin ();
		 iter != _templ.end ();
		 ++iter)
	{
		Menu::Item const * curTempl = *iter;
		bool separatorPending = false;
		int curItem = 0;
		while (curTempl [curItem].kind != END)
		{
			if (curTempl [curItem].kind == CMD)
			{
				// Add only enabled items
				Cmd::Status cmdStatus = _cmdVector.Test (curTempl [curItem].cmdName);
				if (cmdStatus != Cmd::Disabled && cmdStatus != Cmd::Invisible)
				{
					if (separatorPending)
					{
						// Add separators only if needed
						if (!IsEmpty ())
							AddSeparator ();
						separatorPending = false;
					}
					AddItem (_cmdVector.Cmd2Id (curTempl [curItem].cmdName), 
							curTempl [curItem].displayName);
				}
			}
			else if (curTempl [curItem].kind == POP)
			{
				if (separatorPending)
				{
					// Add separators only if needed
					if (!IsEmpty ())
						AddSeparator ();
					separatorPending = false;
				}
				Menu::AutoHandle subPopup = CreatePopup (curTempl [curItem].popup, _cmdVector);
				AddPopup (subPopup, curTempl [curItem].displayName);
			}
			else
			{
				Assert (curTempl [curItem].kind == SEPARATOR);
				if (!separatorPending)
				{
					// Remember to add separator before next item if any
					separatorPending = true;
				}
			}
			curItem++;
		}
		if (!separatorPending)
		{
			// Remember to add separator before items of next template
			separatorPending = true;
		}
	}
}

Menu::AutoHandle Context::CreatePopup (Menu::Item const * popupItems, Cmd::Vector & cmdVector)
{
    Assert (popupItems != 0);
    
	Menu::Popup popupMaker;
	int curItem = 0;
    while (popupItems [curItem].kind != END)
    {
		Cmd::Status cmdStatus = Cmd::Enabled; //cmdVector.Test (popupItems [curItem].cmdName);
		if (cmdStatus != Cmd::Invisible)
		{
			if (popupItems [curItem].kind == CMD)
			{
				// Associate controller method with menu item
				popupMaker.AddItem (cmdVector.Cmd2Id (popupItems [curItem].cmdName), 
									popupItems [curItem].displayName);
				if(cmdStatus == Cmd::Disabled )
				{
					popupMaker.Disable(cmdVector.Cmd2Id (popupItems [curItem].cmdName));
				}
			}
			else if (popupItems [curItem].kind == Menu::POP)
			{
				popupMaker.AddItem (-1, popupItems [curItem].displayName);
			}
		}
        if(popupItems [curItem].kind == SEPARATOR)
        {
            Assert (popupItems [curItem].kind == SEPARATOR);
			popupMaker.AddSeparator ();
        }
		curItem++;
    }
    return (popupMaker);
}

void Context::RefreshPopup (Menu::Handle menu, int barItemNo) throw ()
{	
	if(!barItemNo)
	{
		menu.Clear ();
		CreateContext();
	}
/*
	
	Menu::Item const * popup = _templ[barItemNo];
	int curItem = 0;
	bool rememberAddSeparator = false;
	std::string dynamicDisplayName;
    while (popup [curItem].kind != END)
    {
		Menu::ItemKind kind = popup [curItem].kind;
        if (kind == CMD)
        {
            // Associate controller method with menu item
			Cmd::Status cmdStatus = Cmd::Enabled; //_cmdVector.Test (popup [curItem].cmdName);
			if (cmdStatus != Cmd::Invisible)
			{
				if (rememberAddSeparator)
				{
					// Add separators only if needed
					if (!menu.IsEmpty ())
						menu.AddSeparator ();
					rememberAddSeparator = false;
				}

				char const *displayName = popup [curItem].displayName;

				if (_pDisplayNameSource != 0)
				{
					try
					{
						dynamicDisplayName =
							_pDisplayNameSource->GetItemDisplayName(popup[curItem].cmdName);
						if (!dynamicDisplayName.empty())
						{
							//	client wants to override displayName
							displayName = dynamicDisplayName.c_str();
						}
					}
					catch (...)
					{
						//	we'll just use the default display name
					}
				}

				menu.AddItem (_cmdVector.Cmd2Id (popup [curItem].cmdName), 
							  displayName);

				int itemId = _cmdVector.Cmd2Id (popup [curItem].cmdName);
				switch (cmdStatus)
				{
				case Cmd::Disabled:
					menu.Disable (itemId);
					break;
				case Cmd::Enabled:
					menu.Enable (itemId);
					menu.UnCheck (itemId);
					break;
				case Cmd::Checked:
					menu.Enable (itemId);
					menu.Check (itemId);
					break;
				}				
			}
        }
		else if (kind == POP)
		{
			if (rememberAddSeparator)
			{
				// Add separators only if needed
				if (!menu.IsEmpty ())
					menu.AddSeparator ();
				rememberAddSeparator = false;
			}
			Menu::AutoHandle subPopup = CreatePopup (popup [curItem].popup, _cmdVector);
			menu.AddPopup (subPopup, popup [curItem].displayName);
		}
        else
        {
            Assert (kind == SEPARATOR);			
			rememberAddSeparator = true;
        }
		curItem++;
    }*/
}