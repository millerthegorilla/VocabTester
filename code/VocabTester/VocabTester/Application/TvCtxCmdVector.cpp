#include "precompiled.h"
#include "TvCtxCmdVector.h"
#include "TvCtxCommander.h"

namespace Cmd
{
	Cmd::Item<TvCtxCommander> const TvCtxTable [] =
	{	
		{ "Program_About",			&TvCtxCommander::Program_About,			0,	 "About Program" },
		{ "TreeView_InsertFolder",	&TvCtxCommander::TreeView_InsertFolder,	&TvCtxCommander::TreeView_Can_InsertFolder,   "Insert folder" },
		{ "TreeView_InsertList",	&TvCtxCommander::TreeView_InsertList,	&TvCtxCommander::TreeView_Can_InsertList,	 "Insert list" },
		{ "TreeView_InsertWord",	&TvCtxCommander::TreeView_InsertWord,	&TvCtxCommander::TreeView_Can_InsertWord,	 "Insert word" },
		{ "TreeView_InsertTrans",	&TvCtxCommander::TreeView_InsertTrans,	&TvCtxCommander::TreeView_Can_InsertTrans,	 "Insert translation" },
		{ "TreeView_Delete_Item",	&TvCtxCommander::TreeView_Delete_Item,	&TvCtxCommander::TreeView_Can_Delete_Item,	"Delete item" },
		{ "TestItem",				&TvCtxCommander::TestItem,				0,	"Test Item" },
		{ "LoadTree",				&TvCtxCommander::LoadTree,				0,  "Load Tree" },
		{ 0, 0, 0 }
	};
}