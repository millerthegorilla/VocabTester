#if !defined (POP3_H)
#define POP3_H
// -------------------------------
// (c) Reliable Software 2003 - 05
// -------------------------------

// Based on RFC #1939

#include <Net/Socket.h>
#include <Mail/Mime.h>
#include <GenericIo.h>

class BufferedStream;

namespace Pop3
{
	// There is no way to tell a POP3 single-line response from multi-line response
	// Client protocol must know what response to expect: 
	// either a single-line response or a multi-line response.

	// Single-line response
	class SingleLineResponse
	{
	public:
		SingleLineResponse (std::string const & line);
		SingleLineResponse (Socket & socket);
		std::string const & Get () const { return _line; }
		char const * c_str () const { return _line.c_str (); }
		bool IsError () const { return !_err.empty (); }
		std::string const & GetError () const { return _err; }
	private:
		void Init (std::string const & line);
	private:
		std::string _line;
		std::string _err;
	};

	// Returns lines until end marker found (a period on a line by itself)
	// Ignores byte-stuffing leading periods
	// After every line, the stream is positioned at the beginning of next line
	// (it can be used to continue parsing)
	class MultiLineResponse : public ::LineSeq
	{
	public:
		MultiLineResponse (Socket & socket);

		bool IsError () const { return _firstLine.IsError (); }
		std::string const & GetError () const { return _firstLine.GetError (); }

		// LineSeq interface
		void Advance ();

	private:
		Socket::Stream				_stream;
		LineBreaker					_lineBreaker;
		Pop3::SingleLineResponse	_firstLine;
	};


	class Connection
	{
	public:
		Connection (std::string const & server);
		~Connection ();
		Socket & GetSocket () { return _socket; }
	private:
		Socket	_socket;
	};

	class Session
	{
	public:
		Session (Pop3::Connection & connection, std::string const & user, std::string const & pass);
		Socket & GetSocket () { return _socket; }
	private:
		Socket & _socket;
	};

	class MessageRetriever
	{
		// Notice: Messages on a POP3 server are numbered from 1 to N
	public:
		MessageRetriever (Pop3::Session & session);
		void Advance ()
		{
			Assert (!AtEnd ());
			_cur++;
		}
		bool AtEnd () const { return _cur > _count; }

		LineSeq & RetrieveHeader ();
		LineSeq & RetrieveMessage ();

		void DeleteMsgFromServer ();
	private:
		Socket			  &	_socket;
		unsigned int		_count;
		unsigned int		_cur;

		std::auto_ptr<Pop3::MultiLineResponse> _lines;
	};

	class Input : public GenericInput<'\0'>
	{
	public:
		Input (BufferedStream & stream) : _stream (stream) {}
		char Get ();
	private:
		BufferedStream & _stream;
	};
}

#endif
