#if !defined (CTXCMDVECTOR_H)
#define CTXCMDVECTOR_H

#include <Ctrl/Command.h>

class TvCtxCommander;

namespace Cmd
{
	extern Cmd::Item<TvCtxCommander> const TvCtxTable [];
}

typedef Cmd::VectorExec<TvCtxCommander> TvCtxCmdVector;

#endif
