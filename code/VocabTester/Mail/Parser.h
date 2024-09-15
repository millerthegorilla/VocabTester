#if !defined (POP3PARSER_H)
#define POP3PARSER_H
// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------

#include <Mail/Mime.h>
#include <stack>

// Message format based on RFC #2822
// MIME based on RFC #2045

class LineSeq;

namespace Pop3
{
	class Sink;

	class Parser
	{
	public:
		Parser (LineSeq & lineSeq, Pop3::Sink & sink)
			: _lineSeq (lineSeq), 
			  _sink (sink)
		{}
		void Parse ();
	private:
		void Message ();
		void Headers ();
		void Body ();
		void MultiPart ();
		void SimplePart ();
	private:
		LineSeq		  & _lineSeq; 
		Pop3::Sink    & _sink;
		std::stack<MIME::ContentType>	_context;
		MIME::ContentType			  	_currentContext;
	};
};

#endif
