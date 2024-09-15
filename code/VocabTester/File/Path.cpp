//-----------------------------------
// (c) Reliable Software 1998 -- 2004
//-----------------------------------
#include <WinLibBase.h>
#include "Path.h"
#include "File.h"
#include "Dir.h"

#include <Sys/RegKey.h>

#include <LightString.h>
#include <StringOp.h>
#include <shlwapi.h>

char const FilePath::IllegalChars [] = "<>\"\n\r\t*?";

char const FilePath::_allFilesName [] = "*.*";
char const FilePath::_pathSeparators [] = "\\/";
char const FilePath::_emptyPath [] = "";

//-----------PREDICATES---------------------------

// predicate for path character comparison
inline bool PathCharLess (char a, char b)
{
	if (a == b) 
		return false; // common case
	// separator always < non-separator
	if (FilePath::IsSeparator (a))
		return !FilePath::IsSeparator (b);
	if (FilePath::IsSeparator (b))
		return false; // non-separator > separator
	return ::ToLower (a) < ::ToLower (b);
}

//-------------------------------------------------

void FilePath::Clear ()
{
	//	swapping with local variable will cause any memory associated with _buf
	//	to be deallocated immediately
	std::string empty;
	_buf.swap (empty);
	_prefix = 0;
}

void FilePath::Change (char const * newPath)
{
	_buf.assign (newPath);
	if (!_buf.empty ())
	{
		// Remove terminating backslash, if present
		std::string::size_type lastPos = _buf.length () - 1;
		if (FilePath::IsSeparator (_buf [lastPos]))
			_buf.resize (lastPos);
	}
	_prefix = _buf.length ();
}

void FilePath::Change (FilePath const & newPath)
{
	Change (newPath.GetDir ());
}

void FilePath::Change (std::string const & newPath)
{
	Change (newPath.c_str ());
}

void FilePath::DirUp ()
{
	size_t backSlashPos = _buf.find_last_of (_pathSeparators, _prefix - 1);
	if (backSlashPos != std::string::npos)
	{
		_buf.resize (backSlashPos);
		_prefix = _buf.length ();
	}
}

// Append sub folder to the current path; move _prefix
void FilePath::DirDown (char const * subDir)
{
	if (_prefix > 0 && IsSeparator (_buf [_prefix - 1]))
		--_prefix;
	_buf.resize (_prefix);
	Append (subDir);
	_prefix = _buf.length ();
}

char const * FilePath::GetAllFilesPath () const
{
	return GetFilePath (_allFilesName);
}

char const * FilePath::GetDir () const
{
	_buf.resize (_prefix);
	if (_prefix == 2 && _buf [_prefix - 1] == ':')
	{
		// Handle special case "c:"
		_buf.append (1, _backSlash);
	}
	return _buf.c_str ();
}

// Extract relative path at prefix
char const * FilePath::GetRelativePath (unsigned int prefix) const
{
	char const * fullPath = GetDir ();
	if (_buf.length () > prefix)
		return &fullPath [prefix];
	else
		return _emptyPath;
}

// Append sub folder to the current path; don't move _prefix
void FilePath::Append (std::string const & subDir)
{
	Assert (subDir.length () != 0);

	if (_buf.empty ())
	{
		_buf.assign (subDir);
	}
	else
	{
		_buf.append (1, _backSlash);
		_buf.append (subDir);
	}
}

std::string FilePath::GetDrive () const
{
	Assert (IsAbsolute (_buf));
	FullPathSeq pathSeq (_buf.c_str ());
	if (pathSeq.HasDrive ())
		return std::string (pathSeq.GetDrive ());
	else
		return std::string (pathSeq.GetServerShare ());
}

// File full path -- append file name to the current path at _prefix
char const * FilePath::GetFilePath (char const * fileName) const
{
	_buf.resize (_prefix);
	if (!_buf.empty () && !FilePath::IsSeparator (fileName [0]))
		_buf.append (1, _backSlash);
	_buf.append (fileName);
	return _buf.c_str ();
}

