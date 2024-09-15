#if !defined (COLOR_H)
#define COLOR_H
// -----------------------------
// (c) Reliable Software, 2000
// -----------------------------

namespace Win
{
	class Color
	{
	public:
		Color (COLORREF color = 0) : _color (color) {}
		Color (int r, int g, int b)
			: _color (RGB (r, g, b))
		{}
		void Init (int r, int g, int b)
		{
			_color = RGB (r, g, b);
		}

		operator COLORREF () const { return _color; }
		static Color Window () { return Color (::GetSysColor (COLOR_WINDOW)); }
		static Color _3DFace () { return Color (::GetSysColor (COLOR_3DFACE)); }
		static Color ToolTipBkg () { return Color (::GetSysColor (COLOR_INFOBK)); }

		// Note: Windows GetRValue, etc, macros cause runtime warnings
		int R () const { return _color & 0xff; }
		int G () const { return (_color >> 8) & 0xff; }
		int B () const { return (_color >> 16) & 0xff; }

	private:
		COLORREF _color;
	};
}

#endif
