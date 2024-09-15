#include "precompiled.h"
#include "TheTreeView.h"

TheTreeView::TheTreeView(Win::Dow::Handle &parentWinH) : _imageList(16,16,2,0,0)
{
	_treeHandler.reset(new TreeHandler(1));
	Tree::Maker treemaker(parentWinH,1);
	treemaker.Style ()
		<< Win::Style::Child 
		<< Tree::View::Style::Bits::HasLines
		<< Tree::View::Style::Bits::LinesAtRoot
		<< Tree::View::Style::Bits::ShowSelAlways 
		<< Tree::View::Style::Bits::HasButtons;
	_treeView.Reset(treemaker.Create ());
	_treeView.SubClass(&_tvCtrl);
	_imageFolderClosed = _imageList.AddImage(parentWinH.GetInstance(), IDB_BITMAP1);
	_imageFolderOpen = _imageList.AddImage(parentWinH.GetInstance(), IDB_BITMAP2);
	_treeView.AttachImageList (_imageList.ToNative());

}