char const * FilePath::GetFilePath (std::string const & fileName) const
{
	return GetFilePath (fileName.c_str ());
}

bool FilePath::Canonicalize (bool quiet)
{
	char canonicalPath [MAX_PATH];
	char const * path = GetDir ();
	if (!::PathCanonicalize (canonicalPath, path))
	{
		if (quiet)
			return false;
		else
			throw Win::Exception ("Illegal path -- cannot canonicalize.", path);
	}
	Change (canonicalPath);
	return true;
}

void FilePath::ConvertToLongPath ()
{
	if (FilePath::IsAbsolute (GetDir ()))
	{
		FilePath workPath;
		FullPathSeq pathSeq (GetDir ());
		if (pathSeq.HasDrive ())
		{
			workPath.Change (pathSeq.GetDrive ());
		}
		else
		{
			Assert (pathSeq.IsUNC ());
			workPath.Change (pathSeq.GetServerShare ());
		}
		// For every path segment get its long name
		for (pathSeq.Advance (); !pathSeq.AtEnd (); pathSeq.Advance ())
		{
			WIN32_FIND_DATA findData;
			workPath.DirDown (pathSeq.GetSegment ());
			HANDLE handle = ::FindFirstFile (workPath.GetDir (), &findData);
			if (handle != INVALID_HANDLE_VALUE)
			{
				workPath.DirUp ();
				workPath.DirDown (findData.cFileName);
				::FindClose (handle);
			}
		}
		Change (workPath.GetDir ());
	}
}

void FilePath::ExpandEnvironmentStrings ()
{
	if (_buf.find ('%') == std::string::npos)
		return;	// No environment varaibles in the path

	std::string expandedPath;
	expandedPath.resize (256);
	unsigned int expandedLength = ::ExpandEnvironmentStrings (GetDir (), &expandedPath [0], expandedPath.length ());
	if (expandedLength > expandedPath.length ())
	{
		// try again with longer buffer
		expandedPath.resize (expandedLength);
		expandedLength = ::ExpandEnvironmentStrings (GetDir (), &expandedPath [0], expandedPath.length ());
	}
	Assert (expandedLength <= expandedPath.length ());
	Change (expandedPath);
}

bool FilePath::IsAbsolute (char const * path)
{
	FilePathSeq folderSeq (path);
	if (folderSeq.HasDrive ())
	{
		return (std::strlen (path) == 2 || IsSeparator (path [2]));
	}
	return folderSeq.IsUNC ();
}

bool FilePath::IsNetwork (char const  * path)
{
	FilePathSeq folderSeq (path);
	if (folderSeq.IsUNC ())
	{
		return true;
	}
	else if (folderSeq.HasDrive ())
	{
		// Check if this is network drive
		FullPathSeq fullPathSeq (path);
		char const * drive = fullPathSeq.GetDrive ();
		int driveType = ::GetDriveType (drive);
		return driveType == DRIVE_REMOTE;
	}
	return false;
}

bool FilePath::HasValidDriveLetter (char const * path)
{
	FilePathSeq folderSeq (path);
	if (folderSeq.HasDrive ())
	{
		unsigned int driveBitMask = ::GetLogicalDrives ();
		if (driveBitMask != 0)
		{
			PathSplitter splitter (path);
			std::string drive (splitter.GetDrive ());
			unsigned int index = ToUpper (drive [0]) - 'A';
			unsigned int driveBit = (1 << index);
			return (driveBitMask & driveBit) != 0;
		}
	}
	return true;
}

bool FilePath::IsFileNameOnly (char const  * path)
{
	FilePathSeq pathSeq (path);
	if (pathSeq.HasDrive ())
		return false;

	std::string filePath (path);
	return filePath.find_first_of (FilePath::_pathSeparators) == std::string::npos;
}

bool FilePath::IsEqualDir (char const * path) const
{
	return IsEqualDir (path, GetDir ());
}

