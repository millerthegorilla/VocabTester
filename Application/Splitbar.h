#if !defined (SPLITBAR_H)
#define SPLITBAR_H

#include "..\Library\Splitter.h"

class VSplitterBar;

class splitterNotifier : public Win::VSplitterBarNotifyHandler
{
public:
    bool OnBeginDrag( VSplitterBar* splitter );
    bool OnEndDrag( VSplitterBar* splitter );
    bool OnMoveSplitter( VSplitterBar* splitter, int x );
};

#endif