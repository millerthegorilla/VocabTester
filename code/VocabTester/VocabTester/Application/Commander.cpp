//----------------------------
// (c) Reliable Software, 2004
//----------------------------
#include "precompiled.h"
#include "Commander.h"
#include "TopCtrl.h"
#include "About.h"

void Commander::Program_Exit () 
{
	_ctrl.GetWindow ().Destroy ();
}

void Commander::Program_About ()
{
	AboutDlgCtrl ctrl;
	Dialog::Modal dlg (_ctrl.GetWindow (), ctrl);
}