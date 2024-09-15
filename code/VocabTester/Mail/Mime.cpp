// --------------------------
// (c) Reliable Software 2005
// --------------------------
#include <WinLibBase.h>
#include "Mime.h"

// Parse something like this
// Content-Type: application/octet-stream;
//	name="Scripts.znc"

void MIME::ContentType::Init (std::string const & headerValue, std::string const & comment)
{
	unsigned curPos = 0;
	// type/subtype delimited by semicolon
	unsigned slashPos = headerValue.find ('/', curPos);
	if (slashPos != std::string::npos)
	{
		_type = headerValue.substr (curPos, slashPos - curPos);
		curPos = slashPos + 1;
		_subtype = headerValue.substr (curPos);
	}
	else
	{
		_type = headerValue.substr (curPos);
	}
	// comment
	if (!comment.empty ())
	{
		_attribute.Init (comment);
		if (IsMultiPart ())
		{
			//multipart/alternative;	boundary="----=_NextPart_000_008E_01C37F64.36A424D0"
			if (_attribute.GetName () == "boundary")
			{
				_boundary = "--";
				_boundary += _attribute.GetValue ();
			}
		}
	}
}
