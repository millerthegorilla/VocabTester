//----------------------------
// (c) Reliable Software, 2005
//----------------------------
#include "precompiled.h"
#include "TvTest.h"
#include <Com/Shell.h>
#include <StringOp.h>
#include <sstream>

bool TvTest::OnInitDialog () throw ()
{
	_edit.Init( GetWindow(), IDC_EDIT1);
	std::ostringstream editText;
	editText << "&hItem = " << ToString(_tvitem.hItem) << "\n" 
		<< "iImage = " << ToString(_tvitem.iImage) << "\n" 
		<< "iSelected = " << ToString(_tvitem.iSelectedImage) << "\n" 
		<< "cChildren = " << ToString(_tvitem.cChildren);

	_edit.SetString(editText.str().c_str());
	return true;
}

bool TvTest::OnApply () throw ()
{
	EndOk ();
	return true;
}
