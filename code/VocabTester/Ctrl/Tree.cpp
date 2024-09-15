// (c) Reliable Software 2002
#include <WinLibBase.h>
#include "Tree.h"
#include <Graph/ImageList.h>
#include <Win/Keyboard.h>

namespace Notify
{
	bool TreeViewHandler::OnNotify (NMHDR * hdr, long & result)
	{
		NMTREEVIEW * notifyInfo = reinterpret_cast<NMTREEVIEW *>(hdr);
		Tree::View::Item * itemOld = reinterpret_cast<Tree::View::Item *>(&notifyInfo->itemOld);
		Tree::View::Item * itemNew = reinterpret_cast<Tree::View::Item *>(&notifyInfo->itemNew);
		Tree::Notify::Action action = static_cast<Tree::Notify::Action> (notifyInfo->action);
		switch (hdr->code)
		{
		case TVN_GETDISPINFO:
			{
				NMTVDISPINFO * dispInfo = reinterpret_cast<NMTVDISPINFO *> (hdr);
				Tree::View::Item * item = reinterpret_cast<Tree::View::Item *>(&dispInfo->item);
				// make copies
				Tree::View::Request request (*item);
				Tree::View::State state (*item);
				// clear original
				item->Unmask ();
				return OnGetDispInfo (request, state, *item);
			}
		case TVN_BEGINLABELEDIT:
			{
				NMTVDISPINFO * dispInfo = reinterpret_cast<NMTVDISPINFO *> (hdr);
				return OnBeginLabel(*dispInfo);
			}
		case TVN_ENDLABELEDIT:
			{
				NMTVDISPINFO * dispInfo = reinterpret_cast<NMTVDISPINFO *> (hdr);
				return OnEndLabel(*dispInfo);
			}
		case TVN_ITEMEXPANDING:
			{
				bool ok = false;
				bool allow = false;
				if (notifyInfo->action == TVE_EXPAND)
					ok = OnItemExpanding (*itemNew, action, allow);
				else if (notifyInfo->action == TVE_COLLAPSE)
					ok = OnItemCollapsing (*itemNew, action, allow);
				result = allow? FALSE: TRUE;
				return ok;
			}
		case TVN_ITEMEXPANDED:
			{
				if (notifyInfo->action == TVE_EXPAND)
					return OnItemExpanded (*itemNew, action);
				else if (notifyInfo->action == TVE_COLLAPSE)
					return OnItemCollapsed (*itemNew, action);
			}
		case TVN_SELCHANGING:
			{
				return OnSelChanging (*itemOld, *itemNew, action);
			}
		case TVN_SELCHANGED:
			{
				return OnSelChanged (*itemOld, *itemNew, action);
			}
		case NM_SETFOCUS :
			return OnSetFocus (hdr->hwndFrom, hdr->idFrom);
		case TVN_KEYDOWN:
			{
				NMTVKEYDOWN * keyDown = reinterpret_cast<NMTVKEYDOWN *> (hdr);
				Keyboard::Handler * pHandler = GetKeyboardHandler ();
				if (pHandler != 0)
				{
					if (pHandler->OnKeyDown (keyDown->wVKey, keyDown->flags))
						return true;
				}
			}
			break;
		case TVN_BEGINDRAG:
			{
				return OnBeginDrag(hdr);
			}
			break;
		case NM_RCLICK :
			return OnRClick (hdr);
		}
		return false;
	}
}

namespace Tree
{
	Maker::Maker (Win::Dow::Handle parentWin, int id)
		: Win::ControlMaker (WC_TREEVIEW, parentWin, id)
	{
		Win::CommonControlsRegistry::Instance()->Add(Win::CommonControlsRegistry::TREEVIEW);
	}
	void View::AttachImageList (ImageList::Handle imageList)
	{
		Win::Message msg (TVM_SETIMAGELIST);
		msg.SetWParam (TVSIL_NORMAL);
		msg.SetLParam (reinterpret_cast<long> (imageList.ToNative ()));
		SendMsg (msg);
	}
	
	NodeHandle View::InsertRoot (Tree::Node & item)
	{
		item.ToInsertAfter (TVI_ROOT);
		Win::Message msg (TVM_INSERTITEM);
		msg.SetLParam (&item);
		SendMsg (msg);
		return reinterpret_cast<HTREEITEM> (msg.GetResult ());
	}

	NodeHandle View::AppendChild (Tree::Node & item)
	{
		item.ToInsertAfter (TVI_LAST);
		Win::Message msg (TVM_INSERTITEM);
		msg.SetLParam (&item);
		SendMsg (msg);
		if (msg.GetResult () == 0)
			throw Win::Exception ("Cannot append tree node");
		return reinterpret_cast<HTREEITEM> (msg.GetResult ());
	}

	void View::Select (Tree::NodeHandle node)
	{
		Win::Message msg (TVM_SELECTITEM, TVGN_CARET);
		msg.SetLParam (node.ToNative ());
		SendMsg (msg);
		if (msg.GetResult () == 0)
			throw Win::Exception ("Cannot select item");
	}

	void View::Expand (Tree::NodeHandle parent)
	{
		Win::Message msg (TVM_EXPAND, TVE_EXPAND);
		msg.SetLParam (parent.ToNative ());
		SendMsg (msg);
		if (msg.GetResult () == 0)
			throw Win::Exception ("Cannot expand tree");
	}

	void View::Collapse (Tree::NodeHandle parent, bool forget)
	{
		unsigned flag = TVE_COLLAPSE;
		if (forget)
			flag |= TVE_COLLAPSERESET;
		Win::Message msg (TVM_EXPAND, flag);
		msg.SetLParam (parent.ToNative ());
		SendMsg (msg);
		//if (msg.GetResult () == 0)
		//	throw Win::Exception ("Cannot collapse tree");
	}

	void View::ClearAll ()
	{
		Win::Message msg (TVM_DELETEITEM);
		msg.SetLParam (TVI_ROOT);
		SendMsg (msg);
		if (msg.GetResult () == 0)
			throw Win::Exception ("Cannot clear tree");
	}

	void View::SetChildless (Tree::NodeHandle node)
	{
		Item item (node);
		item.SetChildCount (0);
		Win::Message msg (TVM_SETITEM);
		msg.SetLParam (&item);
		SendMsg (msg);
	}
}
