#if !defined (WINSOCKET_H)
#define WINSOCKET_H
// (c) Reliable Software 2003
//#include <WinSock2.h>
#include <Parse/BufferedStream.h>

// Initialize and deinitialize Windows Sockets Subsystem
class WinSocks
{
public:
	WinSocks ();
	~WinSocks ()
	{
		::WSACleanup ();
	}
private:
	WSADATA _info;
};

class Socket
{
public:
	class Stream: public BufferedStream
	{
	public:
		Stream (Socket & sock, unsigned bufSize)
			: BufferedStream (bufSize), _sock (sock)
		{}
	protected:
		void FillBuf ();
	private:
		Socket	   &_sock;
	};

public:
	Socket ();
	~Socket ();
	void Connect (std::string const & host, short port);

	void Send (char const * buf, int len);
	void SendLine ();
	// Appends newline to str
	void SendLine (std::string const & str);

	// Low level method
	unsigned Socket::Receive (char buf [], unsigned size);
private:
	SOCKET _sock;
};

class HostName
{
	friend class Socket;
public:
	HostName (std::string const & host);
private:
	in_addr _addr;
};

#endif
