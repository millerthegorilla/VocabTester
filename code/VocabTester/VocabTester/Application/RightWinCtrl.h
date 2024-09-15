#if !defined(RIGHTWINCTRL_H)
#define RIGHTWINCTRL_H

#include <Win/Controller.h>
#include <Graph/ImageList.h>
#include <Graph/Icon.h>
#include <Graph/Canvas.h>
#include "resource.h"
#include <Win/Message.h>

class RightWinCtrl : public Win::SubController  
{
public:
	RightWinCtrl() : _initMsg ("InitMessage"), _imageList(32,32,9,0,0)
	{

	}
	bool OnCreate (Win::CreateData const * create, bool & success) throw ();
	bool OnRegisteredMessage (Win::Message & msg) throw ();
	void OnInit();
	bool OnLButtonDown(int x, int y, Win::KeyState kState);
	bool OnSize (int width, int height, int flag) throw ();
	bool OnPaint () throw ();
private:
	ImageList::AutoHandle			_imageList;
	std::vector <Icon::Handle >		_iconVector;
	std::auto_ptr <Win::Canvas>		_canvas;
	Win::RegisteredMessage			_initMsg;
	int								_imageindex;
};




#endif