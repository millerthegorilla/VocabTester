#if !defined (MESSAGEMAKER_H)
#define MESSAGEMAKER_H
// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------

#include "Sink.h"

class MessagePart;

namespace Pop3
{
	class MessageMaker : public Pop3::Sink
	{
	public:
		MessagePart const * GetTree () const { return _root.get (); }

		// Sink interface
		void OnHeadersStart ();
		void OnHeader (
			std::string const & name,
			std::string const & value,
			std::string const & attribute);
		void OnHeadersEnd ();
		void OnBodyStart ();
		void OnBodyEnd ();

	private:
		std::auto_ptr<MessagePart>	_root;
	};
};

#endif
