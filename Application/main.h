#if !defined MAIN_H
#define MAIN_H

#include <windows.h>
#include "..\Library\Class.h"

class MainClass : public Win::SimpleClass
{
  public:
    MainClass( TCHAR const className[], HINSTANCE hInst )
    : SimpleClass(className, hInst, WndProc)
    {}

  private:
    static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

class PaneClass : public Win::SimpleClass
{
  public:
    PaneClass( TCHAR const className[], HINSTANCE hInst )
    : SimpleClass(className, hInst, WndProc)
    {}

  private:
    static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
};
#endif