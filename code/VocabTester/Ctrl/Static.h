#if !defined (STATIC_H)
#define STATIC_H
//----------------------------------------------------
// (c) Reliable Software 2001 -- 2003
//----------------------------------------------------

#include "Controls.h"
#include <Graph/Icon.h>
#include <Graph/Bitmap.h>

namespace Win
{
	class StaticText: public SimpleControl
	{
	public:
		StaticText () {}
		StaticText (Win::Dow::Handle winParent, int id)
			: SimpleControl (winParent, id)
		{}
		std::string GetString () const
		{
			unsigned len = SendMsg (WM_GETTEXTLENGTH);
			std::string text;
			text.resize (len);
			SendMsg (WM_GETTEXT, static_cast<WPARAM>(len + 1), reinterpret_cast<WPARAM>(&text [0]));
			return text;
		}
	};

	class StaticImage: public SimpleControl
	{
	public:
		StaticImage () {}
		StaticImage (Win::Dow::Handle winParent, int id)
			: SimpleControl (winParent, id)
		{}

		void SetIcon (Icon::Handle icon)
		{
			SendMsg (STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(icon.ToNative ()));
		}
		Bitmap::Handle GetBitmap ()
		{
			return reinterpret_cast<HBITMAP> (SendMsg (STM_GETIMAGE, IMAGE_BITMAP));
		}
		void ReplaceBitmapPixels (Win::Color colorFrom, Win::Color colorTo = Win::Color::_3DFace ())
		{
			Bitmap::Handle hbmImage = GetBitmap ();
			Assert (!hbmImage.IsNull ());
			hbmImage.ReplacePixels (colorFrom, colorTo);
		}
	};

	class StaticMaker : public ControlMaker
	{
	public:
		StaticMaker(Win::Dow::Handle winParent, int id)
			: ControlMaker("STATIC", winParent, id)
		{
		}
	};
}

#endif
