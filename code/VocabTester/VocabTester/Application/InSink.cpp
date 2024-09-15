#include "precompiled.h"
#include "InSink.h"

void InSink::OnAttribute (const std::string &attrib, const std::string &value)
{
	if(attrib == "Height")
	{
		if(_isCurrentNodeOpen)
		{
			throw Win::Exception ("Error reading XML file");
		}
		else
		{
			_isCurrentNodeOpen = true;
		}
		_currentNode.lParam = atoi(value.c_str ());
	}

	if(attrib == "Image")
	{
		if(!_isCurrentNodeOpen)
		{
			throw Win::Exception ("Error reading XML file");
		}
		_currentNode.iImage = atoi(value.c_str());
	}

	if(attrib == "ImageSelected")
	{
		if(!_isCurrentNodeOpen)
		{
			throw Win::Exception ("Error reading XML file");
		}
		_currentNode.iSelectedImage = atoi(value.c_str()); 	
	}

	if(attrib == "StateImage")
	{
		if(!_isCurrentNodeOpen)
		{
			throw Win::Exception ("Error reading XML file");
		}
		_currentNode.state = atoi(value.c_str());
	}

	if(attrib == "Text")
	{
		_currentNode.cchTextMax = value.length ();
		_currentNode.pszText = (LPSTR)value.c_str();
		_currentNode.mask = TVIF_IMAGE | TVIF_PARAM  
							| TVIF_SELECTEDIMAGE 
                            | TVIF_STATE | TVIF_TEXT;
		_currentNode.stateMask = TVIS_STATEIMAGEMASK;

		if(_currentNode.lParam == _lastHeight + 1) //child of previous inserted
		{
			TVINSERTSTRUCT tvis;
			tvis.itemex = _currentNode;
			tvis.hInsertAfter = TVI_LAST;
			tvis.hParent = _lastInserted;
			_lastInserted = TreeView_InsertItem(_treeView,&tvis);
		}
		if(_currentNode.lParam > _lastHeight + 1 || _currentNode.lParam < 0) //error
		{
			throw Win::Exception ("Error Reading XML File");
		}
		if(_currentNode.lParam == _lastHeight) //sibling of previous inserted
		{
			TVINSERTSTRUCT tvis;
			tvis.itemex = _currentNode;
			tvis.hInsertAfter = TVI_LAST;
			tvis.hParent = TreeView_GetParent(_treeView, _lastInserted);
			_lastInserted = TreeView_InsertItem(_treeView,&tvis);			
		}
		if(_currentNode.lParam < _lastHeight)//sibling of more previously inserted
		{
			while(_lastHeight > _currentNode.lParam)
			{
				_lastInserted = TreeView_GetParent(_treeView, _lastInserted);
				_lastHeight = _lastHeight - 1;
				Assert(_lastHeight > 0);
			}
			TVINSERTSTRUCT tvis;
			tvis.itemex = _currentNode;
			tvis.hInsertAfter = TVI_LAST;
			tvis.hParent = TreeView_GetParent(_treeView, _lastInserted);
			_lastInserted = TreeView_InsertItem(_treeView,&tvis);	
		}
		_lastHeight = _currentNode.lParam;
		_isCurrentNodeOpen = false;
	}//end of attrib == text
}