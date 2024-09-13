#include "Splitter.h"
#include "Class.h"

using namespace Win;

TCHAR const VSplitterBar::ClassName[] = TEXT("VSplitterBarClass");
TCHAR const HSplitterBar::ClassName[] = TEXT("HSplitterBarClass");

void VSplitterBar::Register( HINSTANCE hInst )
{
  Class cls(VSplitterBar::ClassName, hInst);
  cls.SetSysCursor(IDC_SIZEWE);
  cls.SetSysBkColor(COLOR_3DFACE);
  cls.Register();
}

void HSplitterBar::Register( HINSTANCE hInst )
{
  Class cls(HSplitterBar::ClassName, hInst);
  cls.SetSysCursor(IDC_SIZENS);
  cls.SetSysBkColor(COLOR_3DFACE);
  cls.Register();
}


VSplitterBar::VSplitterBar( HWND hwnd )
: Controller(hwnd),
  _id(0),
  _prevX(0)
{
}

bool VSplitterBar::OnCreate( CREATESTRUCT* data, LRESULT& result )
{ 
  _parent.Init(data->hwndParent);
  _id = reinterpret_cast<int>(data->hMenu);
  return true;
}

bool VSplitterBar::OnMouseMove( int x, int y, Win::KeyState const& state )
{
  if ( !HasCapture() ) return true;
  
  POINT pt;
	::GetCursorPos(&pt);
  _parent.ScreenToClient(pt);
  if ( _prevX == pt.x ) return true;
	
  _prevX = pt.x;
  VSplitterBar::Notify notify(_hwnd, _id, VSplitterBar::MOVESPLITTER, _prevX);
  _parent.SendMessage(WM_NOTIFY, _id, reinterpret_cast<LPARAM>(&notify));
  return true;
}

bool VSplitterBar::OnLButtonDown( int x, int y, Win::KeyState const& state )
{
  CaptureMouse();
  VSplitterBar::Notify notify(_hwnd, _id, VSplitterBar::BEGINDRAG);
  _parent.SendMessage(WM_NOTIFY, _id, reinterpret_cast<LPARAM>(&notify));
  return true;
}

bool VSplitterBar::OnLButtonUp( int x, int y, Win::KeyState const& state )
{ 
  ReleaseMouse();
  return true;
}

bool VSplitterBar::OnCaptureChanged( HWND newCaptureOwner )
{
  VSplitterBar::Notify notify(_hwnd, _id, VSplitterBar::ENDDRAG);
  _parent.SendMessage(WM_NOTIFY, _id, reinterpret_cast<LPARAM>(&notify));
  return true;
}

bool VSplitterBarNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  VSplitterBar::Notify* notify = reinterpret_cast<VSplitterBar::Notify*>(hdr);
  VSplitterBar* ctrl = GetLong<VSplitterBar*>(notify->GetHwnd(), 0);
  switch ( notify->GetCode() )
  {
    case VSplitterBar::BEGINDRAG:
      if ( OnBeginDrag(ctrl) ) return true;
      break;

    case VSplitterBar::ENDDRAG:
      if ( OnEndDrag(ctrl) ) return true;
      break;

    case VSplitterBar::MOVESPLITTER:
      if ( OnMoveSplitter(ctrl, notify->GetX()) ) return true;
      break;
  }
  return false;
}

HSplitterBar::HSplitterBar( HWND hwnd )
: Controller(hwnd),
  _id(0),
  _prevY(0)
{
}

bool HSplitterBar::OnCreate( CREATESTRUCT* data, LRESULT& result )
{ 
  _parent.Init(data->hwndParent);
  _id = reinterpret_cast<int>(data->hMenu);
  return true;
}

bool HSplitterBar::OnMouseMove( int x, int y, Win::KeyState const& state )
{
  if ( !HasCapture() ) return true;
  
  POINT pt;
	::GetCursorPos(&pt);
  _parent.ScreenToClient(pt);
  if ( _prevY == pt.y ) return true;
	
  _prevY = pt.y;
  HSplitterBar::Notify notify(_hwnd, _id, HSplitterBar::MOVESPLITTER, _prevY);
  _parent.SendMessage(WM_NOTIFY, _id, reinterpret_cast<LPARAM>(&notify));
  return true;
}

bool HSplitterBar::OnLButtonDown( int x, int y, Win::KeyState const& state )
{
  CaptureMouse();
  HSplitterBar::Notify notify(_hwnd, _id, HSplitterBar::BEGINDRAG);
  _parent.SendMessage(WM_NOTIFY, _id, reinterpret_cast<LPARAM>(&notify));
  return true;
}

bool HSplitterBar::OnLButtonUp( int x, int y, Win::KeyState const& state )
{ 
  ReleaseMouse();
  return true;
}

bool HSplitterBar::OnCaptureChanged( HWND newCaptureOwner )
{
  HSplitterBar::Notify notify(_hwnd, _id, HSplitterBar::ENDDRAG);
  _parent.SendMessage(WM_NOTIFY, _id, reinterpret_cast<LPARAM>(&notify));
  return true;
}

bool HSplitterBarNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  HSplitterBar::Notify* notify = reinterpret_cast<HSplitterBar::Notify*>(hdr);
  HSplitterBar* ctrl = GetLong<HSplitterBar*>(notify->GetHwnd(), 0);
  switch ( notify->GetCode() )
  {
    case HSplitterBar::BEGINDRAG:
      if ( OnBeginDrag(ctrl) ) return true;
      break;

    case HSplitterBar::ENDDRAG:
      if ( OnEndDrag(ctrl) ) return true;
      break;

    case HSplitterBar::MOVESPLITTER:
      if ( OnMoveSplitter(ctrl, notify->GetY()) ) return true;
      break;
  }
  return false;
}