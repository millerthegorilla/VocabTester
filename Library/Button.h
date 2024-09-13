#ifndef WINBUTTON_H
#define WINBUTTON_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"

namespace Win
{

class Button : public Win::Dow
{
  public: // Methods
    explicit Button( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    Button( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    void SetText( TCHAR const text[] )
    { SendMessage(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
    }
};

typedef Button GroupBox;


class CheckBox : public Button
{
  public:
    explicit CheckBox( HWND hwnd = 0 )
    : Button(hwnd)
    {}

    CheckBox( HWND dlg, int id )
    : Button(::GetDlgItem(dlg, id))
    {}
    
    bool IsChecked( void ) const
    { return SendMessage(BM_GETCHECK) == BST_CHECKED;
    }

    void Check( bool check = true )
    { SendMessage(BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED);
    }

    void UnCheck( void ) { Check(false); }
};

class RadioButton : public Button
{
  public:
	  explicit RadioButton( HWND hwnd = 0 )
    : Button(hwnd)
    {}

    RadioButton( HWND dlg, int id )
    : Button(::GetDlgItem(dlg, id))
    {}

    bool IsSelected( void ) const
    { return SendMessage(BM_GETCHECK) == BST_CHECKED;
    }

    void Select( void )
    { SendMessage(BM_SETCHECK, BST_CHECKED);
    }
};

typedef CommonNotifyHandler ButtonNotifyHandler;

class ButtonMaker : public ControlMakerHandler<ButtonNotifyHandler>
{
  public:
    ButtonMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ButtonNotifyHandler>(TEXT("BUTTON"), parent, id, style, exStyle, x, y, w, h)
    { _style |= BS_PUSHBUTTON;
    }

    void MakeDefault( void )
    { 
      _style &= ~BS_PUSHBUTTON; 
      _style |= BS_DEFPUSHBUTTON;
    }
};

class GroupBoxMaker : public ControlMakerHandler<ButtonNotifyHandler>
{
  public:
    GroupBoxMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ButtonNotifyHandler>(TEXT("BUTTON"), parent, id, style, exStyle, x, y, w, h)
    { _style |= BS_GROUPBOX;
    }
};

class CheckBoxMaker : public ControlMakerHandler<ButtonNotifyHandler>
{
  public:
    CheckBoxMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ButtonNotifyHandler>(TEXT("BUTTON"), parent, id, style, exStyle, x, y, w, h)
    { _style |= BS_CHECKBOX;
    }

    void MakeAutoCheckBox( void )
    {
      _style &= ~(BS_CHECKBOX|BS_AUTOCHECKBOX|BS_3STATE|BS_AUTO3STATE);
      _style |= BS_AUTOCHECKBOX; 
    }

    void Make3State( void )
    {
      _style &= ~(BS_CHECKBOX|BS_AUTOCHECKBOX|BS_3STATE|BS_AUTO3STATE); 
      _style |= BS_3STATE;
    }

    void MakeAuto3State( void )
    {
      _style &= ~(BS_CHECKBOX|BS_AUTOCHECKBOX|BS_3STATE|BS_AUTO3STATE);
      _style |= BS_AUTO3STATE;
    }
};

class RadioButtonMaker : public ControlMakerHandler<ButtonNotifyHandler>
{
  public:
    RadioButtonMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ButtonNotifyHandler>(TEXT("BUTTON"), parent, id, style, exStyle, x, y, w, h)
    { _style |= BS_RADIOBUTTON;
    }

    void MakeAutoRadioButton( void )
    {
      _style &= ~BS_RADIOBUTTON;
      _style |= BS_AUTORADIOBUTTON; 
    }
};

} // namespace Win

#endif // WINBUTTON_H
