//----------------------------
// (c) Reliable Software, 2004
//----------------------------
#include "precompiled.h"
#include "Menu.h"

namespace Menu
{
	const Item subItems [] =
	{
		{CMD,		"&Exit",			"Program_Exit", 0},
		{CMD,		"&About",			"Program_About", 0},
		{END,		0,					0}
	};

	const Item programItems [] =
	{
		{CMD,		"&Exit",			"Program_Exit", 0},
		{CMD,		"&About",			"Program_About", 0},
		{SEPARATOR,	0,					0, 0},
		{POP,		"&Submenu",			0,  subItems},
		{END,		0,					0, 0}
	};
	//
	// Menu Bar Items
	//

	const Item barItems [] =
	{
		{POP,   "&Program",		"Program",	programItems},
		{END,	0,				0,			0}
	};
};