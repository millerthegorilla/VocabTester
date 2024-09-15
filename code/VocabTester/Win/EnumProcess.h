#if !defined (ENUMPROCESS_H)
#define ENUMPROCESS_H
//----------------------------------------------------
// (c) Reliable Software 2000 -- 2003
//----------------------------------------------------

namespace Win
{
	class Messenger
	{
	public:
		virtual void DeliverMessage (Win::Dow::Handle win) = 0;
	};

	class EnumProcess
	{
	public:
		EnumProcess (Win::Dow::Handle callerWindow, Messenger & messenger);
		EnumProcess (std::string const & className, Messenger & messenger);

	private:
		void ProcessWindow (Win::Dow::Handle win);
		static BOOL CALLBACK EnumCallback (HWND hwnd, LPARAM lParam);

	private:
		Win::Dow::Handle	_callerWindow;
		std::string			_className;
		Messenger &			_messenger;
	};
}

#endif
