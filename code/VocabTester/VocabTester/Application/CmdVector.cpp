//----------------------------
// (c) Reliable Software, 2004
//----------------------------
#include "precompiled.h"
#include "CmdVector.h"
#include "Commander.h"

namespace Cmd
{
	Cmd::Item<Commander> const Table [] =
	{																									
		{ "Program_Exit",	&Commander::Program_Exit,	&Commander::can_Program_Exit,   "Exit program" },
		{ "Program_About",	&Commander::Program_About,	0,   "About" },
		{ 0, 0, 0 }
	};
}