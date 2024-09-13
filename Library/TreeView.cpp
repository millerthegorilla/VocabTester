#include "TreeView.h"

using namespace Win;

bool TreeViewNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case NM_CUSTOMDRAW:
      if ( OnCustomDraw(hdr->hwndFrom, *reinterpret_cast<NMTVCUSTOMDRAW*>(hdr), result) ) return true;
      break;

    case TVN_BEGINDRAG:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        if ( OnBeginDrag(hdr->hwndFrom, data->itemNew, data->ptDrag) ) return true;
      }
      break;

    case TVN_BEGINLABELEDIT:
      {
        NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(hdr);
        result = FALSE;
        if ( OnBeginLabelEdit(hdr->hwndFrom, info->item, result) ) return true;
      }
      break;

    case TVN_BEGINRDRAG:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        if ( OnBeginRightDrag(hdr->hwndFrom, data->itemNew, data->ptDrag) ) return true;
      }
      break;

    case TVN_DELETEITEM:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        if ( OnDeleteItem(hdr->hwndFrom, data->itemOld) ) return true;
      }
      break;

    case TVN_ENDLABELEDIT:
      {
        NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(hdr);
        result = TRUE;
        if ( OnEndLabelEdit(hdr->hwndFrom, info->item, result) ) return true;
      }
      break;

    case TVN_GETDISPINFO:
      {
        NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(hdr);
        if ( OnGetDispInfo(hdr->hwndFrom, info->item) ) return true;
      }
      break;

    case TVN_GETINFOTIP:
      if ( OnGetInfoTip(hdr->hwndFrom, *reinterpret_cast<NMTVGETINFOTIP*>(hdr)) ) return true;
      break;

    case TVN_ITEMEXPANDED:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        if ( data->action&TVE_COLLAPSE )
        { if ( OnItemCollapsed(hdr->hwndFrom, data->itemNew, data->action) ) return true;
        }
        else
        { if ( OnItemExpanded(hdr->hwndFrom, data->itemNew, data->action) ) return true;
        }
      }
      break;

    case TVN_ITEMEXPANDING:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        result = FALSE;
        if ( data->action&TVE_COLLAPSE )
        { if ( OnItemCollapsing(hdr->hwndFrom, data->itemNew, data->action, result) ) return true;
        }
        else
        { if ( OnItemExpanding(hdr->hwndFrom, data->itemNew, data->action, result) ) return true;
        }
      }
      break;

    case TVN_KEYDOWN:
      {
        NMTVKEYDOWN* data = reinterpret_cast<NMTVKEYDOWN*>(hdr);
        result = 0;
        if ( OnKeyDown(hdr->hwndFrom, data->wVKey, Win::KeyData(data->flags), result) ) return true;
      }
      break;

    case TVN_SELCHANGED:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        if ( OnSelChanged(hdr->hwndFrom, data->itemOld, data->itemNew, data->action) ) return true;
      }
      break;

    case TVN_SELCHANGING:
      {
        NMTREEVIEW* data = reinterpret_cast<NMTREEVIEW*>(hdr);
        result = FALSE;
        if ( OnSelChanging(hdr->hwndFrom, data->itemOld, data->itemNew, data->action, result) ) return true;
      }
      break;

    case TVN_SETDISPINFO:
      {
        NMTVDISPINFO* info = reinterpret_cast<NMTVDISPINFO*>(hdr);
        if ( OnSetDispInfo(hdr->hwndFrom, info->item) ) return true;
      }
      break;
/*
    case TVN_SINGLEEXPAND:
      result = TVNRET_DEFAULT;
      if ( OnSingleExpand(hdr->hwndFrom, *reinterpret_cast<NMTREEVIEW*>(hdr), result) ) return true;
      break;
*/
  }
  return CommonNotifyHandler::OnNotify(hdr, result);
}