#include "NotifyHandler.h"

using namespace Win;

bool CommonNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case NM_CHAR:
      if ( OnChar(hdr->hwndFrom, reinterpret_cast<NMCHAR*>(hdr), result) ) return true;
      break;

    case NM_CLICK:
      if ( OnClick(hdr->hwndFrom, result) ) return true;
      break;

    case NM_DBLCLK:
      if ( OnDoubleClick(hdr->hwndFrom, result) ) return true;
      break;

    case NM_HOVER:
      result = 0; // Default processing
      if ( OnHover(hdr->hwndFrom, result) ) return true;
      break;

    case NM_KEYDOWN:
      {
        NMKEY* data = reinterpret_cast<NMKEY*>(hdr);
        result = 0; // Default processing
        if ( OnKeyDown(hdr->hwndFrom, data->nVKey, KeyData(data->uFlags), result) ) return true;
      }
      break;

    case NM_KILLFOCUS:
      if ( OnKillFocus(hdr->hwndFrom, result) ) return true;
      break;

    case NM_NCHITTEST:
      result = 0; // Default processing
      if ( OnNCHitTest(hdr->hwndFrom, reinterpret_cast<NMMOUSE*>(hdr), result) ) return true;
      break;

    case NM_OUTOFMEMORY:
      if ( OnOutOfMemory(hdr->hwndFrom, result) ) return true;
      break;

    case NM_RCLICK:
      result = 0; // Default processing
      if ( OnRightClick(hdr->hwndFrom, result) ) return true;
      break;

    case NM_RDBLCLK:
      if ( OnRightDoubleClick(hdr->hwndFrom, result) ) return true;
      break;

    case NM_RELEASEDCAPTURE:
      result = 0; // Default processing
      if ( OnReleasedCapture(hdr->hwndFrom, result) ) return true;
      break;

    case NM_RETURN:
      if ( OnReturn(hdr->hwndFrom, result) ) return true;
      break;

    case NM_SETCURSOR:
      result = 1; // Allow set the cursor
      if ( OnSetCursor(hdr->hwndFrom, reinterpret_cast<NMMOUSE*>(hdr), result) ) return true;
      break;

    case NM_SETFOCUS:
      if ( OnSetFocus(hdr->hwndFrom, result) ) return true;
      break;
/*
    // Windows 2000 +
    case NM_TOOLTIPSCREATED:
      if ( OnToolTipsCreated(hdr->hwndFrom, reinterpret_cast<NMTOOLTIPSCREATED*>(hdr), result) ) return true;
      break;
*/
  }
  return false;
}