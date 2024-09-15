// -----------------------------------
// (c) Reliable Software, 2000 -- 2004
// -----------------------------------

#include <WinLibBase.h>

#include "Icon.h"
#include <Win/Instance.h>

template<>
void Win::Disposal<Icon::Handle>::Dispose (Icon::Handle h) throw ()
{
	::DestroyIcon (h.ToNative ());
}

namespace Icon
{
	AutoHandle Maker::Load (Win::Instance inst, unsigned id)
	{
		return inst.LoadIcon (id, _width, _height, _options);
	}

	AutoHandle Maker::Load (char const * iconName)
	{
		HICON hicon = reinterpret_cast<HICON> (::LoadImage (0, iconName, IMAGE_ICON, _width, _height, _options | LR_SHARED));
		if (hicon == 0)
			throw Win::Exception ("System icon load failed");
		return Icon::AutoHandle (hicon);
	}

	SmallMaker::SmallMaker ()
		: Maker (::GetSystemMetrics (SM_CXSMICON),
				 ::GetSystemMetrics (SM_CYSMICON))
	{}

	StdMaker::StdMaker ()
		: Maker (::GetSystemMetrics (SM_CXICON),
				 ::GetSystemMetrics (SM_CYICON))
	{}
}