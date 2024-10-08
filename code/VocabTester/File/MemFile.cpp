//--------------------------------
// (c) Reliable Software 1997-2004
//--------------------------------
#include <WinLibBase.h>
#include "MemFile.h"

MappableFile::MappableFile (std::string const & path,
							File::Mode const & mode,
							File::Size size)
	: File (path, mode), _hMap (0)
{
	Map (size, mode.IsReadOnly ());
}

MappableFile::MappableFile (std::string const & path,
							File::Mode const & mode)
	: File (path, mode), _hMap (0)
{
	Map (mode.IsReadOnly ());
}

MappableFile::MappableFile (File & file, bool readOnly)
	: File (file), _hMap (0)
{
	Map (readOnly);
}

void MappableFile::Close () throw ()
{
	Unmap ();
	File::Close ();
}

void MappableFile::Map (File::Size size, bool readOnly)
{
	if (size.IsZero ())
		return;

	_hMap = ::CreateFileMapping ( 
					_hFile,
					0, // security
					readOnly? PAGE_READONLY: PAGE_READWRITE,
					size.High (),
					size.Low (),
					0); // name of the mapping object

	if (_hMap == 0)
	{
		throw Win::Exception ("Create file mapping failed");
	}
}

void MappableFile::Map (bool readOnly)
{
	_hMap = ::CreateFileMapping ( 
					_hFile,
					0, // security
					readOnly? PAGE_READONLY: PAGE_READWRITE,
					0, // map whole file
					0,
					0); // name of the mapping object

	if (_hMap == 0)
	{
		// zero-size files are ok
		File::Size size = GetSize ();
		if (!size.IsZero ())
			throw Win::Exception ("Create file mapping failed");
		Win::ClearError ();
	}
}

void MappableFile::Unmap () throw ()
{
	if(_hMap != 0)
	{
		::CloseHandle(_hMap);
		_hMap = 0;
	}
}

//---------------------------------------------------

MemFile::MemFile (std::string const & path,
				  Mode const & mode,
				  File::Size size)
	: MappableFile (path, mode, size),
	  _buf (0),
	  _bufSize (0),
	  _readOnly (mode.IsReadOnly ())
{
	InitBufSize (size);
	Allocate ();
}

MemFile::MemFile (std::string const & path,
				  Mode const & mode)
	: MappableFile (path, mode),
	  _buf (0),
	  _bufSize (0),
	  _readOnly (mode.IsReadOnly ())
{
	InitBufSize ();
	Allocate ();
}

MemFile::MemFile (File & file, bool readOnly)
	: MappableFile (file, readOnly),
	  _buf (0),
	  _bufSize (0),
	  _readOnly (readOnly)
{
    InitBufSize ();
    Allocate ();
}

MemFile::~MemFile() throw ()
{
	Close ();
}

void MemFile::Close () throw ()
{
	Deallocate ();
	MappableFile::Close ();
}

void MemFile::Flush ()
{
	::FlushViewOfFile (_buf, 0);
}

void MemFile::Deallocate () throw ()
{
	if (_buf != 0)
	{
		::UnmapViewOfFile (_buf);
		_buf = 0;
	}
}

void MemFile::InitBufSize (File::Size size)
{
	if (!size.IsLarge () && size.Low () < MAX_BUFFER)
		_bufSize = size.Low ();
	else
		_bufSize = MAX_BUFFER;
}

void MemFile::InitBufSize ()
{
	File::Size size = GetSize ();
	InitBufSize (size);
}

char * MemFile::ResizeFile (File::Size size)
{
	Assert (!_readOnly);
	Deallocate ();
	Unmap ();
	MappableFile::Resize (size);
	Map (size, _readOnly);
	InitBufSize (size);
	Allocate ();
	return _buf;
}

char * MemFile::Reallocate (unsigned long bufSize,
							File::Offset fileOffset)
{
	Deallocate ();
	_bufSize = bufSize;
	Allocate (fileOffset);
	return _buf;
}

void MemFile::Allocate (File::Offset fileOffset)
{
	if (_bufSize == 0 || _hMap == 0)
		return;

	_buf = reinterpret_cast<char *> (::MapViewOfFile (
					_hMap,
					_readOnly? FILE_MAP_READ: FILE_MAP_WRITE,
					fileOffset.High (),
					fileOffset.Low (),
					_bufSize));

	if (_buf == 0)
	{
		throw Win::Exception ("Map view of file failed");
	}
}


// Set final size and close
void MemFile::Close (File::Size size)
{
	if (_hFile != INVALID_HANDLE_VALUE)
	{
		Deallocate ();
		Unmap ();
		File::Resize (size);
		File::Close ();
	}
}

unsigned long MemFile::GetCheckSum () const
{
	if (_buf == 0)
		return 0;
    unsigned long checkSum = 0;
    for (unsigned long i = 0; i < _bufSize; i++)
        checkSum += _buf [i];
    return checkSum;
}

//---------------

// Acquire open file
MemFileReadOnly::MemFileReadOnly (File & file)
    : MemFile (file, true) // read only
{}

//---------------

void LokFile::New (char const *path)
{
    Close ();
	HANDLE hFile = ::CreateFile (
                            path,
                            GENERIC_READ | GENERIC_WRITE,
                            0, // no sharing
                            0,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_HIDDEN,
                            0);
    if (!InitHandle (hFile))
        throw Win::Exception ("Database error: Lock file creation failed.");
	Map (File::Size (1, 0), false);
	_bufSize = 1;
    Allocate ();
}

bool LokFile::IsLocked (char const *path) throw ()
{
    HANDLE hFile = ::CreateFile (
                            path,
                            GENERIC_READ | GENERIC_WRITE,
                            0, // no sharing
                            0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_HIDDEN,
                            0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD err = ::GetLastError ();
		Win::ClearError ();
		if ( err == ERROR_SHARING_VIOLATION)
			return true;
	}
	else
	{
		::CloseHandle (hFile);
	}
	return false;
}

bool LokFile::Open (char const *path)
{
    Close ();
    HANDLE hFile = ::CreateFile (
                            path,
                            GENERIC_READ | GENERIC_WRITE,
                            0, // no sharing
                            0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_HIDDEN,
                            0);
    if (!InitHandle (hFile))
    {
		unsigned error = ::GetLastError ();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
		{
			Win::ClearError ();
            return false;
		}
        throw Win::Exception ("Database error: Could not open lock file.");
    }
    Map (false);
    InitBufSize ();
    Allocate ();
    return true;
}

void LokFile::Stamp (unsigned long timeStamp)
{
    if (!Rewind ())
        throw Win::Exception ("Database error: Cannot rewind lock file");
    unsigned long nBytes = sizeof (timeStamp);
    if (!SimpleWrite (&timeStamp, nBytes) || nBytes != sizeof (timeStamp))
        throw Win::Exception ("Database error: Cannot write to lock file");

    if (!File::Flush ())
        throw Win::Exception ("Database error: Cannot flush lock file");
    if (!Rewind ())
        throw Win::Exception ("Database error: Cannot rewind lock file");
	::UnmapViewOfFile (_buf);
    _buf = 0;
	::CloseHandle(_hMap);
    _hMap = 0;
    Map (false);
    InitBufSize ();
    Allocate ();
}


