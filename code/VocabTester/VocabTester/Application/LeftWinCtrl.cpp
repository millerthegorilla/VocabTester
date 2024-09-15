#include "precompiled.h"
#include "LeftWinCtrl.h"
#include "UserMessages.h"
#include <fstream>

bool LeftWinCtrl::OnLButtonDown (int x, int y, Win::KeyState kState) throw()
{
	return false;
}

bool LeftWinCtrl::OnCreate (Win::CreateData const * create, bool & success) throw ()
{
	success = true;
	GetWindow().PostMsg (_initMsg);
	return true;
}

void LeftWinCtrl::OnInit ()
{
	_treeHandler.reset(new TreeHandler(1,_treeView,*this));
	Win::Dow::Handle win = GetWindow();
	Win::Rect rc;
	GetWindow().GetWindowRect(rc);
	Tree::Maker treemaker(win,1);
	treemaker.SetPosition(0,0, rc.Width (), rc.Height ());
	treemaker.Style () 
		<< Tree::View::Style::Bits::HasLines
		<< Tree::View::Style::Bits::LinesAtRoot
		<< Tree::View::Style::Bits::ShowSelAlways
		<< Tree::View::Style::Bits::HasButtons
		<< Tree::View::Style::Bits::CheckBoxes
		<< Tree::View::Style::Bits::EditLabels;
	_treeView.Reset(treemaker.Create ());
	_tvCtrl.reset(new TvCtrl(*this));
	_treeView.SubClass(_tvCtrl.get());
	HINSTANCE hinst = win.GetInstance();
	_imageFolderClosed = _imageList.AddImage(hinst, IDB_BITMAP1);
	_imageFolderOpen = _imageList.AddImage(hinst, IDB_BITMAP2);
	_imageBookOpen = _imageList.AddImage(hinst, IDB_OPENBK);
	_imageBookClosed = _imageList.AddImage(hinst, IDB_CLOSEDBK);
	_imageListOpen = _imageList.AddImage(hinst, IDB_OPENLST);
	_imageListClosed = _imageList.AddImage(hinst,IDB_CLOSEDLST);
	_imageW	= _imageList.AddImage(hinst, IDB_WORD);
	_imageT = _imageList.AddImage(hinst, IDB_TRANSL);
	_treeView.AttachImageList (_imageList.ToNative());
	_treeView.ForceRepaint();
	
	InsertRootItem();

}

void LeftWinCtrl::InsertRootItem ()
{
	Tree::View::Item	tvitem;
	
	tvitem.SetText();
	tvitem.SetIcon (_imageBookClosed,_imageBookOpen);
	tvitem.pszText = "Root";
	tvitem.lParam = 0;
	Tree::Node  tnode;
	tnode.itemex = tvitem;
	tnode.SetHasChildren (true);
	Tree::NodeHandle roothandle = _treeView.InsertRoot (tnode);
}

Notify::Handler * LeftWinCtrl::GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw ()
{
	if(winFrom == _treeView)
	{
		return _treeHandler.get();
	}
	return 0;
}

bool LeftWinCtrl::OnSize (int width, int height, int flag) throw ()
{
    _treeView.Move(0,0,width, height);
	return true;
}

bool LeftWinCtrl::OnRegisteredMessage (Win::Message & msg) throw ()
{
	if (msg == _initMsg)
	{
		OnInit ();
		return true;
	}
	return false;
}

bool LeftWinCtrl::OnUserMessage (Win::UserMessage & msg) throw ()
{
/*	if (msg == UM_BEGINDRAG)
	{
		_isDragging = 1;
		_tvItemHandle = (HTREEITEM*) msg.GetWParam();
	}*/
	return false;
}

void LeftWinCtrl::OnBeginDrag (HTREEITEM htitem)
{
	_tvItemHandle = TreeView_GetSelection(_treeView.ToNative());
	_isDragging = 1;
}

