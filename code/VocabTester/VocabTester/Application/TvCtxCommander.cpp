#include "precompiled.h"
#include "TvCtxCommander.h"
#include "About.h"
#include "TvCtrl.h"
#include "TvTest.h"
#include <XML/Scanner.h>
#include <XML/Parser.h>
#include "InSink.h"
#include <fstream>

Cmd::Status TvCtxCommander::Can_Program_About () const
{
	return Cmd::Disabled;
}

void TvCtxCommander::Program_About()
{
	AboutDlgCtrl ctrl;
	Dialog::Modal dlg (_ctrl.GetWindow (), ctrl);
}

Cmd::Status TvCtxCommander::TreeView_Can_InsertFolder () const
{
	HWND Hwnd = _ctrl.GetWindow().ToNative ();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	selected.mask = TVIF_IMAGE | TVIF_HANDLE;
	TreeView_GetItem(Hwnd, &selected);
	if (selected.iImage == _leftWinCtrl.GetImageFolderClosed()
		|| selected.iImage == _leftWinCtrl.GetImageBookClosed())
	{
		return Cmd::Enabled;
	}
	return Cmd::Disabled;
}

void TvCtxCommander::TreeView_InsertFolder () //insertchild
{
	HWND Hwnd = _ctrl.GetWindow().ToNative();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	//setup the new parent
	TVITEMEX tv;
	tv.mask = TVIF_HANDLE | TVIF_CHILDREN; 
	tv.hItem = selected.hItem;
	tv.cChildren = 1;
	TreeView_SetItem(Hwnd, &tv );
	//get the parent's height
	tv.mask = TVIF_HANDLE | TVIF_PARAM;
	TreeView_GetItem(Hwnd, &tv);
	//add one to the height
	tv.lParam = tv.lParam + 1;

	Tree::Node tnode;
	tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
	tv.iImage = _leftWinCtrl.GetImageFolderClosed ();
	tv.iSelectedImage = _leftWinCtrl.GetImageFolderOpen ();
	tv.cchTextMax = 10;
	tv.pszText = "New Folder"; 
	tnode.SetParent (selected.hItem);
	tnode.ToInsertAfter (TVI_LAST);
	tnode.itemex = tv;

	TreeView_InsertItem(Hwnd, &tnode);
	//_treeView.AppendChild (tnode);

}

Cmd::Status TvCtxCommander::TreeView_Can_InsertList () const
{
	HWND Hwnd = _ctrl.GetWindow().ToNative ();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	selected.mask = TVIF_IMAGE | TVIF_HANDLE;
	TreeView_GetItem(Hwnd, &selected);
	if (selected.iImage == _leftWinCtrl.GetImageFolderClosed ())
	{
		return Cmd::Enabled;
	}
	return Cmd::Disabled;
}

void TvCtxCommander::TreeView_InsertList()
{
	HWND Hwnd = _ctrl.GetWindow().ToNative();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	//setup the new parent
	TVITEMEX tv;
	tv.mask = TVIF_HANDLE | TVIF_CHILDREN; 
	tv.hItem = selected.hItem;
	tv.cChildren = 1;
	TreeView_SetItem(Hwnd, &tv );
	//get the parent's height
	tv.mask = TVIF_HANDLE | TVIF_PARAM;
	TreeView_GetItem(Hwnd, &tv);
		//add one to the height
	tv.lParam = tv.lParam + 1;

	Tree::Node tnode;
	tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
	tv.iImage = _leftWinCtrl.GetImageListClosed ();
	tv.iSelectedImage = _leftWinCtrl.GetImageListOpen ();
	tv.cchTextMax = 10;
	tv.pszText = "New List"; 
	tnode.SetParent (selected.hItem);
	tnode.ToInsertAfter (TVI_LAST);
	tnode.itemex = tv;

	TreeView_InsertItem(Hwnd, &tnode);
}

Cmd::Status TvCtxCommander::TreeView_Can_InsertWord () const
{
	HWND Hwnd = _ctrl.GetWindow().ToNative (); 
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	selected.mask = TVIF_IMAGE | TVIF_HANDLE;
	BOOL bob = TreeView_GetItem(Hwnd, &selected);
	if (selected.iImage == _leftWinCtrl.GetImageListClosed ())
	{
		return Cmd::Enabled;
	}
	return Cmd::Disabled;
}

