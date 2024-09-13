#ifndef WINCONTROLLER_H
#define WINCONTROLLER_H

#include "Window.h"
#include "MessageData.h"
#include "Tools.h"
#include <dbt.h>
#include <ras.h>

namespace Win
{

class Controller : public Win::Dow
{
  public:
    virtual ~Controller( void ) {}
    virtual bool OnNCCreate( CREATESTRUCT* data, LRESULT& result ) { return false; }
    virtual bool OnActivate( Win::ActivateData const& data ) { return false; }
    virtual bool OnActivateApp( Win::ActivateAppData const& data ) { return false; }
    virtual bool OnAskCBFormatName( Win::AskCBFormatNameData& data ) { return false; }
    virtual bool OnCancelMode( void ) { return false; }
    virtual bool OnCaptureChanged( HWND newCaptureOwner ) { return false; }
    virtual bool OnClose( void ) { return false; }
    virtual bool OnCommand( int cmdId, bool isAccel ) { return false; }
    virtual bool OnCompacting( double cpuUsage ) { return false; }
    virtual bool OnCompareItem( COMPAREITEMSTRUCT* data, LRESULT& result ) { return false; }
    virtual bool OnContextMenu( int x, int y, HWND child ) { return false; }
    virtual bool OnCopyData( HWND sender, COPYDATASTRUCT const* data ) { return false; }
    virtual bool OnCreate( CREATESTRUCT* data, LRESULT& result ) { return false; }
    virtual bool OnCtlColorBtn( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorDlg( HWND dlg, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorEdit( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorListBox( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorMsgBox( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorScrollBar( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorStatic( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnDeadChar( TCHAR charCode, Win::KeyData const& data ) { return false; }
    virtual bool OnDeleteItem( DELETEITEMSTRUCT* data ) { return false; }
    virtual bool OnDestroy( void ) { return false; }
    virtual bool OnDestroyClipboard( void ) { return false; }
    virtual bool OnDeviceChange( UINT event, DEV_BROADCAST_HDR* data, LRESULT& result ) { return false; }
    virtual bool OnDevModeChange( TCHAR const devName[] ) { return false; }
    virtual bool OnDisplayChange( int cx, int cy, int bpp ) { return false; }
    virtual bool OnDrawClipboard( void ) { return false; }
    virtual bool OnDrawItem( DRAWITEMSTRUCT* data ) { return false; }
    virtual bool OnDropFiles( HANDLE hDrop ) { return false; }
    virtual bool OnEnable( bool enable ) { return false; }
    virtual bool OnEndSession( bool endSession, bool logOff ) { return false; }
    virtual bool OnEnterIdle( int source, HWND hwnd ) { return false; }
    virtual bool OnEnterMenuLoop( bool isTrackPopupMenu ) { return false; }
    virtual bool OnEnterSizeMove( void ) { return false; }
    virtual bool OnEraseBkgnd( HDC hdc ) { return false; }
    virtual bool OnExitMenuLoop( bool isTrackPopupMenu ) { return false; }
    virtual bool OnExitSizeMove( void ) { return false; }
    virtual bool OnFontChange( void ) { return false; }
    virtual bool OnGetIcon( int type, HICON& hIcon ) { return false; }
    virtual bool OnGetMinMaxInfo( MINMAXINFO* data ) { return false; }
    virtual bool OnHelp( HELPINFO* data ) { return false; }
    virtual bool OnHotKey( Win::HotKeyData const& data ) { return false; }
    virtual bool OnHScroll( int code, int pos, HWND hwnd ) { return false; }
    virtual bool OnHScrollClipboard( int code, int pos, HWND hwnd ) { return false; }
    virtual bool OnChangeCBChain( HWND beingRemoved, HWND next ) { return false; }
    virtual bool OnChar( TCHAR charCode, Win::KeyData const& data ) { return false; }
    virtual bool OnCharToItem( int key, int caretPos, HWND listBox, LRESULT& result ) { return false; }
    virtual bool OnIconEraseBkgnd( HDC hdc ) { return false; }
    virtual bool OnInitMenu( HMENU hMenu ) { return false; }
    virtual bool OnInitMenuPopup( HMENU hMenu, UINT pos, bool sysMenu ) { return false; }
    virtual bool OnInputLangChange( int charSet, HKL hkl ) { return false; }
    virtual bool OnInputLangChangeRequest( bool sysCharSet, HKL hkl ) { return false; }
    virtual bool OnKeyDown( int virtKey, Win::KeyData const& data ) { return false; }
    virtual bool OnKeyUp( int virtKey, Win::KeyData const& data ) { return false; }
    virtual bool OnKillFocus( HWND receivingFocus ) { return false; }
    virtual bool OnLButtonDblClk( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnLButtonDown( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnLButtonUp( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnMButtonDblClk( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnMButtonDown( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnMButtonUp( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnMeasureItem( MEASUREITEMSTRUCT* data ) { return false; }
    virtual bool OnMenuChar( TCHAR ch, Win::MenuFlags const& flags, HMENU hMenu, LRESULT& result ) { return false; }
    virtual bool OnMenuSelect( UINT menuItem, Win::MenuFlags const& flags, HMENU hMenu ) { return false; }
    virtual bool OnMouseActivate( HWND topWin, Win::HitTestData const& data, UINT mouseMsg, LRESULT& result ) { return false; }
    virtual bool OnMouseMove( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnMouseWheel( int x, int y, int zDelta, Win::KeyState const& state ) { return false; }
    virtual bool OnMove( int x, int y ) { return false; }
    virtual bool OnMoving( RECT* rect, Win::EdgeData const& data ) { return false; }
    virtual bool OnNCActivate( bool active, LRESULT& result ) { return false; }
    virtual bool OnNCCalcSize( NCCALCSIZE_PARAMS* data, LRESULT& result ) { return false; }
    virtual bool OnNCCalcSize( RECT* rect ) { return false; }
    virtual bool OnNCLButtonDblClk( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCLButtonDown( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCLButtonUp( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCMButtonDblClk( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCMButtonDown( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCMButtonUp( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCMouseMove( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCPaint( HRGN hRgn ) { return false; }
    virtual bool OnNCRButtonDblClk( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCRButtonDown( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCRButtonUp( int x, int y, Win::HitTestData const& data ) { return false; }
    virtual bool OnNCHitTest( int x, int y, LRESULT& result ) { return false; }
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result ) { return false; }
    virtual bool OnNotifyFormat( HWND from, int cmd, LRESULT& result ) { return false; }
    virtual bool OnPaint( void ) { return false; }
    virtual bool OnPaintClipboard( HWND hwnd, PAINTSTRUCT* data ) { return false; }
    virtual bool OnPaintIcon( void ) { return false; }
    virtual bool OnPaletteChange( HWND changer ) { return false; }
    virtual bool OnPaletteIsChanging( HWND changer ) { return false; }
/*
    virtual bool OnParentNotifyCreate( HWND child ) { return false; }
    virtual bool OnParentNotifyDestroy( HWND child ) { return false; }
*/
    virtual bool OnPowerBroadcast( DWORD event, DWORD data, LRESULT& result ) { return false; }
    virtual bool OnPrint( HDC hdc, Win::PrintFlags const& flags ) { return false; }
    virtual bool OnPrintClient( HDC hdc, Win::PrintFlags const& flags ) { return false; }
    virtual bool OnQueryDragIcon( HICON& hIcon ) { return false; }
    virtual bool OnQueryEndSession( bool logOff, LRESULT& result ) { return false; }
    virtual bool OnQueryNewPalette( LRESULT& result ) { return false; }
    virtual bool OnQueryOpen( LRESULT& result ) { return false; }
    virtual bool OnQueueSync( void ) { return false; }
    virtual bool OnQuit( int exitCode ) { return false; }
    virtual bool OnRASDialEvent( RASCONNSTATE state, DWORD error ) { return false; }
    virtual bool OnRButtonDblClk( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnRButtonDown( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnRButtonUp( int x, int y, Win::KeyState const& state ) { return false; }
    virtual bool OnRenderAllFormats( void ) { return false; }
    virtual bool OnRenderFormat( UINT formatId ) { return false; }
    virtual bool OnSetCursor( HWND hwnd, Win::HitTestData const& data, UINT mouseMsg, LRESULT& result ) { return false; }
    virtual bool OnSetFocus( HWND losingWin ) { return false; }
    virtual bool OnSetFont( HFONT hFont, bool redraw ) { return false; }
    virtual bool OnSetText( TCHAR const text[], LRESULT& result ) { return false; }
    virtual bool OnSettingChange( int flag, TCHAR const section[] ) { return false; }
    virtual bool OnShowWindow( bool show, int status ) { return false; }
    virtual bool OnSize( int cx, int cy, int sizeType ) { return false; }
    virtual bool OnSizeClipboard( HWND hwnd, RECT* rect ) { return false; }
    virtual bool OnSizing( RECT* rect, Win::EdgeData const& data ) { return false; }
    virtual bool OnSpoolerStatus( int nJobsLeft ) { return false; }
    virtual bool OnStyleChanged( STYLESTRUCT const* data, bool style, bool exStyle ) { return false; }
    virtual bool OnStyleChanging( STYLESTRUCT* data, bool style, bool exStyle ) { return false; }
    virtual bool OnSysColorChange( void ) { return false; }
    virtual bool OnSysCommand( int x, int y, UINT cmdType ) { return false; }
    virtual bool OnSysDeadChar( TCHAR charCOde, Win::KeyData const& data ) { return false; }
    virtual bool OnSysChar( TCHAR charCOde, Win::KeyData const& data ) { return false; }
    virtual bool OnSysKeyDown( int virtKey, Win::KeyData const& data ) { return false; }
    virtual bool OnSysKeyUp( int virtKey, Win::KeyData const& data ) { return false; }
    virtual bool OnTCard( int actionId, DWORD actionData ) { return false; }
    virtual bool OnTimeChange( void ) { return false; }
    virtual bool OnTimer( UINT timerId, TIMERPROC* proc ) { return false; }
    virtual bool OnUserChanged( void ) { return false; }
    virtual bool OnVKeyToItem( int virtKey, int caretPos, HWND listBox, LRESULT& result ) { return false; }
    virtual bool OnVScroll( int code, int pos, HWND hwnd ) { return false; }
    virtual bool OnVScrollClipboard( int code, int pos, HWND hwnd ) { return false; }
    virtual bool OnWindowPosChanged( WINDOWPOS const* data ) { return false; }
    virtual bool OnWindowPosChanging( WINDOWPOS* data ) { return false; }

    virtual bool OnUserMessage( UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result ) { return false; }

  protected: // Methods
    Controller( HWND hwnd )
    : Win::Dow(hwnd)
    {}

    void SetFont( Win::Font::Descr const& decr = Win::Font::Descr(TEXT("MS Sans Serif"), 8) );

  protected: // Data members
    Win::Font::Logical _font;

  friend class Class;
};

} // namespace Win

#endif // WINCONTROLLER_H