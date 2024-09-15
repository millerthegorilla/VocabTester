#if !defined (POP3TREE_H)
#define POP3TREE_H
// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------
#include <File/Path.h>
#include <auto_vector.h>

class Socket;

class MessagePart
{
public:
	virtual ~MessagePart () {}

	std::string GetHeader (std::string const & name) const;

	virtual bool IsMultiPart () const { return false; }
	virtual void AgreeOnBoundary (std::string & boundary) const {}
	virtual void Send (Socket & socket) const = 0;
};

class MultipartMixedPart : public MessagePart
{
public:
	void AddPart (std::auto_ptr<MessagePart> part)
	{
		_parts.push_back (part);
	}
	bool IsMultiPart () const { return true; }
	void Send (Socket & socket) const;
private:
	auto_vector<MessagePart>	_parts;
};

class PlainTextPart : public MessagePart
{
public:
	PlainTextPart (std::string const & text)
		: _text (text)
	{}
	void AgreeOnBoundary (std::string & boundary) const;
	void Send (Socket & socket) const;
private:
	std::string const _text;
};

class ApplicationOctetStreamPart : public MessagePart
{
public:
	ApplicationOctetStreamPart (FilePath const & attPath);
	void Send (Socket & socket) const;
private:
	FilePath const	_attPath;
	std::string		_filename;
};

#endif
