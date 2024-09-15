#if !defined (ICON_H)
#define ICON_H
// -----------------------------------
// (c) Reliable Software, 2000 -- 2004
// -----------------------------------

namespace Win { class Instance; }

namespace Icon
{
	class Maker
	{
	public:
	    Maker (int width = 0, int height = 0)
			: _options (0),
			  _width (width),
			  _height (height)
		{}
		void SetSize (int width, int height)
		{
			_width = width;
			_height = height;
		}
		// set other options

		Icon::AutoHandle Load (Win::Instance inst, unsigned id);
		Icon::AutoHandle Load (char const * iconName);

	protected:
		unsigned _options;
		int		_width;
		int		_height;
	};

	class SmallMaker: public Maker
	{
	public:
		SmallMaker ();
	};

	class StdMaker: public Maker
	{
	public:
		StdMaker ();
	};
}

#endif
