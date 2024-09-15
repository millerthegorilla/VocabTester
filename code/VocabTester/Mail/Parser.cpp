// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------

#include "WinLibBase.h"
#include "Parser.h"
#include "EmailMsg.h"
#include "Sink.h"

void Pop3::Parser::Parse ()
{
	Message ();
}

void Pop3::Parser::Message ()
{
	Headers ();
	Body ();
}

void Pop3::Parser::Headers ()
{
	_sink.OnHeadersStart ();

	for (HeaderSeq hdr (_lineSeq); !hdr.AtEnd (); hdr.Advance ())
	{
		if (hdr.IsName ("Content-Type"))
		{
			_currentContext.Init (hdr.GetValue (), hdr.GetComment ());
		}
		else if (hdr.IsName ("Content-Transfer-Encoding"))
		{
			_currentContext.SetEncoding (hdr.GetValue ());
		}
		_sink.OnHeader (hdr.GetName (), hdr.GetValue (), hdr.GetComment ());
	}

	_sink.OnHeadersEnd ();
}

void Pop3::Parser::Body ()
{
	_sink.OnBodyStart ();

	if (_currentContext.IsMultiPart ())
	{
		MultiPart ();
	}
	else
	{
		SimplePart ();
	}

	_sink.OnBodyEnd ();
}

// Recursive
void Pop3::Parser::MultiPart ()
{
	std::string const & boundary = _currentContext.GetBoundary ();
	unsigned boundaryLen = boundary.size ();
	while (!_lineSeq.AtEnd ())
	{
		std::string const line = _lineSeq.Get ();
		unsigned lineLen = line.size ();
		if (line.compare (0, boundaryLen, boundary) == 0)
		{
			// Multi-part MIME boundary found
			if (lineLen >= boundaryLen + 2 && line [boundaryLen] == '-' && line [boundaryLen + 1] == '-')
			{
				// Closing boundary
				_lineSeq.Advance ();
				break;
			}
			else
			{
				_lineSeq.Advance ();
				_context.push (_currentContext);
				_currentContext.Clear ();
				// Recurse
				Message ();
				_currentContext = _context.top ();
				_context.pop ();
			}
		}
		else
		{
			// Not a MIME boundary
			_lineSeq.Advance ();
			// Revisit: we cannot just skip this line -- we must be able to serialize the whole message
		}
	}
}

void Pop3::Parser::SimplePart ()
{
	// Revisit: implement
}
