// (c) Reliable Software 2003
#include <WinLibBase.h>
#include "EmailMsg.h"

void HeaderSeq::Advance ()
{
	// Empty line means end of headers
	if (_nextLine.size () == 0)
	{
		_done = true;
	}
	else
	{
		std::string foldedLine (_nextLine);
		_lines.Advance ();
		while (!_lines.AtEnd ())
		{
			_nextLine = _lines.Get ();
			if (_nextLine.size () > 0 && (_nextLine [0] == ' ' || _nextLine [0] == '\t'))
			{
				// folded line
				foldedLine.append (_nextLine);
				_lines.Advance ();
			}
			else
				break;
		}
		unsigned endPos = _namedAttribute.Init (foldedLine);
		if (endPos != std::string::npos)
		{
			unsigned curPos = foldedLine.find_first_not_of (" \t", endPos);
			_comment = foldedLine.substr (curPos);
		}
		else
			_comment.clear ();
	}
}