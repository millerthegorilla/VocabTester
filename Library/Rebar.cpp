#include "Rebar.h"

bool Win::RebarNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case RBN_AUTOSIZE:
      { NMRBAUTOSIZE* info = reinterpret_cast<NMRBAUTOSIZE*>(hdr); 
        if ( OnAutoSize(hdr->hwndFrom, *info) ) return true;
      }
      break;
  }
  return CommonNotifyHandler::OnNotify(hdr, result);
}