#if !defined (TVCTXCOMMANDER_H)
#define TVCTXCOMMANDER_H

#include <Ctrl/Command.h>
#include <Ctrl/Tree.h>
#include "LeftWinCtrl.h"

class TvCtrl;
class LeftWinCtrl;

class TvCtxCommander

{
public:
	TvCtxCommander (TvCtrl & ctrl, HWND	treeView, LeftWinCtrl & leftwinctrl)
		: _ctrl (ctrl), _treeView(treeView), _leftWinCtrl(leftwinctrl)
	{}
	Cmd::Status Can_Program_About () const;
	void Program_About();
	Cmd::Status TreeView_Can_InsertFolder () const;
	void TreeView_InsertFolder ();
	Cmd::Status TreeView_Can_InsertList () const;
	void TreeView_InsertList();
	Cmd::Status TreeView_Can_InsertWord () const;
	void TreeView_InsertWord();
	Cmd::Status TreeView_Can_InsertTrans () const;
	void TreeView_InsertTrans();
	Cmd::Status TreeView_Can_Delete_Item () const;
	void TreeView_Delete_Item();
	void TestItem();
	void LoadTree();
private:
	TvCtrl    & _ctrl;
	HWND	 _treeView;
	LeftWinCtrl & _leftWinCtrl;
};

#endif
