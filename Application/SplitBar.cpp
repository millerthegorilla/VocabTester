#include "SplitBar.h"

bool splitterNotifier::OnBeginDrag(VSplitterBar* splitter)
{
	return false;
}

bool splitterNotifier::OnEndDrag(VSplitterBar *splitter)
{
	return false;
}

bool splitterNotifier::OnMoveSplitter(VSplitterBar *splitter, int x)
{
	return false;
}