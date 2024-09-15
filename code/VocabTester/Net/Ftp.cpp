// (c) Reliable Software 2003
#include <WinLibBase.h>
#include "Ftp.h"

namespace Ftp
{
	bool Handle::GetFile (	char const * remotePath,
							char const * localPath, 
							File::Attributes attr,
							Internet::Callback * callback)
	{
		bool success = ::FtpGetFile (ToNative (), 
							remotePath, 
							localPath, 
							FALSE, // fail if exists
							attr,
							FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_HYPERLINK,
							reinterpret_cast<DWORD_PTR> (callback)) != FALSE;
		if (!success && callback != 0 && Win::GetError () == ERROR_IO_PENDING)
		{
			Win::ClearError ();
			success = true;
		}
		return success;
	}

	bool Handle::PutFile (	char const * localPath,
							char const * remotePath,
							Internet::Callback * callback)
	{
		bool success =  ::FtpPutFile (ToNative (), 
							localPath, 
							remotePath, 
							FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_HYPERLINK,
							reinterpret_cast<DWORD_PTR> (callback)) != FALSE;

		if (!success && callback != 0 && Win::GetError () == ERROR_IO_PENDING)
		{
			Win::ClearError ();
			success = true;
		}
		return success;
	}

	Ftp::AutoHandle Session::Connect (Internet::Protocol protocol, 
									Internet::Callback * callback)
	{
		INTERNET_PORT port;
		DWORD service;
		switch (protocol)
		{
		case Internet::FTP:
			port = INTERNET_DEFAULT_FTP_PORT;
			service = INTERNET_SERVICE_FTP;
			break;
		case Internet::HTTP:
			port = INTERNET_DEFAULT_HTTP_PORT;
			service = INTERNET_SERVICE_HTTP;
			break;
		case Internet::GOPHER:
			port = INTERNET_DEFAULT_GOPHER_PORT;
			service = INTERNET_SERVICE_GOPHER;
			break;
		};

		Ftp::AutoHandle h (::InternetConnect (  _internetHandle.ToNative (),
												_server.c_str (),
												port,
												_user.c_str (),
												_passwd.c_str (),
												service,
												0,
												reinterpret_cast<DWORD_PTR> (callback)));
		if (h.IsNull () && !(callback != 0 && Win::GetError () == ERROR_IO_PENDING))
		{
			throw Internet::Exception ("Cannot connect to FTP server", _server.c_str ());
		}
		return h;
	}

	// Couldn't figure out how to work it asynchronously
	FileSeq::FileSeq (Ftp::Handle hFtp, char const * pattern, Internet::Callback * callback)
		:_atEnd (false),
		 _callback (callback),
		 _handle (::FtpFindFirstFile (hFtp.ToNative (), pattern, &_data, 0, reinterpret_cast<DWORD_PTR> (callback)))
	{
		Assert (callback == 0); // not tested
		if (_handle == INVALID_HANDLE_VALUE || _handle.IsNull ())
		{
			int err = ::GetLastError ();
			if (_callback != 0 && err == ERROR_IO_PENDING)
				return;
			if (err == ERROR_FILE_NOT_FOUND)
			{
				_atEnd = true;
				Win::ClearError ();
			}
			else if (err == ERROR_INTERNET_EXTENDED_ERROR)
			{
				// Revisit: use InternetGetLastResponseInfo
				throw Win::Exception ("FTP: Cannot list files");
			}
			else
			{
				throw Internet::Exception ("Cannot list files.", pattern);
			}
		}
		SkipPseudoDirs ();
	}

	void FileSeq::SkipPseudoDirs ()
	{
		// skip "." and ".." directories
		while (!AtEnd () && IsFolder () && IsDots ())
			Advance ();
	}

	void FileSeq::Advance ()
	{
		_atEnd = !::InternetFindNextFile (_handle.ToNative (), &_data);
		if (_atEnd)
			Win::ClearError ();
	}

	char const * FileSeq::GetName () const
	{
		Assert (!_atEnd);
		return _data.cFileName;
	}

	RemoteFile::RemoteFile (
		Ftp::Handle hFtp,
		char const * path,
		Access access,
		Internet::Callback * callback)
	: _h (::FtpOpenFile (hFtp.ToNative (), 
						 path, 
						 access,
						 FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_HYPERLINK, 
						 reinterpret_cast<DWORD_PTR> (callback)))
	{
		Assert (callback == 0); // not tested
		if (_h.IsNull ())
			throw Internet::Exception ("Cannot open file", path);
		// Note: only one file can be opened in an FTP session
	}
	FileReadable::FileReadable (Ftp::Handle hFtp, char const * path, Internet::Callback * callback)
		: Ftp::RemoteFile (hFtp, path, Ftp::RemoteFile::Read, callback)
	{}
	FileWritable::FileWritable (Ftp::Handle hFtp, char const * path, Internet::Callback * callback)
		: Ftp::RemoteFile (hFtp, path, Ftp::RemoteFile::Write, callback)
	{}
}