bool FilePath::IsEqualDir (std::string dir1, std::string dir2)
{
	if (dir1.length () != dir2.length ())
		return false;
	std::string::const_iterator seq1 = dir1.begin ();
	std::string::const_iterator seq2 = dir2.begin ();
	for (; seq1 != dir1.end (); ++seq1, ++seq2)
	{
		if (::ToLower (*seq1) != ::ToLower (*seq2))
		{
			if (!IsSeparator (*seq1) || !IsSeparator (*seq2))
				return false;
		}
	}
	return true;
}

bool FilePath::IsDirLess (std::string dir1, std::string dir2)
{
	std::string::const_iterator seq1 = dir1.begin ();
	std::string::const_iterator seq2 = dir2.begin ();
	for (; seq1 != dir1.end () && seq2 != dir2.end (); ++seq1, ++seq2)
	{
		if (IsSeparator (*seq1))
		{
			if (IsSeparator (*seq2))
				continue;
			else
				return true; // separator is less than any other char
		}
		if (IsSeparator (*seq2))
			return false;

		char c1 = ::ToLower (*seq1);
		char c2 = ::ToLower (*seq2);
		if (c1 != c2)
			return c1 < c2;
	}
	return seq1 == dir1.end () && seq2 != dir2.end ();
}

bool FilePath::IsDirLess (FilePath const & path) const
{
	char const * b1 = _buf.c_str ();
	int len1 = _prefix;
	char const * b2 = path._buf.c_str ();
	int len2 = path._prefix;
	return std::lexicographical_compare (b1, b1 + len1, b2, b2 + len2, PathCharLess);
}

bool FilePath::HasPrefix (char const * path) const
{
	size_t prefixLen = strlen (path);
	if (prefixLen > _buf.length ())
		return false;
	if (prefixLen == 0)
		return true;
	size_t i;
	for (i = 0; i < prefixLen; i++)
	{
		if (::ToLower (_buf [i]) != ::ToLower (path [i]))
		{
			if (!IsSeparator (_buf [i]) || !IsSeparator (path [i]))
				break;
		}
	}
	if (i < prefixLen)
		return false;
	// Check if match found on complete path parts -- not in the middle of folder name
	if (prefixLen < _buf.length ())
	{
		// "path = c:\foo\"
		// "buf  = c:\foo\bar"
		if (IsSeparator (path [prefixLen - 1]))
			return true;
		if (!IsSeparator (_buf [prefixLen]))
			return false;
	}
	return true;
}

bool FilePath::HasSuffix (char const * path) const
{
	size_t suffixLen = strlen (path);
	if (suffixLen > _buf.length ())
		return false;
	if (suffixLen == 0)
		return true;
	int i;
	for (i = suffixLen - 1; i >= 0; --i)
	{
		int j = _buf.length () - (suffixLen - i);
		if (::ToLower (_buf [j]) != ::ToLower (path [i]))
		{
			if (!IsSeparator (_buf [j]) || !IsSeparator (path [i]))
				break;
		}
	}
	if (i > 0)
		return false;
	// Check if match found on complete path parts -- not in the middle of folder name
	if (suffixLen < _buf.length ())
	{
		// "path = \foo\bar.h"
		// "buf  = c:\foo\bar.h"
		if (IsSeparator (path [0]))
			return true;
		int k = _buf.length () - suffixLen - 1;
		if (!IsSeparator (_buf [k]))
			return false;
	}
	return true;
}

bool FilePath::IsValid (std::string const & filePath)
{
	if (filePath.find_first_of (FilePath::IllegalChars) != std::string::npos)
		return false;
	size_t colonPos = filePath.find_first_of (":");
	if (colonPos != std::string::npos && colonPos != 1)
		return false;
	return true;
}

