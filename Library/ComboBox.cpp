#include "ComboBox.h"

using namespace Win;

bool ComboBoxNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case CBN_DBLCLK:
      if ( OnDoubleClick(hdr->hwndFrom) ) return true;
      break;

    case CBN_ERRSPACE:
      if ( OnOutOfMemory(hdr->hwndFrom) ) return true;
      break;

    case CBN_KILLFOCUS:
      if ( OnKillFocus(hdr->hwndFrom) ) return true;
      break;

    case CBN_SELENDCANCEL:
      if ( OnSelectionEndCancel(hdr->hwndFrom) ) return true;
      break;

    case CBN_SELENDOK:
      if ( OnSelectionEndOk(hdr->hwndFrom) ) return true;
      break;

    case CBN_SELCHANGE:
      if ( OnSelectionChanged(hdr->hwndFrom) ) return true;
      break;

    case CBN_SETFOCUS:
      if ( OnSetFocus(hdr->hwndFrom) ) return true;
      break;

    case CBN_CLOSEUP:
      if ( OnCloseUp(hdr->hwndFrom) ) return true;
      break;

    case CBN_DROPDOWN:
      if ( OnDropDown(hdr->hwndFrom) ) return true;
      break;

    case CBN_EDITCHANGE:
      if ( OnEditChanged(hdr->hwndFrom) ) return true;
      break;

    case CBN_EDITUPDATE:
      if ( OnEditUpdate(hdr->hwndFrom) ) return true;
      break;
  }
  return NotifyHandler::OnNotify(hdr, result);
}