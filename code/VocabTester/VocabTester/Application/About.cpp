//----------------------------
// (c) Reliable Software, 2005
//----------------------------
#include "precompiled.h"
#include "About.h"
#include <Com/Shell.h>

bool AboutDlgCtrl::OnControl (Win::Dow::Handle win, int ctrlId, int notifyCode) throw (Win::Exception)
{
	switch (ctrlId)
	{
	case IDB_RELISOFT:
		ShellMan::Open (win, "http://www.relisoft.com");
		return true;
	}
    return false;
}

bool AboutDlgCtrl::OnApply () throw ()
{
	EndOk ();
	return true;
}
