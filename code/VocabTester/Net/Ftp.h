#if !defined (FTP_H)
#define FTP_H
// (c) Reliable Software 2003
#include "Internet.h"
#include <Win/Handles.h>
#include <File/File.h>

namespace Ftp
{
	class Handle: public Internet::Handle
	{
	public:
		Handle (HINTERNET h = 0): Internet::Handle (h) {}

		bool GetFile (char const * remotePath,
					  char const * localPath, 
					  File::Attributes attr = File::NormalAttributes (),
					  Internet::Callback * callback = 0);
		bool PutFile (char const * remotePath,
					  char const * localPath,
					  Internet::Callback * callback = 0);
		bool DeleteFile (char const * path)
		{
			return ::FtpDeleteFile (ToNative (), path) != FALSE;
		}
		bool RenameFile (char const * oldPath, char const * newPath)
		{
			return ::FtpRenameFile (ToNative (), oldPath, newPath) != FALSE;
		}
		bool CreateDirectory (char const * path)
		{
			return ::FtpCreateDirectory (ToNative (), path) != FALSE;
		}
		bool RemoveDirectory (char const * path)
		{
			return ::FtpRemoveDirectory (ToNative (), path) != FALSE;
		}
		bool SetCurrentDirectory (char const * path)
		{
			return ::FtpSetCurrentDirectory (ToNative (), path) != FALSE;
		}
		File::Size GetFileSize () const
		{
			unsigned long high, low;
			low = ::FtpGetFileSize (H (), &high);
			return File::Size (low, high);
		}
		bool GetCurrentDirectory (char * buf, unsigned long & len)
		{
			return ::FtpGetCurrentDirectory (ToNative (), buf, &len) != FALSE;
		}
	};

	class AutoHandle: public Win::AutoHandle<Ftp::Handle, Internet::Disposal<Internet::Handle> >
	{
	public:
		AutoHandle (HINTERNET h)
			: Win::AutoHandle<Ftp::Handle, Internet::Disposal<Internet::Handle> > (h)
		{}
		AutoHandle (Internet::AutoHandle h)
			: Win::AutoHandle<Ftp::Handle, Internet::Disposal<Internet::Handle> > (h.release ())
		{}
	};

	class Session
	{
	public:
		Session (Internet::Handle hInternet)
			: _internetHandle (hInternet)
		{}
		void SetServer (std::string const & server)
		{
			_server.assign (server);
		}
		void SetUser (std::string const & user)
		{
			_user.assign (user);
		}
		void SetPassword (std::string const & passwd)
		{
			_passwd.assign (passwd);
		}

		Ftp::AutoHandle Connect (Internet::Protocol protocol = Internet::FTP,
								 Internet::Callback * callback = 0);

	private:
		Internet::Handle _internetHandle;
		std::string		_server;
		std::string		_user;
		std::string		_passwd;
	};

	class FileSeq
	{
	public:
		FileSeq (Ftp::Handle hFtp, char const * pattern, Internet::Callback * callback = 0);
		bool AtEnd () const
		{
			return _atEnd;
		}
		virtual void Advance ();
		char const * GetName () const;
		FileTime GetWriteTime () const { return _data.ftLastWriteTime; }
		unsigned long GetSize () const { return _data.nFileSizeLow; }
		bool IsFolder () const { return (_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0; }
		bool IsReadOnly () const { return (_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0; }
		bool IsDots () const 
		{ 	
			return _data.cFileName [0] == '.' && (_data.cFileName [1] == '\0' // single dot
					|| (_data.cFileName [1] == '.' && _data.cFileName [2] == '\0')); // double dot
		}
	protected:
		void SkipPseudoDirs ();
	protected:
		bool            _atEnd;
		Internet::Callback * _callback;
		Internet::AutoHandle _handle;
		WIN32_FIND_DATA	_data;
	};

	class RemoteFile
	{
	public:
		enum Access
		{
			Read  = GENERIC_READ,
			Write = GENERIC_WRITE
		};
	public:
		RemoteFile (Ftp::Handle hFtp,
			        char const * path,
					Access access,
					Internet::Callback * callback = 0);
		File::Size GetSize () const
		{
			return _h.GetFileSize ();
		}
	protected:
		Ftp::AutoHandle _h;
	};

	class FileReadable : public Ftp::RemoteFile
	{
	public:
		FileReadable (Ftp::Handle hFtp, char const * path, Internet::Callback * callback = 0);
		bool Read (char * buf, unsigned long & size)
		{
			BOOL result = ::InternetReadFile (_h.ToNative (), buf, size, &size);
			return result != FALSE;
		}
	};

	class FileWritable : public Ftp::RemoteFile
	{
	public:
		FileWritable (Ftp::Handle hFtp, char const * path, Internet::Callback * callback = 0);
		bool Write (char * buf, unsigned long & size)
		{
			BOOL result = ::InternetWriteFile (_h.ToNative (), buf, size, &size);
			return result != FALSE;
		}
	};
}

#endif
