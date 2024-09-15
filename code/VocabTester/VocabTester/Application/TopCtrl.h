//-----------------------------
//  (c) Reliable Software, 2004
//-----------------------------
#include "CmdVector.h" // CmdVector is a typedef, not a class
#include <Win/Controller.h>
#include <Win/Message.h>
#include <Ctrl/Accelerator.h>
#include "LeftWinCtrl.h"
#include "RightWinCtrl.h"
namespace Win { class MessagePrepro; }
namespace Hierarchy { class Controller; }
class Commander;
class TvCtxCommander;
class View;

class TopCtrl: public Win::Controller
{
public:
	TopCtrl (char const * cmdLine, Win::MessagePrepro & msgPrepro);
	~TopCtrl ();
	Notify::Handler * GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw ();
	bool OnCreate (Win::CreateData const * create, bool & success) throw ();
	bool OnDestroy () throw ();
	bool OnSize (int width, int height, int flag) throw ();
	bool OnRegisteredMessage (Win::Message & msg) throw ();
	bool OnInitPopup (Menu::Handle menu, int pos) throw ();
	bool OnMenuSelect (int id, Menu::State state, Menu::Handle menu) throw ();
	bool OnCommand (int cmdId, bool isAccel) throw ();
	bool OnUserMessage (Win::UserMessage & msg) throw ();
	bool OnLButtonDown (int x, int y, Win::KeyState kState) throw ();
private:
	void OnInit ();
	void MenuCommand (int cmdId);
private:
	
	enum { splitWidth = 8 };	// width of splitter

	bool							_ready;
	std::string						_cmdLine;
	Win::MessagePrepro			  & _msgPrepro;
	Win::RegisteredMessage			_initMsg;
	std::auto_ptr<Commander>		_commander;
	std::auto_ptr<CmdVector>		_cmdVector;
	Accel::AutoHandle				_kbdAccel;
	std::auto_ptr<Menu::DropDown>	_menu;
	std::auto_ptr<View>				_view;
	Win::Dow::Handle				_leftWinHandle;
	Win::Dow::Handle				_rightWinHandle;
	Win::Dow::Handle				_splitterHandle;
	LeftWinCtrl						_leftWinCtrl;
	RightWinCtrl					_rightWinCtrl;
	int				_cx;
	int				_cy;
	int				_splitRatio;	// in per cent
};
