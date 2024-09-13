#include "ToolBar.h"

bool Win::ToolBarNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case TBN_BEGINADJUST:
      if ( OnBeginAdjust(hdr->hwndFrom) ) return true;
      break;

    case TBN_BEGINDRAG:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        if ( OnBeginDrag(hdr->hwndFrom, info->iItem) ) return true;
      }
      break;

    case TBN_CUSTHELP:
      if ( OnCustomHelp(hdr->hwndFrom) ) return true;
      break;

    case TBN_DELETINGBUTTON:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        if ( OnDeletingButton(hdr->hwndFrom, info->iItem) ) return true;
      }
      break;

    case TBN_DRAGOUT:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        if ( OnDragOut(hdr->hwndFrom, info->iItem) ) return true;
      }
      break;

    case TBN_DROPDOWN:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        result = TBDDRET_NODEFAULT;
        if ( OnDropDown(hdr->hwndFrom, info->iItem, result) ) return true;
      }
      break;

    case TBN_ENDADJUST:
      if ( OnEndAdjust(hdr->hwndFrom) ) return true;
      break;

    case TBN_ENDDRAG:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        if ( OnEndDrag(hdr->hwndFrom, info->iItem) ) return true;
      }
      break;

    case TBN_GETBUTTONINFO:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        result = 0;
        if ( OnGetButtonInfo(hdr->hwndFrom, *info, result) ) return true;
      }
      break;

    case TBN_GETDISPINFO:
      { NMTBDISPINFO* info = reinterpret_cast<NMTBDISPINFO*>(hdr);
        if ( OnGetDispInfo(hdr->hwndFrom, *info) ) return true;
      }
      break;

    case TBN_GETINFOTIP:
      { NMTBGETINFOTIP* info = reinterpret_cast<NMTBGETINFOTIP*>(hdr);
        if ( OnGetInfoTip(hdr->hwndFrom, *info) ) return true;
      }
      break;

    case TBN_HOTITEMCHANGE:
      { NMTBHOTITEM* info = reinterpret_cast<NMTBHOTITEM*>(hdr);
        result = 0;
        if ( OnHotButtonChange(hdr->hwndFrom, *info, result) ) return true;
      }
      break;
/*
    case TBN_INITCUSTOMIZE:
      if ( OnInitCustomize(hdr->hwndFrom, result) ) return true;
      break;
*/
    case TBN_QUERYDELETE:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        result = TRUE;
        if ( OnQueryDelete(hdr->hwndFrom, info->iItem, result) ) return true;
      }
      break;

    case TBN_QUERYINSERT:
      { NMTOOLBAR* info = reinterpret_cast<NMTOOLBAR*>(hdr);
        result = TRUE;
        if ( OnQueryInsert(hdr->hwndFrom, info->iItem, result) ) return true;
      }
      break;

    case TBN_RESET:
      if ( OnReset(hdr->hwndFrom) ) return true;
      break;
/*
    case TBN_RESTORE:
      { NMTBRESTORE* info = reinterpret_cast<NMTBRESTORE*>(hdr);
        result = 0;
        if ( OnRestore(hdr->hwndFrom, *info, result ) ) return true;
      }
      break;

    case TBN_SAVE:
      { NMTBSAVE* info = reinterpret_cast<NMTBSAVE*>(hdr);
        if ( OnSave(hdr->hwndFrom, *info) ) return true;
      }
      break;
*/
    case TBN_TOOLBARCHANGE:
      if ( OnToolBarChange(hdr->hwndFrom) ) return true;
      break;
  }
  return CommonNotifyHandler::OnNotify(hdr, result);
}