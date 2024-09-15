//---------------------------
// (c) Reliable Software 2005
//---------------------------
#include <WinLibBase.h>
#include "WinResource.h"

Resource::Resource (Win::Dow::Handle owner, int resId, char const * type)
	: _hRes (0),
	  _hGlob (0)
{
	_hRes = ::FindResource (owner.GetInstance (), MAKEINTRESOURCE (resId), type);
	if (_hRes != 0)
	{
		_hGlob = ::LoadResource (owner.GetInstance (), _hRes);
	}
}

