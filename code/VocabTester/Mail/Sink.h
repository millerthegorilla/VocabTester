#if !defined (POP3SINK_H)
#define POP3SINK_H
// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------

namespace Pop3
{
	class Sink
	{
	public:
		virtual void OnHeadersStart () = 0;
		virtual void OnHeader (
			std::string const & name,
			std::string const & value,
			std::string const & attribute) = 0;
		virtual void OnHeadersEnd () = 0;
		virtual void OnBodyStart () = 0;
		virtual void OnBodyEnd () = 0;
	};
};

#endif
