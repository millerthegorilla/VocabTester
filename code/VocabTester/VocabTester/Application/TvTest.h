#if !defined (TVTEST_H)
#define TVTEST_H
//----------------------------
// (c) Reliable Software, 2005
//----------------------------
#include "../Resource/resource.h"
#include <Win/Dialog.h>
#include <Ctrl/Tree.h>
#include <Ctrl/Edit.h>

class TvTest : public Dialog::ModalController
{
public:
	TvTest (TVITEMEX & tvitem) : Dialog::ModalController (IDD_TVTEST),
										_tvitem(tvitem)
	{}
	bool OnInitDialog() throw ();
	bool OnApply () throw ();
private:
	Win::Edit		_edit;
	TVITEMEX & _tvitem;
};

#endif
