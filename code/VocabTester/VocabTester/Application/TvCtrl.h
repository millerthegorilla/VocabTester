#if !defined(TVCTRL_H)
#define TVCTRL_H

#include <Win/Controller.h>
#include "TvCtxCmdVector.h"
#include "TvCtxMenuTable.h"
#include "TvCtxCommander.h"
#include <Ctrl/Menu.h>
#include <Ctrl/Tree.h>
#include "LeftWinCtrl.h"

class LeftWinCtrl;

class TvCtrl : public Win::SubController  
{
public:
	TvCtrl(LeftWinCtrl & leftwinctrl);
	bool OnRButtonDown(int x, int y, Win::KeyState kState);
	bool OnLButtonDown(int x, int y, Win::KeyState kState);
	bool OnCommand(int cmdId, bool isAccel) throw ();
	bool OnInitPopup (Menu::Handle menu, int pos) throw ();
private:
	std::auto_ptr<Menu::Context>	_treeContext;
	std::auto_ptr<TvCtxCommander>	_tvCtxCommander;
	std::auto_ptr<TvCtxCmdVector>	_tvCtxCmdVector;
	std::vector<Menu::Item const *> _templ;
};


#endif