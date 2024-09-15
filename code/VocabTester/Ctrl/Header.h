#if !defined (HEADER_H)
#define HEADER_H
//--------------------------------
// (c) Reliable Software 2001
//--------------------------------

#include "Controls.h"
#include <Graph/Canvas.h>

//
//	Header_... are Windows API's but they can't be prefixed with :: because
//	Windows implements them as macros...
//

namespace Win
{
	class Header : public SimpleControl
	{
	public:
		class Style : public Win::Style
		{
		public:
			Header::Style & operator<<(void (Header::Style::*method)())
			{
				(this->*method)();
				return *this;
			}

			void Buttons()	//	SetButtons()
			{
				_style |= HDS_BUTTONS;
			}

			void DragDrop()	//	SetDragDrop()
			{
				_style |= HDS_DRAGDROP;
			}

			void FullDrag()	//	SetFullDrag()
			{
				_style |= HDS_FULLDRAG;
			}

			void FilterBar()	//	SetFilterBar()
			{
				_style |= HDS_FILTERBAR;
			}

			void Hidden()	//	SetHidden()
			{
				_style |= HDS_HIDDEN;
			}

			void Horizontal()	//	SetHorizontal()
			{
				_style |= HDS_HORZ;
			}

			void HotTracking()	//	SetHotTracking()
			{
				_style |= HDS_HOTTRACK;
			}
		};

		int AppendItem(char const *pszTitle, int width)
		{
			HDITEM hdi;
			hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
			hdi.cxy = width;
			hdi.pszText = const_cast<char *> (pszTitle);
			hdi.cchTextMax = strlen(pszTitle);
			hdi.fmt = HDF_STRING | HDF_LEFT;

			return Header_InsertItem(_h, Header_GetItemCount(_h), &hdi);
		}
		void GetItemRect(int item, Win::Rect& rc) const
		{
			Header_GetItemRect(*this, item, &rc);
		}
		void SetItemWidth(int item, int width)
		{
			//	now set the width of this column
			HDITEM hdi;
			hdi.mask = HDI_WIDTH;
			hdi.cxy = width;
			Header_SetItem(*this, item, &hdi);
		}
	};

	class HeaderMaker : public ControlMaker
	{
	public:
		HeaderMaker(Win::Dow::Handle winParent, int id)
			: ControlMaker(WC_HEADER, winParent, id)
		{
			CommonControlsRegistry::Instance()->Add(CommonControlsRegistry::LISTVIEW);
		}

		static char const *ClassName() throw()
		{
			return WC_HEADER;
		}

		Win::Header::Style & Style() { return static_cast<Win::Header::Style &> (_style); }
	};
}

#endif