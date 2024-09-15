// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------
#include <WinLibBase.h>
#include "Smtp.h"
#include <Mail/Base64.h>
#include <StrTime.h>
#include <Sys/SysTime.h>
#include <sstream>

Smtp::Response::Response (Socket & socket)
: _code (-1)
{
	// The general form of a reply is a numeric completion code
	// (indicating failure or success) usually followed by a text string.	
	// In a multiline reply all lines but the last start with a status code followed by a hyphen,
	// the last line starts with a status code followed by a space.
	Socket::Stream stream (socket, 256);
	LineBreaker lines (stream);
	for (;;)
	{
		std::string const & line = lines.Get ();
		std::istringstream istr (lines.Get ());
		int newCode = -1;
		istr >> std::dec >> newCode;
		if (_code != -1)
		{
			if (newCode != _code)
				throw Win::Exception ("Smtp: Invalid server response.");
		}
		_code = newCode;
		char sep = istr.get ();
		if (sep != std::string::npos)
		{
			std::string cmt;
			std::getline (istr, cmt);
			_comment.append (cmt);
		}

		if (sep == ' ')
			break;

		_comment.append ("\r\n");
		lines.Advance ();
	}
}

Smtp::Connection::Connection (std::string const & server)
{
	_socket.Connect (server, 25);
	Smtp::Response connectResponse (_socket);
	if (connectResponse.GetStatus () != Smtp::Response::ConnectSucceeded)
		throw Win::Exception ("SMTP error: connection failed.", connectResponse.c_str ());
}

Smtp::Connection::~Connection ()
{
	_socket.SendLine ("QUIT");
}

Smtp::Session::Session (Connection & connection)
: _connection (connection)
{
	Socket & socket = _connection.GetSocket ();
	std::string ehloStr = "EHLO";
	socket.SendLine (ehloStr.c_str ());
	Smtp::Response ehloResponse (socket);
	if (ehloResponse.GetStatus () != Smtp::Response::Ok)
	{
		std::string heloStr = "HELO";
		socket.SendLine (heloStr.c_str ());
		Smtp::Response heloResponse (socket);
		if (heloResponse.GetStatus () != Smtp::Response::Ok)
			throw Win::Exception ("SMTP error: cannot initiate session.", heloResponse.c_str ());
	}
}

void Smtp::Session::Authenticate (std::string const & user, std::string const & password)
{
	Socket & socket = _connection.GetSocket ();
	std::string auth ("AUTH LOGIN ");
	socket.SendLine (auth);
	Smtp::Response authResponse (socket);
	if (authResponse.GetStatus () != Smtp::Response::AuthChallenge)
		throw Win::Exception ("SMTP error: authentication failed on username.", authResponse.c_str ());
	socket.SendLine (Base64::Encode (user));
	Smtp::Response userResponse (socket);
	if (userResponse.GetStatus () != Smtp::Response::AuthChallenge)
		throw Win::Exception ("SMTP error: authentication failed on password.", userResponse.c_str ());
	socket.SendLine (Base64::Encode (password));
	Smtp::Response passResponse (socket);
	if (passResponse.GetStatus () != Smtp::Response::AuthSucceeded)
		throw Win::Exception ("SMTP error: authentication failed.", passResponse.c_str ());
}	

namespace Smtp
{
	// SMTP maximum line length: 76 characters
	const unsigned int LineLength = 76;
	const unsigned int NumberOfByteQuartetsPerLine = Smtp::LineLength / 4;
}

Smtp::Output::Output (Socket & socket)
	: _socket (socket),
	  _currLineLength (0)
{
	_currentLine [LineLength] = '\r';
	_currentLine [LineLength + 1] = '\n';
}

void Smtp::Output::Put (char c)
{
	Assert (_currLineLength < LineLength);
	Assert (_currLineLength != -1); // sanity: Flush was not called

	_currentLine [_currLineLength] = c;
	++_currLineLength;

	if (_currLineLength == LineLength)
	{
		_socket.Send (&_currentLine [0], LineLengthWithCRLF);
		_currLineLength = 0;
	}
}

void Smtp::Output::Flush ()
{
	Assert (_currLineLength < LineLength);
	if (_currLineLength > 0)
	{
		_socket.Send (&_currentLine [0], _currLineLength);
		_currLineLength = -1;
	}
}

namespace Smtp
{
	std::string GetDateStr ()
	{
		std::string date;
		StrTime now;
		date.append (now.GetString (), 3);
		date += ',';
		date.append (now.GetString () + 3);
		Win::TimeZone zone;
		int bias = zone.GetBias () / 60;
		if (bias < 0)
		{
			date += " -";
			bias = std::abs (bias);
		}
		else
			date += " +";
		if (bias < 10) // one digit
			date += '0';

		std::ostringstream formattedZone;
		formattedZone.width (3);
		formattedZone.fill ('0');
		formattedZone.setf (std::ios_base::left);
		formattedZone << bias;
		date += formattedZone.str ();
		return date;
	}
}

// auxiliary
std::string ToBracketedString (std::string const & str)
{
    std::ostringstream bracketed;
	bracketed << '<' << str << '>';
	return bracketed.str ();
}

const char Smtp::Message::MimeVersion [] = "MIME-Version: 1.0";

Smtp::Message::Message (
		std::string const & from, 
		std::string const & to, 
		std::string const & subject)
{
	_from    = "From:" + ToBracketedString (from);
	_to      = "To:" + to;
	_subject = "Subject:" + subject;
}

void Smtp::Message::Send (Socket & socket)
{
	// From: required
	socket.SendLine (_from);
	// To: 
	socket.SendLine (_to);
	// Subject:
	socket.SendLine (_subject);
	// Date: required;
	std::string date = "Date:" + Smtp::GetDateStr ();
	socket.SendLine (date);
	socket.SendLine (MimeVersion);

	// Body
	_body->Send (socket);

	// End message
	socket.SendLine (".");
}