bool FilePath::IsValidPattern (char const * path)
{
	PathSplitter splitter (path);
	std::string filePath;
	filePath = splitter.GetDrive ();
	filePath += splitter.GetDir ();
	std::string fname;
	fname = splitter.GetFileName ();
	fname += splitter.GetExtension ();
	// File path cannot contain illegal characters
	if (filePath.find_first_of (FilePath::IllegalChars) != std::string::npos)
		return false;
	size_t colonPos = filePath.find_first_of (":");
	if (colonPos != std::string::npos && colonPos != 1)
		return false;
	// If file name contains illegal patch characters this can be only '?' or '*'
	unsigned int curPos = fname.find_first_of (FilePath::IllegalChars);
	while (curPos != std::string::npos)
	{
		char c = fname [curPos];
		if (c != '?' && c != '*')
			return false;
		curPos = fname.find_first_of (FilePath::IllegalChars, ++curPos);
	}
	return true;
}

TmpPath::TmpPath ()
{
	char tmpBuf [MAX_PATH + 1];
	::GetTempPath (sizeof (tmpBuf), tmpBuf);
	Change (tmpBuf);
}

CurrentFolder::CurrentFolder ()
{
	char tmpBuf [MAX_PATH + 1];
	int len = ::GetCurrentDirectory (sizeof (tmpBuf), tmpBuf);
	if (len > sizeof (tmpBuf) || len == 0)
		throw Win::Exception ("Internal error: Cannot get current directory.");
	Change (tmpBuf);
}

void CurrentFolder::Set (char const * path)
{
	Assert (path != 0);
	if (!::SetCurrentDirectory (path))
		throw Win::Exception ("Internal error: Cannot set current directory.", path);
}

ProgramFilesPath::ProgramFilesPath ()
{
	RegKey::System keySystem;
	std::string sysPgmFilesPath = keySystem.GetProgramFilesPath ();
	Change (sysPgmFilesPath.c_str ());
}

SystemFilesPath::SystemFilesPath ()
{
	char buf [MAX_PATH + 1];
	int len = ::GetWindowsDirectory (buf, MAX_PATH);
	if (len > sizeof (buf) || len == 0)
		throw Win::Exception ("Internal error: Cannot get system directory.");
	Change (buf);
}

//
// Path iterators: split path into segments between slashes
//

// Generic FilePathSeq

FilePathSeq::FilePathSeq (char const * path)
	: _path (path)
{}

bool FilePathSeq::HasDrive () const
{
	// c:\folder
	if (_path.length () >= 2)
	{
		int firstChar = _path [0];
		int secondChar = _path [1];
		if (firstChar < 0 || firstChar > 127)
			return false;
		return isalpha (firstChar) && secondChar == ':';
	}
	return false;
}

bool FilePathSeq::IsUNC () const
{
	// \\server\share\folder
	return _path.length () > 2 && FilePath::IsSeparator (_path [0]) && FilePath::IsSeparator (_path [1]);
}

bool FilePathSeq::IsDriveOnly () const
{
	// c:\ or c:
	if (_path.length () < 2)
		return false;

	int firstChar = _path [0];
	int secondChar = _path [1];
	if (firstChar < 0 || firstChar > 127)
		return false;
	if (!isalpha (firstChar) || secondChar != ':')
		return false;
	return _path.length () == 2 
		|| _path.length () == 3 && FilePath::IsSeparator (_path [2]);
}

// Partial path iterator. Don't call with full path or path ending with separator

PartialPathSeq::PartialPathSeq (char const * path)
	: FilePathSeq (path),
	  _cur (-1),
	  _start (0)
{
	Assert (!HasDrive ());
	Assert (!IsUNC ());
	Assert (!FilePath::IsSeparator (_path [_path.length () - 1]));
	Advance ();
}

bool PartialPathSeq::AtEnd () const
{
	return _cur > _path.length ();
}

void PartialPathSeq::Advance ()
{
	_cur++;
	if (_cur < _path.length ())
	{
		_start = _cur;
		while (_cur < _path.length () && !FilePath::IsSeparator (_path [_cur]))
			_cur++;
		_path [_cur] = '\0';
	}
}

char const * PartialPathSeq::GetSegment ()
{
	return &_path [_start];
}

