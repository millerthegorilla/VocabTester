#if !defined (PROCESS_H)
#define PROCESS_H
//---------------------------------
// (c) Reliable Software 2000 -- 02
//---------------------------------

namespace Win
{
	class Message;

	class ProcessProxy
	{
	public:
		ProcessProxy (char const * className, char const * windowName = 0);
		ProcessProxy (unsigned int threadId);
		ProcessProxy (Win::Dow::Handle hwnd)
			: _win (hwnd)
		{}

		void Init (HWND hwnd) { _win.Init (hwnd); }
		void Init (char const * className, char const * windowName = 0);

		Win::Dow::Handle GetWin () const { return _win; }

		bool PostMsg (int msg);
		bool PostMsg (Win::Message & msg);
		void Kill (int timeout = 10000); // timeout 10 seconds

	private:
		static BOOL CALLBACK EnumCallback (HWND hwnd, LPARAM lParam);

	private:
		Win::Dow::Handle _win;
	};

	class ChildProcess
	{
	public:
		ChildProcess (std::string const & cmdLine, bool inheritParentHandles = false);
		~ChildProcess();

		operator HANDLE () { return _processInfo.hProcess; }
		void ShowMinimizedNotActive ();
		void ShowNormal ();
		void SetCurrentFolder (std::string const & curFolder) { _curFolder = curFolder; }
		void SetAppName (std::string const & appName) { _appName = appName; }
		void SetSuspended() { _createSuspended = true; }
		void SetNoFeedbackCursor() { _startupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK; }

		bool Create (unsigned timeoutMilliSec = 1000); // 0 - don't wait at all
		void Resume ();
		bool WaitForDeath (unsigned int timeout = 5000) const;	// Default 5 sec. timeout
		bool IsAlive () const;
		void Terminate ();
		unsigned long GetExitCode () const;
		unsigned int GetThreadId () const { return _processInfo.dwThreadId; }

	private:
		std::string			_cmdLine;
		std::string			_appName;
		std::string			_curFolder;
		bool				_inheritParentHandles;
		bool				_createSuspended;
		PROCESS_INFORMATION	_processInfo;
		STARTUPINFO			_startupInfo; 	   
	};
}

#endif