void TvCtxCommander::TreeView_InsertWord ()
{
	HWND Hwnd = _ctrl.GetWindow().ToNative();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	//setup the new parent
	TVITEMEX tv;
	tv.mask = TVIF_HANDLE | TVIF_CHILDREN; 
	tv.hItem = selected.hItem;
	tv.cChildren = 1;
	TreeView_SetItem(Hwnd, &tv );
	//get the parent's height
	tv.mask = TVIF_HANDLE | TVIF_PARAM;
	TreeView_GetItem(Hwnd, &tv);
	//add one to the height
	tv.lParam = tv.lParam + 1;

	Tree::Node tnode;
	tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
	tv.iImage = _leftWinCtrl.GetImageW ();
	tv.iSelectedImage = _leftWinCtrl.GetImageW ();
	tv.cchTextMax = 10;
	tv.pszText = "New Word"; 
	tnode.SetParent (selected.hItem);
	tnode.ToInsertAfter (TVI_LAST);
	tnode.itemex = tv;

	TreeView_InsertItem(Hwnd, &tnode);
}

Cmd::Status TvCtxCommander::TreeView_Can_InsertTrans () const
{
	HWND Hwnd = _ctrl.GetWindow().ToNative ();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	selected.mask = TVIF_IMAGE | TVIF_HANDLE;
	BOOL bob = TreeView_GetItem(Hwnd, &selected);
	if (selected.iImage == _leftWinCtrl.GetImageW())
	{
		return Cmd::Enabled;
	}
	return Cmd::Disabled;
}

void TvCtxCommander::TreeView_InsertTrans ()
{
	HWND Hwnd = _ctrl.GetWindow().ToNative();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	//setup the new parent
	TVITEMEX tv;
	tv.mask = TVIF_HANDLE | TVIF_CHILDREN; 
	tv.hItem = selected.hItem;
	tv.cChildren = 1;
	TreeView_SetItem(Hwnd, &tv );
	//get the parent's height
	tv.mask = TVIF_HANDLE | TVIF_PARAM;
	TreeView_GetItem(Hwnd, &tv);
	//add one to the height
	tv.lParam = tv.lParam + 1;

	Tree::Node tnode;
	tv.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
	tv.iImage = _leftWinCtrl.GetImageT ();
	tv.iSelectedImage = _leftWinCtrl.GetImageT ();
	tv.cchTextMax = 15;
	tv.pszText = "New Translation"; 
	tnode.SetParent (selected.hItem);
	tnode.ToInsertAfter (TVI_LAST);
	tnode.itemex = tv;

	TreeView_InsertItem(Hwnd, &tnode);
}

Cmd::Status TvCtxCommander::TreeView_Can_Delete_Item () const
{
	HWND Hwnd = _ctrl.GetWindow().ToNative ();
	Tree::View::Item selected = TreeView_GetSelection(Hwnd);
	selected.mask = TVIF_IMAGE | TVIF_HANDLE;
	BOOL bob = TreeView_GetItem(Hwnd, &selected);
	if (selected.iImage == _leftWinCtrl.GetImageBookClosed ())
	{
		return Cmd::Disabled;
	}
	return Cmd::Enabled;
}

void TvCtxCommander::TreeView_Delete_Item ()
{
	HWND hWnd = _ctrl.GetWindow().ToNative ();
	Tree::View::Item selected = TreeView_GetSelection(hWnd);
	TreeView_DeleteItem(hWnd,selected.hItem);
}

void TvCtxCommander::TestItem ()
{	
	/*HWND hWnd = _ctrl.GetWindow().ToNative ();
	Tree::View::Item selected = TreeView_GetSelection(hWnd);
	selected.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
	TreeView_GetItem(hWnd,&selected);
	TvTest ctrl(selected);
	Dialog::Modal dlg (_ctrl.GetWindow (), ctrl);*/
	_leftWinCtrl.SaveTheTree ("c://test.xml");
}

void TvCtxCommander::LoadTree ()
{
	//delete all items
	TreeView_DeleteAllItems(_ctrl.GetWindow().ToNative ());
	//insert new root item
	_leftWinCtrl.InsertRootItem ();

	std::ifstream infile;
	infile.open("c://test.xml");
	XML::Scanner scanner(infile);
	InSink sink(_ctrl.GetWindow().ToNative(), TreeView_GetRoot(_ctrl.GetWindow().ToNative()));
	XML::Parser parser (scanner,sink);
	parser.Parse ();
	infile.close ();
}