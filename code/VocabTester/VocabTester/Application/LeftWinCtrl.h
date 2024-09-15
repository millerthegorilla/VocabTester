#if !defined(LEFTWINCTRL_H)
#define LEFTWINCTRL_H

#include <Win/Controller.h>
#include <Ctrl/Tree.h>
#include <Graph/ImageList.h>
#include <XML/Tree.h>
#include "TreeViewHandler.h"
#include "TvCtrl.h"
#include "resource.h"
//#include "TreeNode.h"

class TreeHandler;
class TvCtrl;

enum EntryCode
{
	ISCHILD,
	ISSIBLING,
	ISROOT
};

class LeftWinCtrl : public Win::Controller
{
public:
	LeftWinCtrl(): _initMsg ("InitMessage"), 
				   _imageList(16,16,9,0,0),
				   _xmlTree(new XML::Tree)
	{}
	Notify::Handler * GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw ();
	bool OnCreate (Win::CreateData const * create, bool & success) throw ();
	bool OnSize (int width, int height, int flag) throw ();
	bool OnLButtonDown(int x, int y, Win::KeyState kState) throw();
	bool OnRegisteredMessage (Win::Message & msg) throw ();
	bool OnUserMessage(Win::UserMessage &msg) throw ();
	bool OnMouseMove(int x, int y, Win::KeyState kState) throw ();
	bool OnLButtonUp(int x, int y, Win::KeyState kState) throw ();
	void OnBeginDrag(HTREEITEM htitem);
	int GetImageFolderClosed() const { return _imageFolderClosed; }
	int GetImageFolderOpen() const { return _imageFolderOpen; }
	int GetImageBookClosed() const { return _imageBookClosed; }
	int GetImageBookOpen() const { return _imageBookOpen; }
	int GetImageListClosed() const { return _imageListClosed; }
	int GetImageListOpen() const { return _imageListOpen; }
	int GetImageT() const { return _imageT; }
	int GetImageW() const { return _imageW; }
	void OnInit();
	HTREEITEM MoveBranch(HTREEITEM hItem, HTREEITEM hItemTo,  
		       HTREEITEM hItemPos = TVI_LAST);
	void SaveTree(HTREEITEM hItem, XML::Node * thisNode, EntryCode entrycode);
	void SaveTheTree(char * filename);
	void InsertRootItem();
private:
	ImageList::AutoHandle			_imageList;
	int								_imageFolderClosed;
	int								_imageFolderOpen;
	int								_imageBookOpen;
	int								_imageBookClosed;
	int								_imageListOpen;
	int								_imageListClosed;
	int								_imageT;
	int								_imageW;
	Tree::View						_treeView;
	std::auto_ptr<TvCtrl>			_tvCtrl;
	Win::RegisteredMessage			_initMsg;
	std::auto_ptr<TreeHandler>  	_treeHandler;
	BOOL							_isDragging;
	HTREEITEM                       _tvItemHandle;
	std::auto_ptr<XML::Tree>		_xmlTree;

};

#endif