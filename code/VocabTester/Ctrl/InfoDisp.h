#if !defined INFODISP_H
#define INFODISP_H
//------------------------------
// (c) Reliable Software 1997-00
//------------------------------
#include <Ctrl/Controls.h>
#include <Graph/Font.h>
#include <Graph/Canvas.h>

namespace Win
{
	class InfoDisplay : public Win::ControlWithFont
	{
	public:
		class Style: public Win::Style
		{
		public:
			enum Bits
			{
				Sunken = SS_SUNKEN,
				Notify = SS_NOTIFY,
				LeftNoWordWrap = SS_LEFTNOWORDWRAP,
				NoPrefix = SS_NOPREFIX,
				PathEllipsis = SS_PATHELLIPSIS,
				WordEllipsis = SS_WORDELLIPSIS
			};
		};
	public:
		InfoDisplay (Win::Dow::Handle winParent, int id)
			: Win::ControlWithFont (winParent, id)
		{}
		InfoDisplay (Win::Dow::Handle win = 0)
			: Win::ControlWithFont (win)
		{}

		void ReSize (int left, int top, int width, int height)
		{
			Move (left, top, width, height);
		}

		void GetTextSize (char const * text, int & textLength, int & textHeight)
		{
			Win::UpdateCanvas canvas (H ());
			canvas.GetTextSize (text, textLength, textHeight);
		}
	};

	inline Win::Style & operator<< (Win::Style & style, Win::InfoDisplay::Style::Bits bits)
	{
		style.OrIn (static_cast<Win::Style::Bits> (bits));
		return style;
	}

	class InfoDisplayMaker : public Win::ControlMaker
	{
	public:
		InfoDisplayMaker (Win::Dow::Handle winParent, int id)
			: ControlMaker ("static", winParent, id)
		{
			Style () << InfoDisplay::Style::Sunken
					 << InfoDisplay::Style::Notify
					 << InfoDisplay::Style::LeftNoWordWrap
					 << InfoDisplay::Style::NoPrefix
					 << InfoDisplay::Style::PathEllipsis
					 << InfoDisplay::Style::WordEllipsis;
		}
	};

}

#endif
