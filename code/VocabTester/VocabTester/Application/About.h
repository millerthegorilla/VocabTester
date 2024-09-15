#if !defined (ABOUT_H)
#define ABOUT_H
//----------------------------
// (c) Reliable Software, 2005
//----------------------------
#include "../Resource/resource.h"
#include <Win/Dialog.h>

class AboutDlgCtrl : public Dialog::ModalController
{
public:
	AboutDlgCtrl () : Dialog::ModalController (IDD_ABOUT) {}
    bool OnControl (Win::Dow::Handle win, int ctrlId, int notifyCode) throw (Win::Exception);
	bool OnApply () throw ();
};

#endif
