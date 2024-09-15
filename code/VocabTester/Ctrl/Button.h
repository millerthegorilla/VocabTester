#if !defined (BUTTON_H)
#define BUTTON_H
//--------------------------------
// (c) Reliable Software 1998-2001
//--------------------------------

#include "Controls.h"

namespace Win
{
	class Button: public SimpleControl
	{
	public:
		enum Notifications
		{
			Clicked = BN_CLICKED
		};

		class Style: public Win::Style
		{
		public:
			enum Bits
			{
				AutoCheck = BS_AUTOCHECKBOX,
				PushLike = BS_PUSHLIKE
			};
		};
	public:
		Button () {}
		Button (Win::Dow::Handle winParent, int id)
			: SimpleControl (winParent, id)
		{}

		void Push ()
		{
			SendMsg (BM_SETSTATE, 1);
		}

		void Pop ()
		{
			SendMsg (BM_SETSTATE);
		}
	};

	inline Win::Style & operator<<(Win::Style & style, Button::Style::Bits bits)
	{
		style.OrIn (static_cast<Win::Style::Bits> (bits));
		return style;
	}

	class RadioButton: public SimpleControl
	{
	public:
		RadioButton () {}
		RadioButton (Win::Dow::Handle winParent, int id)
			: SimpleControl (winParent, id)
		{}

		void Check()
		{
			SendMsg (BM_SETCHECK, 1);
		}
		void UnCheck()
		{
			SendMsg (BM_SETCHECK);
		}
		bool IsChecked () const
		{
			return (BST_CHECKED == SendMsg (BM_GETCHECK));
		}
	};

	class CheckBox: public SimpleControl
	{
	public:
		CheckBox () {}
		CheckBox (Win::Dow::Handle winParent, int id)
			: SimpleControl (winParent, id)
		{}

		bool IsChecked () const
		{
			return (BST_CHECKED == SendMsg (BM_GETCHECK));
		}
		void Check()
		{
			SendMsg (BM_SETCHECK, (WPARAM)1);
		}
		void UnCheck()
		{
			SendMsg (BM_SETCHECK);
		}
	};
	
	class ButtonMaker : public ControlMaker
	{
	public:
		ButtonMaker(Win::Dow::Handle winParent, int id)
			: ControlMaker("BUTTON", winParent, id)
		{
		}
	};
}

#endif
