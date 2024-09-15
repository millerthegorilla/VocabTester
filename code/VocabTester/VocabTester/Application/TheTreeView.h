#if !defined(THETREEVIEW_H)
#define THETREEVIEW_H

#include <Ctrl/Tree.h>
#include <Graph/ImageList.h>
#include "TreeViewHandler.h"
#include "TvCtrl.h"
#include "resource.h"


class TheTreeView
{
public:
	TheTreeView(Win::Dow::Handle &parentWinH);
	void Load(char * filename);
	void Save(char * filename);
	TreeHandler & GetTreeHandler()const {return *_treeHandler;}
	Tree::View GetHandle()const {return _treeView;}
private:

	int _imageFolderClosed;
	int _imageFolderOpen;
	Tree::View			_treeView;
	TvCtrl				_tvCtrl;
	ImageList::AutoHandle			_imageList;
	std::auto_ptr<TreeHandler>		_treeHandler;
};




#endif