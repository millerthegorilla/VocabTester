#ifndef WINSPLITTER_H
#define WINSPLITTER_H

#include "Controller.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include <commctrl.h>

namespace Win
{

class VSplitterBar : public Win::Controller
{
  public: // Data members
    static TCHAR const ClassName[];

  public: // Methods
    static void Register( HINSTANCE hInst );

  public: // Enums
    enum // Notify Ids
    {
      BEGINDRAG = NM_FIRST,
      ENDDRAG,
      MOVESPLITTER,
    };

  public: // Class
    class Notify : public NMHDR
    {
      public:
        HWND GetHwnd( void ) const { return hwndFrom; }
        int  GetId( void ) const { return idFrom; }
        int  GetCode( void ) const { return code; }
        
        int  GetX( void ) const { return _x; }

      private:
        Notify( HWND hwnd, int id, int notifyCode, int x = 0 )
        : _x(x)
        { hwndFrom = hwnd;
          idFrom = id;
          code = notifyCode;
        }

      private:
        int _x;

      friend class VSplitterBar;
    };

  public: // Methods
    VSplitterBar( HWND hwnd );

    // Controllers' methods
    virtual bool OnCreate( CREATESTRUCT* data, LRESULT& result );
    virtual bool OnMouseMove( int x, int y, Win::KeyState const& state );
    virtual bool OnLButtonDown( int x, int y, Win::KeyState const& state );
    virtual bool OnLButtonUp( int x, int y, Win::KeyState const& state );
    virtual bool OnCaptureChanged( HWND newCaptureOwner );

    // New methods
    int GetId( void ) const { return _id; }
    int GetX( void ) const { return _prevX; }

  private:
    Win::Dow _parent;
    int      _id;
    int      _prevX;
};

class VSplitterBarNotifyHandler : public Win::NotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnBeginDrag( VSplitterBar* splitter ) { return false; }
    virtual bool OnEndDrag( VSplitterBar* splitter ) { return false; }
    virtual bool OnMoveSplitter( VSplitterBar* splitter, int x ) { return false; }
};

class HSplitterBar : public Win::Controller
{
  public: // Data members
    static TCHAR const ClassName[];

  public: // Methods
    static void Register( HINSTANCE hInst );

  public: // Enums
    enum // Notify Ids
    {
      BEGINDRAG = NM_FIRST,
      ENDDRAG,
      MOVESPLITTER,
    };

  public: // Class
    class Notify : public NMHDR
    {
      public:
        HWND GetHwnd( void ) const { return hwndFrom; }
        int  GetId( void ) const { return idFrom; }
        int  GetCode( void ) const { return code; }
        
        int  GetY( void ) const { return _y; }

      private:
        Notify( HWND hwnd, int id, int notifyCode, int y = 0 )
        : _y(y)
        { hwndFrom = hwnd;
          idFrom = id;
          code = notifyCode;
        }

      private:
        int _y;

      friend class HSplitterBar;
    };

  public: // Class
    class NotifyHandler : public Win::NotifyHandler
    {
      public:
        // Overriden methods
        virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

        // New methods
        virtual bool OnBeginDrag( HSplitterBar* splitter ) { return false; }
        virtual bool OnEndDrag( HSplitterBar* splitter ) { return false; }
        virtual bool OnMoveSplitter( HSplitterBar* splitter, int y ) { return false; }
    };

  public: // Methods
    HSplitterBar( HWND hwnd );

    // Controllers' methods
    virtual bool OnCreate( CREATESTRUCT* data, LRESULT& result );
    virtual bool OnMouseMove( int x, int y, Win::KeyState const& state );
    virtual bool OnLButtonDown( int x, int y, Win::KeyState const& state );
    virtual bool OnLButtonUp( int x, int y, Win::KeyState const& state );
    virtual bool OnCaptureChanged( HWND newCaptureOwner );

    // New methods
    int GetId( void ) const { return _id; }
    int GetY( void ) const { return _prevY; }

  private:
    Win::Dow _parent;
    int      _id;
    int      _prevY;
};

class HSplitterBarNotifyHandler : public Win::NotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnBeginDrag( HSplitterBar* splitter ) { return false; }
    virtual bool OnEndDrag( HSplitterBar* splitter ) { return false; }
    virtual bool OnMoveSplitter( HSplitterBar* splitter, int y ) { return false; }
};


/* Doesn't work :-(
typedef UserControlMaker<Win::VSplitterBar, Win::VSplitterBar::ClassName, Win::VSplitterBar::NotifyHandler> VSplitterBarMaker;
*/
#define VSplitterBarMaker UserControlMaker<Win::VSplitterBar, Win::VSplitterBar::ClassName, Win::VSplitterBarNotifyHandler>
#define HSplitterBarMaker UserControlMaker<Win::HSplitterBar, Win::HSplitterBar::ClassName, Win::HSplitterBarNotifyHandler>

} // namespace Win

#endif // WINSPLITTER_H