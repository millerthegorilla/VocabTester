#include "ListBox.h"

using namespace Win;

bool ListBoxNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case LBN_DBLCLK:
      if ( OnDoubleClick(hdr->hwndFrom) ) return true;
      break;

    case LBN_ERRSPACE:
      if ( OnOutOfMemory(hdr->hwndFrom) ) return true;
      break;

    case LBN_KILLFOCUS:
      if ( OnKillFocus(hdr->hwndFrom) ) return true;
      break;

    case LBN_SELCANCEL:
      if ( OnSelectionCancel(hdr->hwndFrom) ) return true;
      break;

    case LBN_SELCHANGE:
      if ( OnSelectionChange(hdr->hwndFrom) ) return true;
      break;

    case LBN_SETFOCUS:
      if ( OnSetFocus(hdr->hwndFrom) ) return true;
      break;
  }
  return NotifyHandler::OnNotify(hdr, result);
}