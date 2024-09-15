#if !defined (ERROR_H)
#define ERROR_H
// (c) Reliable Software 2003

class LastSysErr
{
public:
    LastSysErr ();
    ~LastSysErr ();
    operator char const * () const  { return _msg; }
    char const * Text () const { return _msg; }
    DWORD Code () const { return _err; }
	bool IsFileNotFound () const { return _err == ERROR_FILE_NOT_FOUND; }
	bool IsPathNotFound () const { return _err == ERROR_PATH_NOT_FOUND; }
private:
    char * _msg;
    DWORD  _err;
};

class SysMsg
{
public:
    SysMsg (DWORD errCode, HINSTANCE hModule = 0);
    ~SysMsg ();
    operator char const * () const  { return _msg; }
    char const * Text () const { return _msg; }
private:
    char * _msg;
};

#endif
