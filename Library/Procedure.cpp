#include "Class.h"
#include "Maker.h"
#include "Controller.h"
#include "NotifyHandler.h"
#include "GlobalMem.h"
#include "XWin.h"

using namespace Win;

LRESULT CALLBACK Class::WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  Controller* pCtrl = GetLong<Controller*>(hwnd,0);

  // And here it is - the ugliest construstion computers ever seen
  // and must process
  switch ( msg )
  {
    // First we handle window creation
    case WM_NCCREATE:
      try
      { 
        CREATESTRUCT* data = reinterpret_cast<CREATESTRUCT*>(lParam);
        CtrlFactory* factory = reinterpret_cast<CtrlFactory*>(data->lpCreateParams);
        pCtrl = factory->MakeController(hwnd);
        SetLong<Controller*>(hwnd, pCtrl, 0);
        LRESULT result = TRUE;
        if ( pCtrl->OnNCCreate(data, result) ) return result;
      }
      catch ( Win::Exception& e )
      { ::MessageBox(0, e.Message(), TEXT("Windows exception"), MB_ICONEXCLAMATION|MB_OK);
        return FALSE;
      }
      catch ( ... )
      { ::MessageBox(0, TEXT("Unknown exception has been caught in WndProc while processing WM_NCCREATE"), TEXT("Unknown exception"), MB_ICONEXCLAMATION|MB_OK);
        return FALSE;
      }
      break;

    // And next we proceed in alphabetical order
    case WM_ACTIVATE:
      if ( pCtrl->OnActivate( ActivateData(wParam, lParam) ) ) return 0;
      break;

    case WM_ACTIVATEAPP:
      if ( pCtrl->OnActivateApp( ActivateAppData(wParam, lParam) ) ) return 0;
      break;

    case WM_ASKCBFORMATNAME:
      if ( pCtrl->OnAskCBFormatName( AskCBFormatNameData(wParam, lParam) ) ) return 0;
      break;

    case WM_CANCELMODE:
      if ( pCtrl->OnCancelMode() ) return 0;
      break;

    case WM_CAPTURECHANGED:
      if ( pCtrl->OnCaptureChanged(reinterpret_cast<HWND>(lParam)) ) return 0;
      break;

    case WM_CLOSE:
      if ( pCtrl->OnClose() ) return 0;
      break;

    case WM_COMMAND:
      // If this message is sent by control we convert it to the WM_NOTIFY
      // message
      if ( lParam == 0 )
      { if ( pCtrl->OnCommand(LOWORD(wParam), HIWORD(wParam) == 1) ) return 0;
      }
      else
      { // From a control
        HWND control = reinterpret_cast<HWND>(lParam);
        NMHDR hdr = { control, LOWORD(wParam), HIWORD(wParam) };
        LRESULT result = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(control);
        if ( handler != 0 && handler->OnNotify(&hdr, result) )
        { return result;
        }
        else
        { if ( pCtrl->OnNotify(&hdr, result) ) return result;
        }
      }
      break;

    case WM_COMPACTING:
      if ( pCtrl->OnCompacting(double(wParam) / 0xFFFF) ) return 0;
      break;

    case WM_COMPAREITEM:
      {
        COMPAREITEMSTRUCT* data = reinterpret_cast<COMPAREITEMSTRUCT*>(lParam);
        LRESULT result = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(data->hwndItem);
        if ( handler != 0 && handler->OnCompareItem(data, result) )
        { return result;
        }
        else
        { if ( pCtrl->OnCompareItem(data, result) ) return result;
        }
      }
      break;

    case WM_CONTEXTMENU:
      if ( pCtrl->OnContextMenu(LOWORD(lParam), HIWORD(lParam), reinterpret_cast<HWND>(wParam)) ) return 0;
      break;

    case WM_COPYDATA:
      if ( pCtrl->OnCopyData( reinterpret_cast<HWND>(wParam), reinterpret_cast<COPYDATASTRUCT const*>(lParam) ) ) return TRUE;
      break;

    case WM_CREATE:
      {
        LRESULT result = TRUE;
        if ( pCtrl->OnCreate(reinterpret_cast<CREATESTRUCT*>(lParam), result) ) return result;
      }
      break;

    case WM_CTLCOLORBTN:
      {
        HWND button = reinterpret_cast<HWND>(lParam);
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HBRUSH brush = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(button);
        if ( handler != 0 && handler->OnCtlColorBtn(button, hdc, brush) )
        { return reinterpret_cast<LRESULT>(brush);
        }
        else
        { if ( pCtrl->OnCtlColorBtn(button, hdc, brush) ) return reinterpret_cast<LRESULT>(brush);
        }
      }
      break;

    case WM_CTLCOLORDLG:
      {
        HBRUSH brush = 0;
        if ( pCtrl->OnCtlColorDlg(reinterpret_cast<HWND>(lParam), reinterpret_cast<HDC>(wParam), brush) ) return reinterpret_cast<LRESULT>(brush);
      }
      break;

    case WM_CTLCOLOREDIT:
      {
        HWND edit = reinterpret_cast<HWND>(lParam);
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HBRUSH brush = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(edit);
        if ( handler != 0 && handler->OnCtlColorEdit(edit, hdc, brush) )
        { return reinterpret_cast<LRESULT>(brush);
        }
        else
        { if ( pCtrl->OnCtlColorEdit(edit, hdc, brush) ) return reinterpret_cast<LRESULT>(brush);
        }
      }
      break;

    case WM_CTLCOLORLISTBOX:
      {
        HWND listBox = reinterpret_cast<HWND>(lParam);
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HBRUSH brush = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(listBox);
        if ( handler != 0 && handler->OnCtlColorListBox(listBox, hdc, brush) )
        { return reinterpret_cast<LRESULT>(brush);
        }
        else
        { if ( pCtrl->OnCtlColorListBox(listBox, hdc, brush) ) return reinterpret_cast<LRESULT>(brush);
        }
      }
      break;

    case WM_CTLCOLORMSGBOX:
      {
        HBRUSH brush = 0;
        if ( pCtrl->OnCtlColorMsgBox(reinterpret_cast<HWND>(lParam), reinterpret_cast<HDC>(wParam), brush) ) return reinterpret_cast<LRESULT>(brush);
      }
      break;

    case WM_CTLCOLORSCROLLBAR:
      {
        HWND scrollBar = reinterpret_cast<HWND>(lParam);
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HBRUSH brush = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(scrollBar);
        if ( handler != 0 && handler->OnCtlColorScrollBar(scrollBar, hdc, brush) )
        { return reinterpret_cast<LRESULT>(brush);
        }
        else
        { if ( pCtrl->OnCtlColorScrollBar(scrollBar, hdc, brush) ) return reinterpret_cast<LRESULT>(brush);
        }
      }
      break;

    case WM_CTLCOLORSTATIC:
      {
        HWND stat = reinterpret_cast<HWND>(lParam);
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HBRUSH brush = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(stat);
        if ( handler != 0 && handler->OnCtlColorStatic(stat, hdc, brush) )
        { return reinterpret_cast<LRESULT>(brush);
        }
        else
        { if ( pCtrl->OnCtlColorStatic(stat, hdc, brush) ) return reinterpret_cast<LRESULT>(brush);
        }
      }
      break;

    case WM_DEADCHAR:
      if ( pCtrl->OnDeadChar( static_cast<TCHAR>(wParam), KeyData(lParam) ) ) return 0;
      break;

    case WM_DELETEITEM:
      {
        DELETEITEMSTRUCT* data = reinterpret_cast<DELETEITEMSTRUCT*>(lParam);
        NotifyHandler* handler = GetLong<NotifyHandler*>(data->hwndItem);
        if ( handler != 0 && handler->OnDeleteItem(data) )
        { return TRUE;
        }
        else
        { if ( pCtrl->OnDeleteItem(data) ) return TRUE;
        }
      }
      break;

    case WM_DESTROY:
      if ( pCtrl->OnDestroy() ) return 0;
      break;

    case WM_DESTROYCLIPBOARD:
      if ( pCtrl->OnDestroyClipboard() ) return 0;
      break;

    case WM_DEVICECHANGE:
      {
        LRESULT result = TRUE;
        if ( pCtrl->OnDeviceChange( static_cast<UINT>(wParam), reinterpret_cast<DEV_BROADCAST_HDR*>(lParam), result ) ) return result;
      }
      break;

    case WM_DEVMODECHANGE:
      if ( pCtrl->OnDevModeChange( reinterpret_cast<TCHAR const*>(lParam) ) ) return 0;
      break;

    case WM_DISPLAYCHANGE:
      if ( pCtrl->OnDisplayChange(LOWORD(lParam),HIWORD(lParam),wParam) ) return 0;
      break;

    case WM_DRAWCLIPBOARD:
      if ( pCtrl->OnDrawClipboard() ) return 0;
      break;

    case WM_DRAWITEM:
      {
        DRAWITEMSTRUCT* data = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
        if ( data->CtlType != ODT_MENU )
        { NotifyHandler* handler = GetLong<NotifyHandler*>(data->hwndItem);
          if ( handler != 0 && handler->OnDrawItem(data) )
          { return TRUE;
          }
        }

        if ( pCtrl->OnDrawItem(data) ) return TRUE;
      }
      break;

    case WM_DROPFILES:
      if ( pCtrl->OnDropFiles( reinterpret_cast<HANDLE>(wParam) ) ) return 0;
      break;

    case WM_ENABLE:
      if ( pCtrl->OnEnable(wParam != 0) ) return 0;
      break;

    case WM_ENDSESSION:
      if ( pCtrl->OnEndSession(wParam != 0, lParam == ENDSESSION_LOGOFF) ) return 0;
      break;

    case WM_ENTERIDLE:
      if ( pCtrl->OnEnterIdle( wParam, reinterpret_cast<HWND>(lParam) ) ) return 0;
      break;

    case WM_ENTERMENULOOP:
      if ( pCtrl->OnEnterMenuLoop(wParam != 0) ) return 0;
      break;

    case WM_ENTERSIZEMOVE:
      if ( pCtrl->OnEnterSizeMove() ) return 0;
      break;

    case WM_ERASEBKGND:
      if ( pCtrl->OnEraseBkgnd( reinterpret_cast<HDC>(wParam) ) ) return TRUE;
      break;

    case WM_EXITMENULOOP:
      if ( pCtrl->OnExitMenuLoop(wParam != 0) ) return 0;
      break;

    case WM_EXITSIZEMOVE:
      if ( pCtrl->OnExitSizeMove() ) return 0;
      break;

    case WM_FONTCHANGE:
      if ( pCtrl->OnFontChange() ) return 0;
      break;

    case WM_GETICON:
      {
        HICON hIcon = 0;
        if ( pCtrl->OnGetIcon(wParam, hIcon) ) return reinterpret_cast<LPARAM>(hIcon);
      }
      break;

    case WM_GETMINMAXINFO:
      if ( pCtrl != 0 && pCtrl->OnGetMinMaxInfo( reinterpret_cast<MINMAXINFO*>(lParam) ) ) return 0;
      break;

    case WM_HELP:
      if ( pCtrl->OnHelp( reinterpret_cast<HELPINFO*>(lParam) ) ) return TRUE;
      break;

    case WM_HOTKEY:
      if ( pCtrl->OnHotKey( HotKeyData(wParam, lParam) ) ) return 0;
      break;

    case WM_HSCROLL:
      {
        HWND scroll = reinterpret_cast<HWND>(lParam);
        NotifyHandler* handler = GetLong<NotifyHandler*>(scroll);
        if ( handler != 0 && handler->OnHScroll(LOWORD(wParam), HIWORD(wParam), scroll) )
        { return 0;
        }
        else if ( pCtrl->OnHScroll(LOWORD(wParam), HIWORD(wParam), scroll) )
        { return 0;
        }
      }
      break;

    case WM_HSCROLLCLIPBOARD:
      if ( pCtrl->OnHScrollClipboard(LOWORD(lParam), HIWORD(lParam), reinterpret_cast<HWND>(wParam) ) ) return 0;
      break;

    case WM_CHANGECBCHAIN:
      if ( pCtrl->OnChangeCBChain(reinterpret_cast<HWND>(wParam), reinterpret_cast<HWND>(lParam)) ) return 0;
      break;

    case WM_CHAR:
      if ( pCtrl->OnChar( static_cast<TCHAR>(wParam), KeyData(lParam) ) ) return 0;
      break;

    case WM_CHARTOITEM:
      {
        HWND list = reinterpret_cast<HWND>(lParam);
        LRESULT result = -1;
        NotifyHandler* handler = GetLong<NotifyHandler*>(list);
        if ( handler != 0 && handler->OnCharToItem(LOWORD(wParam), HIWORD(wParam), list, result) )
        { return result;
        }
        else if ( pCtrl->OnCharToItem(LOWORD(wParam), HIWORD(wParam), list, result ) )
        { return result;
        }
      }
      break;

    case WM_ICONERASEBKGND:
      if ( pCtrl->OnIconEraseBkgnd(reinterpret_cast<HDC>(wParam)) ) return TRUE;
      break;

    case WM_INITMENU:
      if ( pCtrl->OnInitMenu(reinterpret_cast<HMENU>(wParam)) ) return 0;
      break;

    case WM_INITMENUPOPUP:
      if ( pCtrl->OnInitMenuPopup(reinterpret_cast<HMENU>(wParam), LOWORD(lParam), HIWORD(lParam) != 0) ) return 0;
      break;

    case WM_INPUTLANGCHANGE:
      pCtrl->OnInputLangChange(wParam, reinterpret_cast<HKL>(lParam));
      break;

    case WM_INPUTLANGCHANGEREQUEST:
      if ( pCtrl->OnInputLangChangeRequest(wParam != 0, reinterpret_cast<HKL>(lParam)) ) return 0;
      break;

    case WM_KEYDOWN:
      if ( pCtrl->OnKeyDown(static_cast<int>(wParam), KeyData(lParam)) ) return 0;
      break;

    case WM_KEYUP:
      if ( pCtrl->OnKeyUp(static_cast<int>(wParam), KeyData(lParam)) ) return 0;
      break;

    case WM_KILLFOCUS:
      if ( pCtrl->OnKillFocus(reinterpret_cast<HWND>(wParam)) ) return 0;
      break;

    case WM_LBUTTONDBLCLK:
      if ( pCtrl->OnLButtonDblClk(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_LBUTTONDOWN:
      if ( pCtrl->OnLButtonDown(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_LBUTTONUP:
      if ( pCtrl->OnLButtonUp(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_MBUTTONDBLCLK:
      if ( pCtrl->OnMButtonDblClk(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_MBUTTONDOWN:
      if ( pCtrl->OnMButtonDown(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_MBUTTONUP:
      if ( pCtrl->OnMButtonUp(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_MEASUREITEM:
      if ( pCtrl->OnMeasureItem( reinterpret_cast<MEASUREITEMSTRUCT*>(lParam) ) ) return TRUE;
      break;

    case WM_MENUCHAR:
      {
        LRESULT result = 0;
        if ( pCtrl->OnMenuChar(LOWORD(wParam), MenuFlags(HIWORD(wParam)), reinterpret_cast<HMENU>(lParam), result) ) return result;
      }
      break;

    case WM_MENUSELECT:
      if ( pCtrl->OnMenuSelect(LOWORD(wParam), MenuFlags(HIWORD(wParam)), reinterpret_cast<HMENU>(lParam)) ) return 0;
      break;

    case WM_MOUSEACTIVATE:
      {
        LRESULT result = MA_ACTIVATE;
        if ( pCtrl->OnMouseActivate(reinterpret_cast<HWND>(wParam), HitTestData(LOWORD(lParam)), HIWORD(lParam), result) ) return result;
      }
      break;

    case WM_MOUSEMOVE:
      if ( pCtrl->OnMouseMove(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;
/*
    // Only on Windows NT
    case WM_MOUSEWHEEL:
      if ( pCtrl->OnMouseWheel(LOWORD(lParam), HIWORD(lParam), HIWORD(wParam), KeyState(LOWORD(wParam))) ) return 0;
      break;
*/
    case WM_MOVE:
      if ( pCtrl->OnMove(LOWORD(lParam), HIWORD(lParam)) ) return 0;
      break;

    case WM_MOVING:
      if ( pCtrl->OnMoving( reinterpret_cast<RECT*>(lParam), EdgeData(wParam)) ) return TRUE;
      break;

    case WM_NCACTIVATE:
      {
        LRESULT result = FALSE;
        if ( pCtrl->OnNCActivate(wParam != 0, result) ) return result;
      }
      break;

    case WM_NCCALCSIZE:
      if ( wParam == TRUE )
      { LRESULT result = 0;
        if ( pCtrl->OnNCCalcSize(reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam), result) ) return result;
      }
      else
      { if ( pCtrl->OnNCCalcSize(reinterpret_cast<RECT*>(lParam)) ) return 0;
      }
      break;

    case WM_NCDESTROY:
      SetLong<Controller*>(hwnd,0);
      delete pCtrl;
      break;

    case WM_NCLBUTTONDBLCLK:
      if ( pCtrl->OnNCLButtonDblClk(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCLBUTTONDOWN:
      if ( pCtrl->OnNCLButtonDown(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCLBUTTONUP:
      if ( pCtrl->OnNCLButtonUp(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCMBUTTONDBLCLK:
      if ( pCtrl->OnNCMButtonDblClk(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCMBUTTONDOWN:
      if ( pCtrl->OnNCMButtonDown(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCMBUTTONUP:
      if ( pCtrl->OnNCMButtonUp(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCMOUSEMOVE:
      if ( pCtrl->OnNCMouseMove(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCPAINT:
      if ( pCtrl->OnNCPaint(reinterpret_cast<HRGN>(wParam)) ) return 0;
      break;

    case WM_NCRBUTTONDBLCLK:
      if ( pCtrl->OnNCRButtonDblClk(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCRBUTTONDOWN:
      if ( pCtrl->OnNCRButtonDown(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCRBUTTONUP:
      if ( pCtrl->OnNCRButtonUp(LOWORD(lParam), HIWORD(lParam), HitTestData(wParam)) ) return 0;
      break;

    case WM_NCHITTEST:
      {
        LRESULT result = 0;
        if ( pCtrl->OnNCHitTest(LOWORD(lParam), HIWORD(lParam), result) ) return result;
      }
      break;

    case WM_NOTIFY:
      {
        NMHDR* hdr = reinterpret_cast<NMHDR*>(lParam);
        LRESULT result = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(hdr->hwndFrom);
        if ( handler != 0 && handler->OnNotify(hdr, result) )
        { return result;
        }
        else if ( pCtrl->OnNotify(hdr, result) )
        { return result;
        }
      }
      break;

    case WM_NOTIFYFORMAT:
      {
        HWND from = reinterpret_cast<HWND>(wParam);
        LRESULT result = 0;
        NotifyHandler* handler = GetLong<NotifyHandler*>(from);
        if ( handler != 0 && handler->OnNotifyFormat(from, lParam,result) )
        { return result;
        }
        else if ( pCtrl->OnNotifyFormat(from, lParam, result) )
        { return result;
        }
      }
      break;

    case WM_PAINT:
      if ( pCtrl->OnPaint() ) return 0;
      break;

    case WM_PAINTCLIPBOARD:
      if ( pCtrl->OnPaintClipboard(reinterpret_cast<HWND>(wParam), GlobalMemLock<PAINTSTRUCT>(reinterpret_cast<HGLOBAL>(lParam))) ) return 0;
      break;

    case WM_PAINTICON:
      if ( pCtrl->OnPaintIcon() ) return 0;
      break;

    case WM_PALETTECHANGED:
      if ( pCtrl->OnPaletteChange(reinterpret_cast<HWND>(wParam)) ) return 0;
      break;

    case WM_PALETTEISCHANGING:
      if ( pCtrl->OnPaletteIsChanging(reinterpret_cast<HWND>(wParam)) ) return 0;
      break;
/*
    case WM_PARENTNOTIFY:
      if ( LOWORD(wParam) == WM_CREATE || LOWORD(wParam) == WM_DESTROY )
      { HWND child = reinterpret_cast<HWND>(lParam);
        NotifyHandler* handler = GetLong<NotifyHandler*>(child);
        switch ( LOWORD(wParam) )
        {
          case WM_CREATE:
            if ( handler != 0 && handler->OnParentNotifyCreate(child) )
            { return 0;
            }
            else if ( pCtrl->OnParentNotifyCreate(child) )
            { return 0;
            }
            break;

          case WM_DESTROY:
            if ( handler != 0 && handler->OnParentNotifyDestroy(child) )
            { return 0;
            }
            else if ( pCtrl->OnParentNotifyDestroy(child) )
            { return 0;
            }
            break;
        }
      }
      break;
*/
    case WM_POWERBROADCAST:
      {
        LRESULT result = TRUE;
        if ( pCtrl->OnPowerBroadcast(wParam, lParam, result) ) return result;
      }
      break;

    case WM_PRINT:
      if ( pCtrl->OnPrint( reinterpret_cast<HDC>(wParam), PrintFlags(lParam)) ) return 0;
      break;

    case WM_PRINTCLIENT:
      if ( pCtrl->OnPrintClient( reinterpret_cast<HDC>(wParam), PrintFlags(lParam)) ) return 0;
      break;

    case WM_QUERYDRAGICON:
      {
        HICON hIcon = 0;
        if ( pCtrl->OnQueryDragIcon(hIcon) ) return reinterpret_cast<LRESULT>(hIcon);
      }
      break;

    case WM_QUERYENDSESSION:
      {
        LRESULT result = TRUE;
        if ( pCtrl->OnQueryEndSession(lParam == ENDSESSION_LOGOFF, result) ) return result;
      }
      break;

    case WM_QUERYNEWPALETTE:
      {
        LRESULT result = FALSE;
        if ( pCtrl->OnQueryNewPalette(result) ) return result;
      }
      break;

    case WM_QUERYOPEN:
      {
        LRESULT result = TRUE;
        if ( pCtrl->OnQueryOpen(result) ) return result;
      }
      break;

    case WM_QUEUESYNC:
      if ( pCtrl->OnQueueSync() ) return 0;
      break;

    case WM_QUIT:
      if ( pCtrl->OnQuit(wParam) ) return 0;
      break;

    case WM_RASDIALEVENT:
      if ( pCtrl->OnRASDialEvent(static_cast<RASCONNSTATE>(wParam), lParam) ) return TRUE;
      break;

    case WM_RBUTTONDBLCLK:
      if ( pCtrl->OnRButtonDblClk(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_RBUTTONDOWN:
      if ( pCtrl->OnRButtonDown(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_RBUTTONUP:
      if ( pCtrl->OnRButtonUp(LOWORD(lParam), HIWORD(lParam), KeyState(wParam)) ) return 0;
      break;

    case WM_RENDERALLFORMATS:
      if ( pCtrl->OnRenderAllFormats() ) return 0;
      break;

    case WM_RENDERFORMAT:
      if ( pCtrl->OnRenderFormat(wParam) ) return 0;
      break;

    case WM_SETCURSOR:
      {
        LRESULT result = FALSE;
        if ( pCtrl->OnSetCursor(reinterpret_cast<HWND>(wParam), HitTestData(LOWORD(lParam)), HIWORD(lParam), result) ) return result;
      }
      break;

    case WM_SETFOCUS:
      if ( pCtrl->OnSetFocus(reinterpret_cast<HWND>(wParam)) ) return 0;
      break;

    case WM_SETFONT:
      if ( pCtrl->OnSetFont(reinterpret_cast<HFONT>(wParam), lParam != 0) ) return 0;
      break;

    case WM_SETTEXT:
      {
        LRESULT result = 0;
        if ( pCtrl->OnSetText(reinterpret_cast<TCHAR const*>(lParam), result) ) return result;
      }
      break;

    case WM_SETTINGCHANGE:
      if ( pCtrl->OnSettingChange(wParam, reinterpret_cast<TCHAR const*>(lParam)) ) return 0;
      break;

    case WM_SHOWWINDOW:
      if ( pCtrl->OnShowWindow(wParam != 0, lParam) ) return 0;
      break;

    case WM_SIZE:
      if ( pCtrl->OnSize(LOWORD(lParam), HIWORD(lParam), wParam) ) return 0;
      break;

    case WM_SIZECLIPBOARD:
      if ( pCtrl->OnSizeClipboard(reinterpret_cast<HWND>(wParam), GlobalMemLock<RECT>(reinterpret_cast<HGLOBAL>(lParam))) ) return 0;
      break;

    case WM_SIZING:
      if ( pCtrl->OnSizing(reinterpret_cast<RECT*>(lParam), EdgeData(wParam)) ) return TRUE;
      break;

    case WM_SPOOLERSTATUS:
      if ( pCtrl->OnSpoolerStatus(LOWORD(lParam)) ) return 0;
      break;

    case WM_STYLECHANGED:
      if ( pCtrl->OnStyleChanged(reinterpret_cast<STYLESTRUCT const*>(lParam), wParam&GWL_STYLE != 0, wParam&GWL_EXSTYLE != 0) ) return 0;
      break;

    case WM_STYLECHANGING:
      if ( pCtrl->OnStyleChanging(reinterpret_cast<STYLESTRUCT*>(lParam), wParam&GWL_STYLE != 0, wParam&GWL_EXSTYLE != 0) ) return 0;
      break;

    case WM_SYSCOLORCHANGE:
      if ( pCtrl->OnSysColorChange() ) return 0;
      break;

    case WM_SYSCOMMAND:
      if ( pCtrl->OnSysCommand(LOWORD(lParam), HIWORD(lParam), wParam) ) return 0;
      break;

    case WM_SYSDEADCHAR:
      if ( pCtrl->OnSysDeadChar(static_cast<TCHAR>(wParam), KeyData(lParam)) ) return 0;
      break;

    case WM_SYSCHAR:
      if ( pCtrl->OnSysChar(static_cast<TCHAR>(wParam), KeyData(lParam)) ) return 0;
      break;

    case WM_SYSKEYDOWN:
      if ( pCtrl->OnSysKeyDown(static_cast<int>(wParam), KeyData(lParam)) ) return 0;
      break;

    case WM_SYSKEYUP:
      if ( pCtrl->OnSysKeyUp(static_cast<int>(wParam), KeyData(lParam)) ) return 0;
      break;

    case WM_TCARD:
      if ( pCtrl->OnTCard(wParam, lParam) ) return 0;
      break;

    case WM_TIMECHANGE:
      if ( pCtrl->OnTimeChange() ) return 0;
      break;

    case WM_TIMER:
      if ( pCtrl->OnTimer(wParam, reinterpret_cast<TIMERPROC*>(lParam)) ) return 0;
      break;

    case WM_USERCHANGED:
      if ( pCtrl->OnUserChanged() ) return 0;
      break;

    case WM_VKEYTOITEM:
      {
        HWND list = reinterpret_cast<HWND>(lParam);
        LRESULT result = -1;
        NotifyHandler* handler = GetLong<NotifyHandler*>(list);
        if ( handler != 0 && handler->OnVKeyToItem(LOWORD(wParam), HIWORD(wParam), list, result) )
        { return result;
        }
        else if ( pCtrl->OnVKeyToItem(LOWORD(wParam), HIWORD(wParam), list, result) )
        { return result;
        }
      }
      break;

    case WM_VSCROLL:
      {
        HWND scroll = reinterpret_cast<HWND>(lParam);
        NotifyHandler* handler = GetLong<NotifyHandler*>(scroll);
        if ( handler != 0 && handler->OnVScroll(LOWORD(wParam), HIWORD(wParam), scroll) )
        { return 0;
        }
        else if ( pCtrl->OnVScroll(LOWORD(wParam), HIWORD(wParam), scroll) )
        { return 0;
        }
      }
      break;

    case WM_VSCROLLCLIPBOARD:
      if ( pCtrl->OnVScrollClipboard(LOWORD(lParam), HIWORD(lParam), reinterpret_cast<HWND>(wParam) ) ) return 0;
      break;

    case WM_WINDOWPOSCHANGED:
      if ( pCtrl->OnWindowPosChanged(reinterpret_cast<WINDOWPOS const*>(lParam)) ) return 0;
      break;

    case WM_WINDOWPOSCHANGING:
      if ( pCtrl->OnWindowPosChanging(reinterpret_cast<WINDOWPOS*>(lParam)) ) return 0;
      break;

    default:
      if ( msg >= WM_USER )
      { LRESULT result = 0;
        if ( pCtrl->OnUserMessage(msg, wParam, lParam, result) ) return result;
      }
      break;
  }

  return ::DefWindowProc(hwnd, msg, wParam, lParam);
}