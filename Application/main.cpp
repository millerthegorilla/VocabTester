//------------------------------------
// main.cpp
// (c) Reliable Software 1996
//------------------------------------

#include "main.h"
#include "..\Resources\resource.h"
#include "..\Library\Splitter.h"
#include "..\Library\Class.h"
#include "..\Library\Maker.h"
#include "..\Library\WinString.h"
#include "..\Library\MsgLoop.h"
#include "control.h"

#include <new.h>

int NewHandler (size_t size);

int WINAPI WinMain
    (HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
    _set_new_handler (&NewHandler);

    bool isError = false;
    try
    {
        // Create top window class


        // Create child pane classes
		PaneClass paneClass ("PaneClass", hInst);
        paneClass.SetSysCursor (IDC_IBEAM);
        //paneClass.SetDblClicks ();
        paneClass.Register ();

		Win::VSplitterBar::Register (hInst);

        // Create top window
		char className[] = "My first Database";
		MainClass winClass (className, hInst);
		winClass.SetIcons (ID_MAIN);
		winClass.Register ();
		Win::TopMaker maker (className, hInst);
		maker.SetSize (400, 400);
		maker.SetPosition (10, 10);
		Win::ControllerFactory<WinController> CFact;

		Win::Dow win (maker.Create (className,CFact));
		win.Display();

		Win::MsgLoop loop;
		return loop.Pump ();
    }
	catch (Win::Exception const & e)
	{
		MessageBox (0, e.Message (), "Error", MB_ICONEXCLAMATION | MB_OK);
        isError = true;
	}
	catch (...)
	{
		MessageBox (0, "Internal error: Unknown exception", "Error", MB_ICONEXCLAMATION | MB_OK);
        isError = true;
	}

    if (isError)
        return 0;

    return -1;
}            

int NewHandler (size_t size)
{
    throw "Internal error: Out of memory";
    return 0;
}


