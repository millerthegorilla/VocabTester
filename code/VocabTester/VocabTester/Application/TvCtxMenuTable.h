#if !defined(TVIEWCTXMENUTABLE_H)
#define TVIEWCTXMENUTABLE_H

#include <Ctrl/Menu.h>

namespace Menu
{
	const Item insertSubItems [] = 
	{
		{CMD,	"Insert Folder",		"TreeView_InsertFolder"	,0},
		{CMD,	"Insert List",			"TreeView_InsertList"	,0},
		{CMD,	"Insert Word",			"TreeView_InsertWord"	,0},
		{CMD,	"Insert Translation",	"TreeView_InsertTrans"	,0},
		{CMD,   "Delete this Item",		"TreeView_Delete_Item"	,0},
		{END,	0,			0}
	};

	const Item contextItem1 [] =
	{
		{CMD,    "About...", "Program_About",0},
		{SEPARATOR, 0,   0,0},
		{CMD,	"Insert Folder",		"TreeView_InsertFolder"	,0},
		{CMD,	"Insert List",			"TreeView_InsertList"	,0},
		{CMD,	"Insert Word",			"TreeView_InsertWord"	,0},
		{CMD,	"Insert Translation",	"TreeView_InsertTrans"	,0},
		{CMD,   "Delete this Item",		"TreeView_Delete_Item"	,0},
		{CMD,   "Test Item",			"TestItem"				,0},
		{CMD,	"Load Tree",			"LoadTree"				,0},
		{END,		0,		0,0}
	};

	/*const Item ContextItem1 = {CMD,    "&About...", "Program_About",0};
	const Item ContextItem2 = {SEPARATOR, 0,   0,0};
	const Item ContextItem3 = {CMD,    "&Insert", 0, insertSubItems};
	const Item ContextItem4 = {END,		0,		0,0};*/
}



#endif