bool LeftWinCtrl::OnMouseMove (int x, int y, Win::KeyState kState)
{	
	HTREEITEM htiTarget;  // handle to target item 
    TVHITTESTINFO tvht;  // hit test information 

	if(_isDragging)
	{
        // Drag the item to the current position of the mouse cursor.
		POINT pt;
		pt.x = x;
		pt.y = y;
		ClientToScreen(GetWindow().ToNative(),&pt);
        ImageList_DragMove(pt.x, pt.y); 
 
        // Find out if the cursor is on the item. If it is, highlight 
        // the item as a drop target. 
        tvht.pt.x = x; 
        tvht.pt.y = y; 
        if ((htiTarget = TreeView_HitTest(_treeView.ToNative(), &tvht)) != NULL) 
		{ 
            TreeView_Select(_treeView.ToNative(), htiTarget, (TVGN_CARET | TVGN_DROPHILITE));
			_treeView.ForceRepaint();
			ImageList_DragMove(pt.x,pt.y);
        } 
    } 

	return true;
}

bool LeftWinCtrl::OnLButtonUp (int x, int y, Win::KeyState kState)
{
	if (_isDragging) 
	{ 
        ImageList_EndDrag(); 
 

		HTREEITEM selected = TreeView_GetSelection(_treeView.ToNative());
		Tree::View::Item tvitemDragged;
		char buffer[20];
		tvitemDragged.mask = TVIF_HANDLE | TVIF_STATE | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvitemDragged.stateMask = TVIS_STATEIMAGEMASK;
		tvitemDragged.hItem = _tvItemHandle;
		tvitemDragged.pszText = buffer;
		tvitemDragged.cchTextMax = 20;
		TreeView_GetItem(_treeView.ToNative(),&tvitemDragged);
		
		Tree::View::Item tvitemDropTarget;
		tvitemDropTarget.mask = TVIF_IMAGE | TVIF_HANDLE;
		tvitemDropTarget.hItem = selected;
		TreeView_GetItem(_treeView.ToNative(), &tvitemDropTarget);
		
		bool canDrop = false;
		if (tvitemDragged.iImage == _imageFolderClosed)
		{
			if(tvitemDropTarget.iImage == _imageFolderClosed || tvitemDropTarget.iImage == _imageBookClosed)
			{
				canDrop = true;
			}
		}
		else if(tvitemDragged.iImage == _imageListClosed)
		{
			if(tvitemDropTarget.iImage == _imageFolderClosed)
			{
				canDrop = true;
			}
		}
		else if(tvitemDragged.iImage == _imageW)
		{
			if(tvitemDropTarget.iImage == _imageListClosed)
			{
				canDrop = true;
			}
		}
		
		if(canDrop)
		{

			MoveBranch(_tvItemHandle, selected);
			TVITEM tvi;
			tvi.mask = TVIF_HANDLE | TVIF_CHILDREN;
			tvi.hItem = selected;
			tvi.cChildren = 1;
			TreeView_SetItem(_treeView.ToNative(),&tvi);
			
		}//end (if canDrop)
		_isDragging = FALSE;
		ImageList_DragShowNolock(0);
		ReleaseCapture(); 
	    ShowCursor(TRUE);
    }//end (is Dragging) 

    return true;
}

HTREEITEM LeftWinCtrl::MoveBranch(HTREEITEM hItem, HTREEITEM hItemTo,  
		       HTREEITEM hItemPos)
{//thanks to :
//Author:	Paul S. Vickery
// E-mail:	paul@vickeryhome.freeserve.co.uk
  if (hItem == NULL || hItemTo == NULL)
    return NULL;
  if (hItem == hItemTo || hItemTo == TreeView_GetParent(_treeView.ToNative(),hItem))
    return hItem;
  // check we're not trying to move to a descendant
  HTREEITEM hItemParent = hItemTo;
  while (hItemParent != TVI_ROOT && 
        (hItemParent = TreeView_GetParent(_treeView.ToNative(),hItemParent)) != NULL)
    if (hItemParent == hItem)
      return NULL;

  // copy items to new location, recursively, then delete old hierarchy
  // get text, and other info

  TVINSERTSTRUCT tvis;
  char buffer[20];

  tvis.item.mask = TVIF_HANDLE | TVIF_TEXT | 
					TVIF_STATE | TVIF_IMAGE | 
					TVIF_SELECTEDIMAGE | TVIF_CHILDREN |
					TVIF_INTEGRAL;
  tvis.item.hItem = hItem;
  tvis.item.pszText = buffer;
  tvis.item.cchTextMax = 20;
  tvis.item.stateMask = TVIS_STATEIMAGEMASK;
  TreeView_GetItem(_treeView.ToNative(),&tvis.item);
  tvis.hParent = hItemTo;
  tvis.hInsertAfter = hItemPos;

  HTREEITEM hItemNew = TreeView_InsertItem(_treeView.ToNative(),&tvis);


  // now move children to under new item
  HTREEITEM hItemChild = TreeView_GetChild(_treeView.ToNative(), hItem);
  while (hItemChild != NULL)
  {
    HTREEITEM hItemNextChild = TreeView_GetNextSibling(_treeView.ToNative(),hItemChild);
    MoveBranch( hItemChild, hItemNew);
    hItemChild = hItemNextChild;
  }

  TreeView_DeleteItem(_treeView.ToNative(),hItem);

  

  return hItemNew;
}

