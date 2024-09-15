#if !defined (FONT_H)
#define FONT_H
// --------------------------------------
// Reliable Software (c) 1998 -- 2002
// --------------------------------------

#include <Graph/StockObj.h>
#include <Graph/Color.h>
#include <Graph/Canvas.h>

namespace Font
{
	// Font weight is between 0 and 1000. These are the predefined values
	//
	// FW_DONTCARE 
	// FW_THIN 
	// FW_EXTRALIGHT 
	// FW_LIGHT 
	// FW_NORMAL 
	// FW_MEDIUM 
	// FW_SEMIBOLD 
	// FW_BOLD 
	// FW_EXTRABOLD 
	// FW_HEAVY 

	// Revisit: add method to initialize LOGFONT using ::GetObject
	class Descriptor: public LOGFONT
	{
	public:
		void SetDefault ();
		char const * GetFaceName () const { return &lfFaceName [0]; }
		int GetHeight () const { return lfHeight; }
		int GetWeight () const { return lfWeight; }
		bool IsItalic () const { return lfItalic != 0; }
	};

	class Maker: public Descriptor
	{
	public:
		Maker ();
		Maker (int pointSize, std::string const & faceName);
		Maker (Font::Descriptor const & newFont);
		void Init (int pointSize, std::string const & faceName);
		int GetPointSize () const { return _pointSize; }
		void SetFaceName (std::string const & faceName);
		void SetPointSize (int pointSize);
		void ScaleUsing (Win::Canvas canvas);
		void MakeBold ()
		{
			lfWeight = FW_BOLD;
		}
		void MakeHeavy ()
		{
			lfWeight = FW_HEAVY;
		}
		void SetWeight (int weigth)
		{
			lfWeight = weigth;
		}
		void SetHeight (int height);
		void SetItalic (bool flag)
		{
			lfItalic = flag ? TRUE : FALSE;
		}
		void SetCharSet (int charSet)
		{
			lfCharSet = charSet;
		}

		Font::AutoHandle Create ();

	private:
		void UpdatePointSize ();
	private:
		int			_pointSize;
	};

	class Holder: public Gdi::ObjectHolder<Font::Handle>
	{
	public:
		Holder (Win::Canvas canvas, Font::Handle font)
			: Gdi::ObjectHolder<Font::Handle> (canvas, font)
		{}
		void GetAveCharSize(int &aveCharWidth, int &aveCharHeight);
		void GetBaseUnits (int &baseUnitX, int &baseUnitY);
		int  GetCharWidthTwips (char c, int pointSize);
	};

	class ColorHolder
	{
	public:
		ColorHolder (Win::Canvas canvas, Win::Color color)
			: _canvas (canvas), 
				_oldColor (::SetTextColor (_canvas.ToNative (), color))
		{}
		~ColorHolder ()
		{
			::SetTextColor (_canvas.ToNative (), _oldColor);
		}
	private:
		Win::Canvas	_canvas;
		COLORREF	_oldColor;
	};

	class BkgHolder
	{
	public:
		BkgHolder (Win::Canvas canvas, Win::Color color)
			: _canvas (canvas), 
				_oldColor (::SetBkColor (_canvas.ToNative (), color))
		{}
		~BkgHolder ()
		{
			::SetBkColor (_canvas.ToNative (), _oldColor);
		}
	private:
		Win::Canvas	_canvas;
		COLORREF	_oldColor;
	};

	enum StockType
	{
		StockSystem = SYSTEM_FONT,
		StockSystemFixed = SYSTEM_FIXED_FONT,
		StockProportional = ANSI_VAR_FONT
	};

	typedef Stock::Object<Font::Handle, Font::StockType> StockObject;

	class SysFixed: public StockObject
	{
	public:
		SysFixed () : StockObject (Font::StockSystemFixed) {}
	};
	
	typedef Stock::ObjectHolder<Font::Holder, Font::StockType> StockHolder;
}

#endif
