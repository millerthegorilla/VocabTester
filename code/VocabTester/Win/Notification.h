#if !defined (NOTIFICATION_H)
#define NOTIFICATION_H
//----------------------------------------------------
// Notify.h
// (c) Reliable Software 2000 -- 2002
//----------------------------------------------------

namespace Win 
{
	class Exception;
	class Controller;
}

// Windows notification handler

namespace Notify
{
	class Handler
	{
		friend class Win::Controller;
	public:
		explicit Handler (unsigned id) : _id (id) {}
		virtual ~Handler () {}
		bool IsHandlerFor (unsigned id) const { return id == _id; }
	protected:
		virtual bool OnNotify (NMHDR * hdr, long & result) throw (Win::Exception) = 0;
		unsigned _id;
	};
}

#endif