bool PartialPathSeq::IsLastSegment ()
{
	return _cur == _path.length ();
}

// Full path iterator: only call with full path

FullPathSeq::FullPathSeq (char const * path)
	: FilePathSeq (path),
	  _cur (-1),
	  _start (0)
{
	Assert (HasDrive () || IsUNC ());
}

char const * FullPathSeq::GetDrive ()
{
	Assert (_cur == -1);
	Assert (HasDrive ());
	return GetSegment ();
}

char const * FullPathSeq::GetServerShare ()
{
	// \\server\share\folder
	Assert (_cur == -1);
	Assert (IsUNC ());
	int slashCount = 2;
	for (_start = 2; _start < _path.length (); _start++)
	{
		if (FilePath::IsSeparator (_path [_start]))
		{
			slashCount++;
			if (slashCount == 4)
			{
				_path [_start] = '\0';
				_cur = _start;
				return &_path [0];
			}
		}
	}
	if (slashCount == 3)
	{
		// Still OK -- \\server\share
		_cur = _path.length () + 1;
		return _path.c_str ();
	}
	throw Win::Exception ("Illegal path", _path.c_str ());
	return 0;
}

bool FullPathSeq::AtEnd () const
{
	if (_cur == -1)
		return false;

	if (_path.length () == 3)
	{
		// Special case for 'c:\'
		return _cur >= _path.length ();
	}
	else
	{
		return _cur > _path.length ();
	}
}

void FullPathSeq::Advance ()
{
	Assert (_cur == -1 || _path [_cur] == '\0');
	_cur++;
	if (_cur < _path.length ())
	{
		_start = _cur;
		while (_cur < _path.length () && !FilePath::IsSeparator (_path [_cur]))
			_cur++;
		_path [_cur] = '\0';
	}
}

char const * FullPathSeq::GetSegment ()
{
	if (_cur == -1)
		Advance ();
	return &_path [_start];
}

bool FullPathSeq::IsLastSegment ()
{
	if (_cur == -1)
		Advance ();
	return _cur == _path.length ();
}

//
// Path Splitter
//

PathSplitter::PathSplitter (char const * path)
{
	_splitpath (path, _drive, _dir, _fname, _ext);
}

PathSplitter::PathSplitter (std::string const & path)
{
	_splitpath (path.c_str (), _drive, _dir, _fname, _ext);
}

bool PathSplitter::IsValidShare () const
{
	// \\machine\share
	std::string directory (_dir);
	int count = std::count_if (directory.begin (), directory.end (), FilePath::IsSeparator);
	return strlen (_drive) == 0   &&
		   count == 3             &&
		   directory.size () > 3  &&
		   FilePath::IsSeparator (_dir [0]) &&
		   FilePath::IsSeparator (_dir [1]) &&
		   FilePath::IsSeparator (_dir [directory.size () - 1]) &&
		   strlen (_fname) > 0 &&
		   strlen (_ext) == 0;
}

PathMaker::PathMaker (char const * drive, char const * dir, char const * fname, char const * ext)
{
	_makepath (_path, drive, dir, fname, ext);
}

DirectoryListing::DirectoryListing (std::string const & path)
{
	if (!File::Exists (path.c_str ()))
		return;
	if (!File::IsFolder (path.c_str ()))
	{
		InitWithPath (path);
		return;
	}
	_sourcePath.Change (path);
	for (FileSeq seq (_sourcePath.GetAllFilesPath ()); !seq.AtEnd (); seq.Advance ())
	{
		_names.push_back (seq.GetName ());
	}
}

void DirectoryListing::InitWithPath (std::string const & path)
{
	if (!File::Exists (path.c_str ()))
		return;
	PathSplitter splitter (path);
	std::string sourcePath (splitter.GetDrive ());
	sourcePath += splitter.GetDir ();
	std::string fname (splitter.GetFileName ());
	fname += splitter.GetExtension ();
	_sourcePath.Change (sourcePath);
	_names.push_back (fname);
}