//stub function for savetree
void LeftWinCtrl::SaveTheTree (char *filename)
{
	XML::Tree xmlTree;
	XML::Node * root = xmlTree.SetRoot ("root");
	
	std::ofstream outfile;
	outfile.open (filename);
	HTREEITEM hRoot = TreeView_GetRoot(_treeView.ToNative());
	SaveTree(hRoot, root, ISROOT);
	xmlTree.Write(outfile);
	outfile.close();

}

//when calling this for the 1st time, isSibling = false & _xmlTree must be initialised with
//root perhaps in stubfunction that writes the tree when everything returns
void LeftWinCtrl::SaveTree (HTREEITEM hItem, XML::Node * thisNode, EntryCode entrycode)
{
	if(hItem == NULL)
	{
		return;
	}

	if(entrycode == ISSIBLING)
	{	
		XML::Node * sibling;
		TVITEMEX	tvitem;
		tvitem.mask = TVIF_CHILDREN | TVIF_HANDLE    
						| TVIF_IMAGE
						| TVIF_INTEGRAL 
						| TVIF_SELECTEDIMAGE  
						| TVIF_STATE   
						| TVIF_TEXT;
		tvitem.stateMask = TVIS_STATEIMAGEMASK;
		tvitem.cchTextMax = 30;
		char buffer[30];
		tvitem.pszText = buffer;
		tvitem.hItem = hItem;
		TreeView_GetItem(_treeView.ToNative(),&tvitem);

		sibling = thisNode->AddChild("sibling");
		sibling->SetParent (thisNode->GetParent ());
		sibling->AddAttribute ("Height", ToString(tvitem.lParam ));
		sibling->AddAttribute ("Image", ToString(tvitem.iImage ));
		sibling->AddAttribute ("ImageSelected", ToString(tvitem.iSelectedImage ));
		sibling->AddAttribute ("StateImage",ToString(tvitem.state ));
		sibling->AddAttribute ("Text",ToString(tvitem.pszText ));
		thisNode = sibling;
	}
	else if (entrycode == ISCHILD)
	{
		XML::Node * child;
		TVITEMEX	tvitem;
		tvitem.mask = TVIF_CHILDREN | TVIF_HANDLE    
						| TVIF_IMAGE
						| TVIF_INTEGRAL 
						| TVIF_SELECTEDIMAGE  
						| TVIF_STATE   
						| TVIF_TEXT;
		tvitem.stateMask = TVIS_STATEIMAGEMASK;
		tvitem.cchTextMax = 30;
		char buffer[30];
		tvitem.pszText = buffer;
		tvitem.hItem = hItem;
		TreeView_GetItem(_treeView.ToNative(),&tvitem);
		
		child = thisNode->AddChild("Child");
		child->SetParent (thisNode);
		child->AddAttribute ("Height", ToString(tvitem.lParam ));
		child->AddAttribute ("Image", ToString(tvitem.iImage ));
		child->AddAttribute ("ImageSelected", ToString(tvitem.iSelectedImage ));
		child->AddAttribute ("StateImage",ToString(tvitem.state ));
		child->AddAttribute ("Text",ToString(tvitem.pszText));
		
		thisNode = child;
	}

	HTREEITEM hItemChild = TreeView_GetChild(_treeView.ToNative(), hItem);
	if(hItemChild != NULL)
	{
		entrycode = ISCHILD;
	}
	while (hItemChild != NULL)
	{
		HTREEITEM hItemNextChild = TreeView_GetNextSibling(_treeView.ToNative(),hItemChild);
		SaveTree( hItemChild, thisNode, entrycode);
		hItemChild = hItemNextChild;
		entrycode = ISSIBLING;
	}
}