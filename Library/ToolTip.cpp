#include "ToolTip.h"

using namespace Win;

bool ToolTipNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case NM_CUSTOMDRAW:
      if ( OnCustomDraw(hdr->hwndFrom, *reinterpret_cast<NMTTCUSTOMDRAW*>(hdr), result) ) return true;
      break;

    case TTN_GETDISPINFO:
      if ( OnGetDispInfo(hdr->hwndFrom, *reinterpret_cast<NMTTDISPINFO*>(hdr)) ) return true;
      break;

    case TTN_POP:
      if ( OnPop(hdr->hwndFrom) ) return true;
      break;

    case TTN_SHOW:
      result = 0;
      if ( OnShow(hdr->hwndFrom, result) ) return true;
      break;
  }
  return CommonNotifyHandler::OnNotify(hdr, result);
}