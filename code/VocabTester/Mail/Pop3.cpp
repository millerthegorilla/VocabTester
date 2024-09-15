// -------------------------------
// (c) Reliable Software 2003 - 05
// -------------------------------
#include <WinLibBase.h>
#include "Pop3.h"
#include <Parse/BufferedStream.h>
#include <StringOp.h>

Pop3::SingleLineResponse::SingleLineResponse (std::string const & line)
{
	Init (line);
}

Pop3::SingleLineResponse::SingleLineResponse (Socket & socket)
{
	// Single line responses may be up to 512 characters long,
    // including the terminating CRLF.
	Socket::Stream stream (socket, 128);
	LineBreaker response (stream);
	Init (response.Get ());
}

void Pop3::SingleLineResponse::Init (std::string const & line)
{
	unsigned int const len = line.length ();
	if (len > 0)
	{
		if (line [0] == '+')
		{
			// +OK
			if (len < 3 || line [1] != 'O' || line [2] != 'K')
			{
				_err = "Incorrect response";
				_err += line;
			}
			else
			{
				unsigned idx = line.find_first_not_of (" \t", 3);
				if (idx != std::string::npos)
					_line = line.substr (idx);
			}
		}
		else if (line [0] == '-')
		{
			// -ERR
			if (len < 4 || line [1] != 'E' || line [2] != 'R' || line [3] != 'R')
			{
				_err = "Incorrect response: ";
				_err += line;
			}
			else
			{
				unsigned idx = line.find_first_not_of (" \t", 4);
				if (idx != std::string::npos)
					_err = line.substr (idx);
				else
					_err = "Unspecified error";
			}
		}
		else
		{
			_err = "Unknown response: ";
			_err += line;
		}
	}
	else
		_err = "Empty response";
}

Pop3::MultiLineResponse::MultiLineResponse (Socket & socket) 
	: _stream (socket, 1024), // Revisit: tune buffer size
	  _lineBreaker (_stream),
	  _firstLine (_lineBreaker.Get ())
{
	if (_firstLine.IsError ())
	{
		_done = true;
	}
	else
	{
		_line = _firstLine.Get ();
	}
}

void Pop3::MultiLineResponse::Advance ()
{
	Assert (!AtEnd ());
	_lineBreaker.Advance ();
	std::string const & currLine = _lineBreaker.Get ();

	if (currLine [0] == '.')
	{
		if (currLine.length () == 1)
		{
			// end marker
			_done = true;
		}
		else
		{
			// Eat leading period, it's byte stuffing.
			_line = currLine.substr (1);
		}
	}
	else
		_line = currLine;
}

Pop3::Connection::Connection (std::string const & server)
{
	_socket.Connect (server, 110);
	Pop3::SingleLineResponse response (_socket);
	if (response.IsError ())
		throw Win::Exception ("Pop3: cannot connect to server.", response.GetError ().c_str ());
}

Pop3::Connection::~Connection ()
{
	try
	{
		_socket.SendLine ("quit");
	}
	catch (...)
	{}
}

Pop3::Session::Session (Connection & connection, std::string const & user, std::string const & pass)
: _socket (connection.GetSocket ())
{
	// Log in:
	// username
	std::string userStr ("user ");
	userStr += user;
    _socket.SendLine (userStr);
	Pop3::SingleLineResponse usercmdResponse (_socket);
	if (usercmdResponse.IsError ())
		throw Win::Exception ("Pop3: login error.", usercmdResponse.GetError ().c_str ());
	// password
	std::string passwordStr ("pass ");
	passwordStr += pass;
    _socket.SendLine (passwordStr);
	Pop3::SingleLineResponse passcmdResponse (_socket);
	if (passcmdResponse.IsError ())
		throw Win::Exception ("Pop3: login error.", passcmdResponse.GetError ().c_str ());
}

Pop3::MessageRetriever::MessageRetriever (Pop3::Session & session)
: _socket (session.GetSocket ()),
  _cur (0),
  _count (0)
{
	_socket.SendLine ("stat");
	Pop3::SingleLineResponse response (_socket);
	if (response.IsError ())
		throw Win::Exception ("Pop3: cannot retrieve account statistics.", response.GetError ().c_str ());

	_count = ToInt (response.Get ());

	Advance ();
}

LineSeq & Pop3::MessageRetriever::RetrieveHeader ()
{
	Assert (!AtEnd ());
	std::string topCmd = "top " + ToString (_cur) + " 0";
	_socket.SendLine (topCmd);

	_lines.reset (new Pop3::MultiLineResponse (_socket));
	if (_lines->IsError ())
		throw Win::Exception ("Pop3: cannot retrieve message header from server.", 
		_lines->GetError ().c_str ());

	return *_lines;
}

LineSeq & Pop3::MessageRetriever::RetrieveMessage ()
{
	Assert (!AtEnd ());
	std::string retrCmd = "retr " + ToString (_cur);
	_socket.SendLine (retrCmd);

	_lines.reset (new Pop3::MultiLineResponse (_socket));
	if (_lines->IsError ())
		throw Win::Exception ("Pop3: cannot retrieve message from server.", 
		_lines->GetError ().c_str ());

	return *_lines;
}

void Pop3::MessageRetriever::DeleteMsgFromServer ()
{
	Assert (!AtEnd ());
	std::string deleCmd = "dele " + ToString (_cur);
	_socket.SendLine (deleCmd);
	Pop3::SingleLineResponse response (_socket);
	if (response.IsError ())
		throw Win::Exception ("Pop3: cannot delete message from server.", 
		response.GetError ().c_str ());
}

char Pop3::Input::Get ()
{
	// don't eat the delimiter, it may be the beginning of MIME boundary
	char c = _stream.LookAhead ();
	if (c == '-' || c == '.' || c == ' ')
		return '\0';
	_stream.GetChar (); // accept
	if (c == '\r' && _stream.LookAhead () == '\n')
	{
		_stream.GetChar (); // '\n'
		c = _stream.LookAhead ();
		if (c == '-' || c == '.' || c == ' ')
			return '\0';
		_stream.GetChar (); // accept
	}
	return c;
}
