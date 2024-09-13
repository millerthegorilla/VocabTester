#include "Status.h"

using namespace Win;

bool StatusBarNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case NM_CLICK:
      result = FALSE; // Default processing
      if ( OnClick(hdr->hwndFrom, reinterpret_cast<NMMOUSE*>(hdr), result) ) return true;
      break;

    case NM_DBLCLK:
      result = FALSE; // Default processing
      if ( OnDoubleClick(hdr->hwndFrom, reinterpret_cast<NMMOUSE*>(hdr), result) ) return true;
      break;

    case NM_RCLICK:
      result = FALSE; // Default processing
      if ( OnRightClick(hdr->hwndFrom, reinterpret_cast<NMMOUSE*>(hdr), result) ) return true;
      break;

    case NM_RDBLCLK:
      result = FALSE; // Default processing
      if ( OnRightDoubleClick(hdr->hwndFrom, reinterpret_cast<NMMOUSE*>(hdr), result) ) return true;
      break;

    case SBN_SIMPLEMODECHANGE:
      if ( OnSimpleModeChange(hdr->hwndFrom) ) return true;
      break;
  }
  return CommonNotifyHandler::OnNotify(hdr, result);
}