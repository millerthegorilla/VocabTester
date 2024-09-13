#include "Edit.h"

using namespace Win;

bool EditNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
/*
    // Only Windows 2000+
    case EN_ALIGN_LTR_EC:
      if ( OnAlignLTR( hdr->hwndFrom ) ) return true;
      break;

    case EN_ALIGN_RTL_EC:
      if ( OnAlignRTL( hdr->hwndFrom ) ) return true;
      break;
*/
    case EN_CHANGE:
      if ( OnChange( hdr->hwndFrom ) ) return true;
      break;

    case EN_ERRSPACE:
      if ( OnErrSpace( hdr->hwndFrom ) ) return true;
      break;

    case EN_HSCROLL:
      if ( OnHScroll( hdr->hwndFrom ) ) return true;
      break;

    case EN_KILLFOCUS:
      if ( OnKillFocus( hdr->hwndFrom ) ) return true;
      break;

    case EN_MAXTEXT:
      if ( OnMaxText( hdr->hwndFrom ) ) return true;
      break;

    case EN_SETFOCUS:
      if ( OnSetFocus( hdr->hwndFrom ) ) return true;
      break;

    case EN_UPDATE:
      if ( OnUpdate( hdr->hwndFrom ) ) return true;
      break;

    case EN_VSCROLL:
      if ( OnVScroll( hdr->hwndFrom ) ) return true;
      break;
  }
  return false;
}