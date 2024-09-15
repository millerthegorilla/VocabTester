#if !defined (INSINK_H)
#define INSINK_H

#include <XML/Sink.h>
#include "TvCtrl.h"

class InSink : public XML::Sink 
{
public:
	InSink(HWND  treeview, HTREEITEM root) : _treeView(treeview),
											_isCurrentNodeOpen(false), 
											_lastHeight(0),
											_lastInserted(root)
	{}
	void OnAttribute (const std::string &attrib, const std::string &value);
private:
	HWND			_treeView;
	TVITEMEX		_currentNode;
	HTREEITEM		_lastInserted;
	bool			_isCurrentNodeOpen;
	int				_lastHeight;
};


#endif