#if !defined WIN_EXCEPTION_H
#define WIN_EXCEPTION_H
//------------------------------------
// (c) Reliable Software 1997 -- 2004
//------------------------------------

namespace Win
{
	inline void ClearError () { ::SetLastError (0); }
	inline int GetError () { return ::GetLastError (); }
	
	enum Error
	{
		PathNotFound		= ERROR_PATH_NOT_FOUND,
		NotReady			= ERROR_NOT_READY,
		BadNetPath			= ERROR_BAD_NETPATH,
		NetUnreachable		= ERROR_NETWORK_UNREACHABLE
	};
	
	class Exception
	{
	public:
		explicit Exception (char const * msg = 0, char const * objName = 0);
		Exception (char const * msg, char const * objName, DWORD err, HINSTANCE hModule = 0);
		DWORD GetError () const { return _err; }
		char const * GetMessage () const { return _msg; }
		char const * GetObjectName () const { return _objName; }
		bool IsSharingViolation () const { return _err == ERROR_SHARING_VIOLATION; }
		bool IsAccessDenied () const { return _err == ERROR_ACCESS_DENIED; }
		HINSTANCE GetModuleHandle () const { return _hModule; }
	private:
		void InitObjName (char const * objName);
	protected:
		DWORD		_err;
		char const *_msg;
		char		_objName [MAX_PATH + 1];
		HINSTANCE	_hModule;
	};

	class CommDlgException: public Exception
	{
	public:
		CommDlgException (DWORD commonDlgExtendedErr, char const * objName = 0);
	};

	class ExitException : public Exception
	{
	public:
		ExitException (char const * msg = 0, char const * objName = 0)
			: Exception (msg, objName)
		{}
	};

	class InternalException : public Exception
	{
	public:
		InternalException (char const * msg, char const * objName = 0)
			: Exception (msg, objName)
		{
			_err = 0;
		}
	};
}

#endif
