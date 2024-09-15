#if !defined(TREEVIEWHANDLER_H)
#define TREEVIEWHANDLER_H

#include <Ctrl/Tree.h>
#include "LeftWinCtrl.h"

class TopCtrl;
class LeftWinCtrl;

class TreeHandler : public Notify::TreeViewHandler 
{
public:
	TreeHandler(unsigned Id, Tree::View & treeview, LeftWinCtrl & leftwinctrl) : 
											Notify::TreeViewHandler (Id),
												     _treeView(treeview),
											  _leftWinCtrl(leftwinctrl)
	{}
	bool OnItemExpanding (Tree::View::Item & item,
									Tree::Notify::Action action,
									bool & allow) throw ();
	bool OnRClick (NMHDR *hdr) throw ();
	bool OnBeginLabel(NMTVDISPINFO &nmTvDisp) throw ();
	bool OnEndLabel(NMTVDISPINFO &nmTvDisp) throw ();
	bool OnBeginDrag(NMHDR * hdr) throw ();
private:
	Tree::View		&_treeView;
	LeftWinCtrl		&_leftWinCtrl;
	char *			_itemText;
};

#endif