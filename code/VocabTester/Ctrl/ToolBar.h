#if !defined (TOOLBAR_H)
#define TOOLBAR_H
//--------------------------------
// (c) Reliable Software 1998-2000
//--------------------------------
#include <Ctrl/Tooltip.h>
#include <Ctrl/Command.h>
#include <Ctrl/Controls.h>
#include <Graph/ImageList.h>

namespace Tool
{
	class Button;

	class Handle : public Win::SimpleControl
	{
	public:
		Handle (Win::Dow::Handle win = 0)
			: Win::SimpleControl (win)
		{}

		int Height () const;
		int ButtonCount () const
		{
			return SendMsg (TB_BUTTONCOUNT);
		}
		// default is 24x22 pixels
		void SetButtonSize (int width, int height)
		{
			SendMsg (TB_SETBUTTONSIZE, 0, MAKELONG(width, height));
		}
		void SetImageList (ImageList::Handle imageList)
		{
			SendMsg (TB_SETIMAGELIST, 0, (LPARAM)imageList.ToNative ());
		}
		void AddButtons (std::vector<Button> & buttons)
		{
			if (buttons.size () != 0)
				SendMsg (TB_ADDBUTTONS, buttons.size (), (LPARAM) &buttons [0]);
		}
		void AutoSize ()
		{
			SendMsg (TB_AUTOSIZE);
		}
		int  CmdIdToButtonIndex (int cmdId);
		void GetButtonRect (int index, Win::Rect & rect);
		void ClearButtons ();
		void AddWindow (Win::Dow::Handle hwndTool);
		int  GetToolTipDelay ();
		void SetToolTipDelay (int miliSeconds);
		// Button manipulation
		bool Delete (int idx)
		{
			return SendMsg (TB_DELETEBUTTON, idx) != FALSE;
		}
		void Enable (int cmdID)
		{
			SendMsg (TB_ENABLEBUTTON, (WPARAM) cmdID, MAKELPARAM (TRUE, 0));
		}

		void Disable (int cmdID)
		{
			SendMsg (TB_ENABLEBUTTON, (WPARAM) cmdID, MAKELPARAM (FALSE, 0));
		}

		void Hide (int cmdID)
		{
			SendMsg (TB_HIDEBUTTON, (WPARAM) cmdID, MAKELPARAM (TRUE, 0));
		}

		void Show (int cmdID)
		{
			SendMsg (TB_HIDEBUTTON, (WPARAM) cmdID, MAKELPARAM (FALSE, 0));
		}

		void Press (int cmdID)
		{
			SendMsg (TB_PRESSBUTTON, (WPARAM) cmdID, MAKELPARAM (TRUE, 0));
		}

		void Release (int cmdID)
		{
			SendMsg (TB_PRESSBUTTON, (WPARAM) cmdID, MAKELPARAM (FALSE, 0));
		}
		
		void InsertSeparator (int idx, int width);
	};

	class Style: public Win::Style
	{
	public:
		enum Bits
		{
		    Tips = TBSTYLE_TOOLTIPS,
		    AlignBottom = CCS_BOTTOM,
		    Flat = TBSTYLE_FLAT,
		    Transparent = TBSTYLE_TRANSPARENT,
		    NoMoveY = CCS_NOMOVEY // don't move vertically when auto sizing
		};
	};

	inline Win::Style & operator<<(Win::Style & style, Tool::Style::Bits bits)
	{
		style.OrIn (static_cast<Win::Style::Bits> (bits));
		return style;
	}

	class Maker : public Win::ControlMaker
	{
	public:
		Maker (Win::Dow::Handle winParent, int id)
			: ControlMaker(TOOLBARCLASSNAME , winParent, id)
		{
			Win::CommonControlsRegistry::Instance()->Add(Win::CommonControlsRegistry::BAR);
		}

		Tool::Handle Create () 
		{
			Tool::Handle h = ControlMaker::Create ();
			h.SendMsg (TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON));
			return h;
		}
	};

	class Item
	{
	public:
		enum { idSeparator = -1, idEnd = -2 };
	public:
		int buttonId;           // Zero based button id (left-most = 0)
		char const * cmdName;   // Command
		char const * tip;       // Tool tip
	};

	// Simple Tool::Bar with tooltips
	class Bar: public Tool::Handle
	{
	public:
		Bar (Win::Dow::Handle winParent,
			int toolbarId,
			int bitmapId,
			int buttonWidth,
			Cmd::Vector const & cmdVector,
			bool topAligh = false);

		// Add button descriptions and lay them out in order
		void SetButtons (Tool::Item const * layout);

		int GetButtonsEnd ();
		int GetButtonWidth (int idx);

		void Enable () throw ();
		void Disable () throw ();
		void FillToolTip (Tool::TipForCtrl * tip) const;
		bool IsCmdButton (int cmdId) const;		 
	protected:
		ImageList::AutoHandle	_imageList;
		Cmd::Vector const &		_cmdVector;
		// these two are parallel
		Tool::Item const *		_buttonItems;
		std::vector<int>		_cmdIds;
	};

	class MultiBar: public Bar
	{
	public:
		MultiBar (Win::Dow::Handle winParent,
					int toolbarId,
					int bitmapId,
					int buttonWidth,
					Cmd::Vector const & cmdVector,
					bool topAlign = false)
		  : Bar (winParent, toolbarId, bitmapId, buttonWidth, cmdVector, topAlign)
		{}
		// Add button descriptions to be used in different layouts
		void SetButtonDescriptions (Tool::Item const * layout);
		void SetLayout (int const * layout);
	};

	class Button: public TBBUTTON
	{
	public:
		Button ();
	};

	class BarSeparator : public Button
	{
	public:
		BarSeparator (int width = 0);
	};

	class BarButton : public Button
	{
	public:
		BarButton (int buttonId, int cmdId);
	};

	// Defines tool window (hwndTool) placed on the toolbar (hwndToolBar)
	class ToolWindow : public TOOLINFO
	{
	public:
		ToolWindow (Win::Dow::Handle hwndTool, Win::Dow::Handle hwndToolBar);
	};
}

#endif
