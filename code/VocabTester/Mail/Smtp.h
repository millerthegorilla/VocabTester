#if !defined (SMTP_H)
#define SMTP_H
// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------
#include <GenericIo.h>
#include <Net/Socket.h>
#include <Mail/Tree.h>

// SMTP: based on RFC #2822

namespace Smtp
{
	class Response
	{
	public:
		enum Status
		{
			ConnectSucceeded = 220,
			Ok               = 250,
			AuthSucceeded    = 235,
			AuthChallenge    = 334,
			DataCmdOk        = 354
		};
	public:
		Response (Socket & socket);
		unsigned int GetStatus () const { return _code; }
		char const * c_str () const { return _comment.c_str (); }
	private:
		unsigned int	_code;
		std::string		_comment;
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
		Session (Connection & connection);
		void Authenticate (std::string const & user, std::string const & password);
	private:
		Connection	& _connection;
	};

	class Output : public GenericOutput
	{
		// SMTP maximum line length: 76 characters
		static const unsigned int LineLength = 76;
		static const unsigned int LineLengthWithCRLF = LineLength + 2; // LineLength + 2: space for CR + LF
	public:
		Output (Socket & socket);
		void Put (char c);
		void Flush ();
	private:
		Socket &			_socket;
		char				_currentLine [LineLengthWithCRLF];
		unsigned int		_currLineLength;
	};

	// ==========
	class Message
	{
	public:
		Message (std::string const & from, 
				 std::string const & to, 
				 std::string const & subject);
		void AddBody (std::auto_ptr<MessagePart> body)
		{
			Assert (_body.get () == 0);
			_body = body;
		}

		void Send (Socket & socket);
	private:
		// Header
		std::string		_from;
		std::string		_to;
		std::string		_subject;

		static const char MimeVersion [];

		// Body
		std::auto_ptr<MessagePart>	_body;
	};
}

#endif
