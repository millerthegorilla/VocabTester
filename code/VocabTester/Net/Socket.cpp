// (c) Reliable Software 2003
#include <WinLibBase.h>
#include "Socket.h"
#include <StringOp.h>

WinSocks::WinSocks ()
{
	WORD version = MAKEWORD(2,0);
	int err = ::WSAStartup (version, &_info);
	if (err != 0)
	{
		switch (err)
		{
		case WSASYSNOTREADY:
			throw Win::Exception ("Windows Sockets: Network subsystem is not ready for network communication");
		case WSAVERNOTSUPPORTED:
			throw Win::Exception ("Version 2.0 of Windows Sockets support is not provided by this particular Windows Sockets implementation");
		case WSAEINPROGRESS: 
			throw Win::Exception ("A blocking Windows Sockets 1.1 operation is in progress");
		case WSAEPROCLIM: 
			throw Win::Exception ("Limit on the number of tasks supported by the Windows Sockets implementation has been reached");
		case WSAEFAULT: 
			Assert (!"The lpWSAData is not a valid pointer");
		}
	}
}

Socket::Socket ()
	:_sock (::socket(AF_INET, SOCK_STREAM, 0))
{
	if (_sock == INVALID_SOCKET)
		throw Win::Exception ("Cannot Open Internet Socket");
}

Socket::~Socket ()
{
	::closesocket (_sock);
}

void Socket::Connect (std::string const & host, short port)
{
	HostName info (host);
	sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = htons (port);
	name.sin_addr = info._addr;
	memset (&(name.sin_zero), 0, 8); 
	if (::connect (_sock, (sockaddr *)&name, sizeof (sockaddr)) != 0)
		throw Win::Exception ("Socket connection failed", host.c_str ());
}

void Socket::SendLine (std::string const & str)
{
	std::string line (str);
	line += "\r\n";
	if (::send (_sock, line.c_str(), line.length (), 0) == SOCKET_ERROR)
	{
		int err = ::WSAGetLastError ();
		::WSASetLastError (0);
		throw Win::Exception ("Socket send failed.", 0, err);
	};
}

void Socket::Send (char const * buf, int len)
{
	if (::send (_sock, buf, len, 0) == SOCKET_ERROR)
	{
		int err = ::WSAGetLastError ();
		::WSASetLastError (0);
		throw Win::Exception ("Socket send failed.", 0, err);
	};
}

void Socket::SendLine ()
{
	char line [] = "\r\n";
	if (::send (_sock, line, std::strlen (line), 0) == SOCKET_ERROR)
	{
		int err = ::WSAGetLastError ();
		::WSASetLastError (0);
		throw Win::Exception ("Socket send failed", 0, err);
	};
}

unsigned Socket::Receive (char buf [], unsigned size)
{
	int len = ::recv (_sock, buf, size, 0);
	if (len == SOCKET_ERROR)
	{
		int err = ::WSAGetLastError ();
		::WSASetLastError (0);
		throw Win::Exception ("Socket receive failed", 0, err);
	}
	return static_cast<unsigned> (len);
}

void Socket::Stream::FillBuf ()
{
	_bufPos = 0;
	_bufEnd = _sock.Receive (GetBuf (), BufferSize ());
}

HostName::HostName (std::string const & host)
{
	hostent *he = gethostbyname (host.c_str());
	if (he == 0)
	{
		throw Win::Exception ("Cannot get host name", host.c_str (), WSAGetLastError ());
	}
	char * address = he->h_addr_list [0];
	_addr = *((in_addr *) address);
}
