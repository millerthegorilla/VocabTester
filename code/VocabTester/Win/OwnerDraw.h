#if !defined (OWNERDRAW_H)
#define OWNERDRAW_H
// (c) Reliable Software 2002 -- 2003

#include <Graph/Canvas.h>
#include <Bit.h>

namespace OwnerDraw
{
	// use for testing Action ().IsSet (...)
	enum ActionBits
	{
		DrawEntire = ODA_DRAWENTIRE,
		Select = ODA_SELECT,
		Focus = ODA_FOCUS
	};
	// use for testing State ().IsSet (...)
	enum StateBits
	{
		Default = ODS_DEFAULT,
		Disabled = ODS_DISABLED,
		HasFocus = ODS_FOCUS,
		Selected = ODS_SELECTED,

		MenuSelected = ODS_SELECTED,
		MenuChecked = ODS_CHECKED,
		MenuGrayed = ODS_GRAYED,

		ComboBoxEdit = ODS_COMBOBOXEDIT // edit control of a combobox
	};

	enum CtrlType
	{
		ComboBox = ODT_COMBOBOX,
		ListBox = ODT_LISTBOX,
		ListView = ODT_LISTVIEW,
		Static = ODT_STATIC,
		Tab = ODT_TAB,
		Button = ODT_BUTTON,
		Menu = ODT_MENU
	};

	class Item
	{
	public:

	public:
		Item (DRAWITEMSTRUCT *draw)
			: _draw (draw)
		{}
		OwnerDraw::CtrlType ControlType () const { return static_cast<CtrlType> (_draw->CtlType); }
		unsigned CtrlId () const { return _draw->CtlID; }
		unsigned ItemId () const { return _draw->itemID; }
		unsigned long ItemData () const { return _draw->itemData; }
		BitField<OwnerDraw::ActionBits> Action () const 
		{
			return BitField<OwnerDraw::ActionBits> (_draw->itemAction);
		}
		BitField<OwnerDraw::StateBits>  State ()  const
		{
			return BitField<OwnerDraw::StateBits> (_draw->itemState);
		}
		Win::Dow::Handle Window () const { return _draw->hwndItem; }
		Win::Rect const & Rect() const
		{
			return static_cast<Win::Rect const &> (_draw->rcItem);
		}
		Win::Canvas Canvas () const { return _draw->hDC; }
	private:
		DRAWITEMSTRUCT * _draw;
	};

	class Handler
	{
	public:
		Handler (Win::Dow::Handle winParent = Win::Dow::Handle::NullValue (), int ctrlId = -1)
		{
			Attach (winParent, ctrlId);
		}
		void Attach (Win::Dow::Handle winParent, int ctrlId);
		virtual ~Handler () {}
		Win::Dow::Handle Parent () const { return _winParent; }
		int CtrlId () const { return _ctrlId; }

		virtual bool Draw (OwnerDraw::Item & item) throw () = 0;
	private:
		Win::Dow::Handle	_winParent;
		int					_ctrlId;
	};
}
#endif
