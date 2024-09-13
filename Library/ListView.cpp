#include "ListView.h"

using namespace Win;

bool ListViewNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case NM_CLICK:
      {
        NMITEMACTIVATE* data = reinterpret_cast<NMITEMACTIVATE*>(hdr);
        if ( OnClick(hdr->hwndFrom, data->iItem, data->iSubItem, data->ptAction) ) return true;
      }
      break;

    case NM_CUSTOMDRAW:
      if ( OnCustomDraw(hdr->hwndFrom, *reinterpret_cast<NMLVCUSTOMDRAW*>(hdr), result) ) return true;
      break;

    case NM_DBLCLK:
      {
        NMITEMACTIVATE* data = reinterpret_cast<NMITEMACTIVATE*>(hdr);
        if ( OnDoubleClick(hdr->hwndFrom, data->iItem, data->iSubItem, data->ptAction) ) return true;
      }
      break;

    case NM_RCLICK:
      {
        NMITEMACTIVATE* data = reinterpret_cast<NMITEMACTIVATE*>(hdr);
        if ( OnRightClick(hdr->hwndFrom, data->iItem, data->iSubItem, data->ptAction) ) return true;
      }
      break;

    case NM_RDBLCLK:
      {
        NMITEMACTIVATE* data = reinterpret_cast<NMITEMACTIVATE*>(hdr);
        if ( OnRightDoubleClick(hdr->hwndFrom, data->iItem, data->iSubItem, data->ptAction) ) return true;
      }
      break;

    case LVN_BEGINDRAG:
      {
        NMLISTVIEW* data = reinterpret_cast<NMLISTVIEW*>(hdr);
        if ( OnBeginDrag(hdr->hwndFrom, data->iItem) ) return true;
      }
      break;

    case LVN_BEGINLABELEDIT:
      {
        NMLVDISPINFO* data = reinterpret_cast<NMLVDISPINFO*>(hdr);
        result = FALSE;
        if ( OnBeginLabelEdit(hdr->hwndFrom, data->item.iItem, result) ) return true;
      }
      break;

    case LVN_BEGINRDRAG:
      {
        NMLISTVIEW* data = reinterpret_cast<NMLISTVIEW*>(hdr);
        if ( OnBeginRightDrag(hdr->hwndFrom, data->iItem) ) return true;
      }
      break;

    case LVN_COLUMNCLICK:
      {
        NMLISTVIEW* data = reinterpret_cast<NMLISTVIEW*>(hdr);
        if ( OnColumnClick(hdr->hwndFrom, data->iSubItem) ) return true;
      }
      break;

    case LVN_DELETEALLITEMS:
      result = FALSE;
      if ( OnDeleteAllItems(hdr->hwndFrom, result) ) return false;
      break;

    case LVN_DELETEITEM:
      {
        NMLISTVIEW* data = reinterpret_cast<NMLISTVIEW*>(hdr);
        if ( OnDeleteItem(hdr->hwndFrom, data->iItem) ) return true;
      }
      break;

    case LVN_ENDLABELEDIT:
      {
        NMLVDISPINFO* data = reinterpret_cast<NMLVDISPINFO*>(hdr);
        result = TRUE;
        if ( OnEndLabelEdit(hdr->hwndFrom, data->item.iItem, data->item.pszText, result) ) return true;
      }
      break;

    case LVN_GETDISPINFO:
      {
        NMLVDISPINFO* data = reinterpret_cast<NMLVDISPINFO*>(hdr);
        if ( OnGetDispInfo(hdr->hwndFrom, data->item) ) return true;
      }
      break;

    case LVN_GETINFOTIP:
      if ( OnGetInfoTip(hdr->hwndFrom, *reinterpret_cast<NMLVGETINFOTIP*>(hdr)) ) return true;
      break;

    case LVN_HOTTRACK:
      {
        NMLISTVIEW* data = reinterpret_cast<NMLISTVIEW*>(hdr);
        result = 0;
        if ( OnHotTrack(hdr->hwndFrom, data->iItem, data->iSubItem, data->ptAction, result) ) return true;
      }
      break;

    case LVN_INSERTITEM:
      {
        NMLISTVIEW* data = reinterpret_cast<NMLISTVIEW*>(hdr);
        if ( OnInsertItem(hdr->hwndFrom, data->iItem) ) return true;
      }
      break;

    case LVN_ITEMACTIVATE:
      if ( OnItemActivate(hdr->hwndFrom, *reinterpret_cast<NMITEMACTIVATE*>(hdr)) ) return true;
      break;

    case LVN_ITEMCHANGED:
      if ( OnItemChanged(hdr->hwndFrom, *reinterpret_cast<NMLISTVIEW*>(hdr)) ) return true;
      break;

    case LVN_ITEMCHANGING:
      result = FALSE;
      if ( OnItemChanging(hdr->hwndFrom, *reinterpret_cast<NMLISTVIEW*>(hdr), result) ) return true;
      break;

    case LVN_KEYDOWN:
      {
        NMLVKEYDOWN* data = reinterpret_cast<NMLVKEYDOWN*>(hdr);
        if ( OnKeyDown(hdr->hwndFrom, data->wVKey, KeyData(data->flags), result) ) return true;
      }
      break;

    case LVN_MARQUEEBEGIN:
      result = 0;
      if ( OnMarqueeBegin(hdr->hwndFrom, result) ) return true;
      break;

    case LVN_ODCACHEHINT:
      {
        NMLVCACHEHINT* data = reinterpret_cast<NMLVCACHEHINT*>(hdr);
        result = 0;
        if ( OnOdCacheHint(hdr->hwndFrom, data->iFrom, data->iTo) ) return true;
      }
      break;

    case LVN_ODFINDITEM:
      {
        NMLVFINDITEM* data = reinterpret_cast<NMLVFINDITEM*>(hdr);
        result = -1;
        if ( OnOdFindItem(hdr->hwndFrom, data->iStart, data->lvfi, result) ) return true;
      }
      break;

    case LVN_ODSTATECHANGED:
      {
        NMLVODSTATECHANGE* data = reinterpret_cast<NMLVODSTATECHANGE*>(hdr);
        result = 0;
        if ( OnOdStateChanged(hdr->hwndFrom, data->iFrom, data->iTo, data->uOldState, data->uNewState) ) return true;
      }
      break;

    case LVN_SETDISPINFO:
      {
        NMLVDISPINFO* data = reinterpret_cast<NMLVDISPINFO*>(hdr);
        if ( OnSetDispInfo(hdr->hwndFrom, data->item) ) return true;
      }
      break;
  }
  return CommonNotifyHandler::OnNotify(hdr, result);
}