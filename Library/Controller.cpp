#include "Controller.h"

using namespace Win;

BOOL CALLBACK EnumChildProc( HWND child, LPARAM lParam )
{
  Win::Dow win(child);
  win.SetFont(reinterpret_cast<HFONT>(lParam));
  return TRUE;
}

void Controller::SetFont( Win::Font::Descr const& descr )
{
  _font.Create(descr);
  ::EnumChildWindows(_hwnd, EnumChildProc, reinterpret_cast<LPARAM>(static_cast<HFONT>(_font)));
}