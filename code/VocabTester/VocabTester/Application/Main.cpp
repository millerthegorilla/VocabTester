//-----------------------------
//  (c) Reliable Software, 2004
//-----------------------------
#include "precompiled.h"
#include "TopCtrl.h"
#include "OutSink.h"
#include "WinTestRegistry.h"
#include "../Resource/resource.h"
#include <Win/WinMain.h>
#include <Win/WinClass.h>
#include <Win/WinMaker.h>
#include <Win/MsgLoop.h>
#include <Handlers.h>
#include <Ctrl/Splitter.h>

void UnexpectedHandler ();

Out::Sink TheOutput;
WinTestRegistry TheRegistry ("SolutionBuilder");

int Win::Main (Win::Instance hInst, char const * cmdParam, int cmdShow)
{
	UnexpectedHandlerSwitch unexpHandler (UnexpectedHandler);

    try
    {
        // Create top window class
		Win::Class::TopMaker topWinClass (ID_MAIN, hInst, IDI_MAIN);
        topWinClass.Register ();

		Win::Class::Maker LeftWinClass (ID_LEFTWIN, hInst);
		LeftWinClass.Register ();
		Win::Class::Maker RightWinClass (ID_RIGHTWIN, hInst);
		RightWinClass.Register ();
		Splitter::RegisterClass (hInst);

		// Create Window Controller (needs access to MessagePrepro)
		Win::MessagePrepro msgPrepro;
		TopCtrl ctrl (cmdParam, msgPrepro);

		// Create top window (hidden)
		ResString caption (hInst, ID_CAPTION);
		Win::TopMaker topWin (caption, ID_MAIN, hInst );
		Win::Dow::Handle appWin = topWin.Create (&ctrl);

		// Display top window
		Win::Placement placement (appWin);
		TheRegistry.ReadPlacement (placement);
		placement.CombineShowCmd (cmdShow);
		appWin.SetPlacement (placement);
		placement.CombineShowCmd (cmdShow);
		appWin.SetPlacement (placement);

		// The main message loop
		return msgPrepro.Pump ();
    }
    catch (Win::Exception e)
    {
        TheOutput.Display (e);
    }
    catch (...)
    {
		Win::ClearError ();
        TheOutput.Display ("Internal error: Unknown exception", Out::Error);
    }
	return 0;
}            

void UnexpectedHandler ()
{
	TheOutput.Display ("Unexpected system exception occurred during program execution.\n"
					   "The program will now exit.");
	terminate ();
}
