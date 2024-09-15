#include "precompiled.h"
#include "TreeViewHandler.h"
#include <Win/Keyboard.h>
#include "UserMessages.h"
#include "Params.h"
#include "OutSink.h"

bool TreeHandler::OnItemExpanding (Tree::View::Item & item,
									Tree::Notify::Action action,
									bool & allow) throw ()
{ 
	allow = false; 
	return false; 
}

bool TreeHandler::OnRClick (NMHDR *hdr) throw ()
{

	return false;
}

bool TreeHandler::OnBeginLabel (NMTVDISPINFO &nmTvDisp) throw ()
{
	TVITEM tvitem = nmTvDisp.item; 
	int i = 0;
	while (tvitem.pszText [i] != 0)
	{
		i++;
	}
	_itemText = new char[i+1];
	i = 0;
	while (tvitem.pszText [i] != 0)
	{
		_itemText[i] = tvitem.pszText [i];
		i++;
	}
	_itemText[i] = 0;
	return false;
}

bool TreeHandler::OnEndLabel (NMTVDISPINFO &nmTvDisp) throw ()
{
	TVITEM tvitem = nmTvDisp.item;
	if(tvitem.pszText == NULL)
	{
		tvitem.pszText = (LPSTR)_itemText;
		tvitem.cchTextMax = strlen(_itemText);
		tvitem.mask = TVIF_TEXT | TVIF_HANDLE;
		TreeView_SetItem(_treeView.ToNative(),&tvitem);
		delete _itemText;
		_itemText = NULL;
	}
	else
	{
		std::string text;
		text = tvitem.pszText;
		if(text.length() >= MAXLABELLENGTH)
		{
			TheOutput.Display ("Labels must be less than 30 characters!", Out::Information);
			tvitem.pszText = (LPSTR)_itemText;
			tvitem.cchTextMax = strlen(_itemText);
			tvitem.mask = TVIF_TEXT | TVIF_HANDLE;
			TreeView_SetItem(_treeView.ToNative(),&tvitem);
			delete _itemText;
			_itemText = NULL;
		}
		else
		{
			tvitem.mask = TVIF_TEXT | TVIF_HANDLE;
			TreeView_SetItem(_treeView.ToNative(),&tvitem);
		}
	}
	return false;
}

bool TreeHandler::OnBeginDrag (NMHDR *hdr) throw ()
{
	NMTREEVIEW * lpnmtv = reinterpret_cast<NMTREEVIEW *>(hdr);
	HIMAGELIST himl;    // handle to image list 
 
    // Tell the tree view control to create an image to use 
    // for dragging. 
    himl = TreeView_CreateDragImage(_treeView.ToNative(), lpnmtv->itemNew.hItem); 
 
    // Start the drag operation. 
    ImageList_BeginDrag(himl, 0, 0, 0); 
	ImageList_DragShowNolock(1);	

 
    // Hide the mouse cursor, and direct mouse input to the 
    // parent window. 
    ShowCursor(FALSE);
	Win::Dow::Handle parent = _treeView.GetParent();
    SetCapture(parent.ToNative ());
	HTREEITEM hTItem = TreeView_GetSelection(_treeView.ToNative());
    _leftWinCtrl.OnBeginDrag (hTItem);

	return false;
}