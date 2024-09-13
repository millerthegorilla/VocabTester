#ifndef WINNOTIFYHANDLER_H
#define WINNOTIFYHANDLER_H

#include <windows.h>
#include <commctrl.h>
#include "MessageData.h"

namespace Win
{

class NotifyHandler
{
  public:
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result ) { return false; }
    virtual bool OnCompareItem( COMPAREITEMSTRUCT* data, LRESULT& result ) { return false; }
    virtual bool OnCtlColorBtn( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorEdit( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorListBox( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorScrollBar( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnCtlColorStatic( HWND hwnd, HDC hdc, HBRUSH& bk ) { return false; }
    virtual bool OnDeleteItem( DELETEITEMSTRUCT* data ) { return false; }
    virtual bool OnDrawItem( DRAWITEMSTRUCT* data ) { return false; }
    virtual bool OnHScroll( int code, int pos, HWND hwnd ) { return false; }
    virtual bool OnCharToItem( int key, int caretPos, HWND listBox, LRESULT& result ) { return false; }
    virtual bool OnNotifyFormat( HWND from, int cmd, LRESULT& result ) { return false; }
/*
    virtual bool OnParentNotifyCreate( HWND child ) { return false; }
    virtual bool OnParentNotifyDestroy( HWND child ) { return false; }
*/
    virtual bool OnVKeyToItem( int virtKey, int caretPos, HWND listBox, LRESULT& result ) { return false; }
    virtual bool OnVScroll( int code, int pos, HWND hwnd ) { return false; }
};

class CommonNotifyHandler : public NotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnChar( HWND hwnd, NMCHAR* data, LRESULT& result ) { return false; }
    virtual bool OnClick( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnDoubleClick( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnHover( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnKeyDown( HWND hwnd, UINT virtKey, Win::KeyData const& flags, LRESULT& result ) { return false; } 
    virtual bool OnKillFocus( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnNCHitTest( HWND hwnd, NMMOUSE* data, LRESULT& result ) { return false; }
    virtual bool OnOutOfMemory( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnRightClick( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnRightDoubleClick( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnReleasedCapture( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnReturn( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnSetCursor( HWND hwnd, NMMOUSE* data, LRESULT& result ) { return false; }
    virtual bool OnSetFocus( HWND hwnd, LRESULT& result ) { return false; }
/*
    // Windows 2000 +
    virtual bool OnToolTipsCreated( HWND hwnd, NMTOOLTIPSCREATED* data, LRESULT& result ) { return false; }
*/
};

} // namespace Win

#endif // WINNOTIFYHANDLER_H