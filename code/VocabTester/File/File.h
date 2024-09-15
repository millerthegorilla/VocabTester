#if !defined FILE_H
#define FILE_H
//-----------------------------------
// (c) Reliable Software 1997 -- 2004
//-----------------------------------

#include <LargeInt.h>
#include <StringOp.h>
#include <Sys/PackedTime.h>
#include <Sys/Dll.h>

namespace Win { class ExitException; }
namespace Security { class Acl; }

class File
{
public:
	class Size: public LargeInteger
	{
	public:
		Size () {}
		Size (unsigned long low, long high)
			: LargeInteger (low, high)
		{}
	};

	// Note: We never use negative offsets.
	// If need arises, create a separate class
	class Offset: public LargeInteger
	{
	public:
		Offset () {}
		Offset (File::Size size) : LargeInteger (size.ToNative ()) {}
		Offset (LargeInteger li) : LargeInteger (li) {}
		Offset (unsigned long low, long high)
			: LargeInteger (low, high)
		{}
		void Reset () { _value.LowPart = 0; _value.HighPart = 0; }
		static Offset Invalid;
	};

	class Sizer
	{
	public:
		Sizer (HANDLE hFile)
			: _hFile (hFile)
		{}
		virtual bool Rewind () = 0;
		virtual File::Size GetSize () const = 0;
		virtual File::Offset GetPosition () const = 0;
		virtual File::Offset SetPosition (File::Offset pos) = 0;
	protected:
		HANDLE _hFile;
	};

	class Sizer64: public Sizer
	{
	public:
		Sizer64 (HANDLE hFile);
		bool Rewind ();
		File::Size GetSize () const;
		File::Offset GetPosition () const;
		File::Offset SetPosition (File::Offset pos);
	private:
		Dll	_dll;
	};

	class Sizer32: public Sizer
	{
	public:
		Sizer32 (HANDLE hFile);
		bool Rewind ();
		File::Size GetSize () const;
		File::Offset GetPosition () const;
		File::Offset SetPosition (File::Offset pos);
	};

	class LegalizeChar : public std::unary_function<char, char>
	{
	public:
		LegalizeChar (CharSet const & badSet)
			: _badSet (badSet)
		{}

		char operator () (char c)
		{
			if (c <= 31 || _badSet.Contains (c))
				return '#';
			else
				return c;
		}

	private:
		CharSet const &	_badSet;
	};

	class ReadOnlyMode;
	class CreateAlwaysMode;
	class OpenAlwaysMode;
	class OpenExistingMode;
	class Attributes;
	class NormalAttributes;
	class DirectoryAttributes;
	class Mode;

public:
	File (std::string const & path, Mode const & mode, Attributes const & attrib);
	File (std::string const & path, Mode const & mode);
	~File () throw () { Close (); }
	bool	FileOk () const throw () { return _hFile != INVALID_HANDLE_VALUE; }
	bool	Open (std::string const & path, Mode const & mode, bool quiet = false);
	bool	Create (std::string const & path);
	void	OpenReadOnly (std::string const & path, bool quiet = false);
	void	Close () throw ();
	bool	Flush () { return ::FlushFileBuffers (_hFile) != FALSE; }
	void	Empty ();
	void	GetWriteTime (FILETIME & lastWrite) const;
	File::Size GetSize () const;
	void	Resize (File::Size newSize);
	// Must be open with Access WriteDac
	void	SetAcl (Security::Acl const & acl);

	HANDLE ToNative () const { return _hFile; }

	// Static methods
	static bool IsValidName (char const * name) throw ();
	static bool IsValidExtension (char const * extension) throw ();
	static void LegalizeName (std::string & name) throw ();
	static bool IsEqualQuick (char const * pathOld, char const * pathNew); 
	static bool IsContentsEqual (char const * pathOld, char const * pathNew); 
	static bool Exists (std::string const & path) throw ();
	static bool IsFolder (char const * path) throw ();
	static bool CreateFolder (char const * path, bool quiet = true);
	static bool CreateNewFolder (char const * path, bool quiet = true);
	static bool IsReadOnly (std::string const & path) throw ();
	static void MakeReadOnly (char const * path);
	static void MakeReadWrite (char const * path);
	static bool MakeReadOnlyNoEx (std::string const & path) throw ();
	static bool MakeReadWriteNoEx (std::string const & path) throw ();
	static void Touch (char const * path);
	static bool TouchNoEx (std::string const & path) throw ();
	static bool ForceTouchNoEx (std::string const & path) throw ();
	static void Delete (char const * path);
	static bool DeleteNoEx (std::string const & path) throw ();
	static bool Copy (char const * pathFrom, char const * pathTo);
	static bool CopyNoEx (char const * pathFrom, char const * pathTo) throw ();
	static bool SafeCopy (char const * pathFrom, char const * pathTo);
	static bool SafeCopyNoEx (char const * pathFrom, char const * pathTo) throw ();
	static std::string Rename2Previous (char const * path);
	static std::string Rename2PreviousNoEx (char const * path) throw ();
	static std::string Rename2CopyOf (char const * path);
	static std::string Rename2CopyOfNoEx (char const * path) throw ();
	static std::string CreateUniqueName (char const * path, char const * additionalNamePart, bool isPrefix = true);
	static void Move (char const * pathFrom, char const * pathTo);
	static bool MoveNoEx (char const * pathFrom, char const * pathTo) throw ();
	static void CopyOrDie (char const * pathFrom, char const * pathTo) throw (Win::ExitException);
	static void Rename2PreviousOrDie (char const * path) throw (Win::ExitException);
	static bool IsTreeEmpty (char const * root);
	static bool CleanupTree (char const * root);

	// Constants
	static char const NameIllegalChars [];
	static CharSet const NameIllegalCharSet;

	struct File_Ref
	{
		HANDLE _hFile;
		File_Ref (File const & file)
		{
			File & tmp = const_cast<File &> (file);
			_hFile = tmp.Release ();
		}
	};
			
protected:
	friend struct File_Ref;

	File (File_Ref ref)
	{
		InitHandle (ref._hFile);
	}

	File () : _hFile (INVALID_HANDLE_VALUE) {}
	// acquire file handle
	File (File & file)
	{
		InitHandle (file.Release ());
	}
	bool SimpleRead (void * buf, ULONG & size) throw ()
	{
		ULONG sizeAsked = size;
		return ::ReadFile (_hFile, buf, sizeAsked, & size, NULL)	!= FALSE;
	}
	bool SimpleWrite (void const * buf, ULONG & size) throw ()
	{
		ULONG sizeAsked = size;
		return ::WriteFile (_hFile, buf, sizeAsked, & size, NULL) != FALSE;
	}
	bool Rewind () throw ()
	{
		return _sizer->Rewind ();
	}

	HANDLE Release ()
	{
		HANDLE tmp = _hFile;
		InitHandle (INVALID_HANDLE_VALUE);
		return tmp;
	}

	bool InitHandle (HANDLE hFile);
	void InitSizer ();

	static std::string RenameUnique (char const * path, char const * namePrefix);
	static std::string RenameUniqueNoEx (char const * path, char const * namePrefix) throw ();

public:
	// usage:
	// File::Mode mode;
	// mode << File::Access::Read << File::Create::Existing;
	class Mode
	{
	public:
		class Access
		{
		public:
			enum Bits
			{
				Read = GENERIC_READ,
				Write = GENERIC_WRITE,
				ReadWrite = (GENERIC_READ | GENERIC_WRITE),
				WriteDac = WRITE_DAC // discretionary access control
			};
		};

		class Share
		{
		public:
			enum Bits
			{
				Delete = FILE_SHARE_DELETE,
				Read = FILE_SHARE_READ,
				Write = FILE_SHARE_WRITE,
				All = (FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE)
			};
		};

		class Create
		{
		public:
			enum Bits
			{
				New = CREATE_NEW, // fail if exists
				Existing = OPEN_EXISTING, // fail if doesn't exist
				Always = CREATE_ALWAYS, // truncate if exists
				OpenAlways = OPEN_ALWAYS, // create if doesn't exist
				TruncateExisting = TRUNCATE_EXISTING // fails if doesn't exist
			};
		};

	public:
		Mode () : _access (0), _share (0), _create (0) {}
		unsigned const & GetAccess () const { return _access; }
		unsigned const & GetShare () const { return _share; }
		unsigned const & GetCreate () const { return _create; }
		bool IsReadOnly () const
		{ 
			return _access & GENERIC_READ
				&& !(_access & GENERIC_WRITE);
		}

		Mode & operator << (Access::Bits access)
		{
			_access |= access;
			return *this;
		}
		Mode & operator << (Share::Bits share)
		{
			_share |= share;
			return *this;
		}
		Mode & operator << (Create::Bits create)
		{
			_create |= create;
			return *this;
		}
	private:
		unsigned	_access;
		unsigned	_share;
		unsigned	_create;
	};

protected:
	HANDLE						_hFile;
	std::auto_ptr<File::Sizer>	_sizer;
};

//----------------
// Utility classes
//----------------

// Prefabricated file modes
class File::ReadOnlyMode: public Mode
{
public:
	ReadOnlyMode ()
	{
		(*this) << File::Mode::Access::Read 
				<< File::Mode::Create::Existing
				<< File::Mode::Share::Read 
				<< File::Mode::Share::Write;
	}
};

class File::CreateAlwaysMode: public Mode
{
public:
	CreateAlwaysMode ()
	{
		(*this) << File::Mode::Access::ReadWrite 
				<< File::Mode::Create::Always;
	}
};

class File::OpenAlwaysMode: public Mode
{
public:
	OpenAlwaysMode ()
	{
		(*this) << File::Mode::Access::ReadWrite 
				<< File::Mode::Create::OpenAlways;
	}
};

class File::OpenExistingMode: public Mode
{
public:
	OpenExistingMode ()
	{
		(*this) << File::Mode::Access::ReadWrite 
				<< File::Mode::Create::Existing;
	}
};

// Usage:
//	File::Attributes attrib;
//	attrib << File::Attributes::DeleteOnClose;
class File::Attributes
{
public:
	Attributes () : _bits (0) {} // default: invalid
	Attributes (char const * filePath);
	Attributes (std::string const & filePath);

	bool AreValid () const { return _bits != INVALID_FILE_ATTRIBUTES; }
	bool IsFolder () const { return (_bits & FILE_ATTRIBUTE_DIRECTORY) != 0; }
	bool IsReadOnly () const { return (_bits & FILE_ATTRIBUTE_READONLY) != 0; }
	bool IsHidden () const { return (_bits & FILE_ATTRIBUTE_HIDDEN) != 0; }
	operator unsigned long () const { return _bits; }
	void MakeReadWrite () { _bits &= ~FILE_ATTRIBUTE_READONLY; }
	void MakeNormal () { _bits = FILE_ATTRIBUTE_NORMAL; }
	enum Bits
	{
	    Normal  = FILE_ATTRIBUTE_NORMAL, // The file has no other attributes set.
														// This attribute is valid only if used alone.
	    Archive = FILE_ATTRIBUTE_ARCHIVE,
	    Encrypted = FILE_ATTRIBUTE_ENCRYPTED,
	    Hidden = FILE_ATTRIBUTE_HIDDEN,
	    NotContentIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
	    OffLine = FILE_ATTRIBUTE_OFFLINE,
	    ReadOnly = FILE_ATTRIBUTE_READONLY,
	    System = FILE_ATTRIBUTE_SYSTEM,
	    Temporary = FILE_ATTRIBUTE_TEMPORARY,
	    WriteThrough = FILE_FLAG_WRITE_THROUGH,
	    Overlapped = FILE_FLAG_OVERLAPPED,
	    NoBuffering = FILE_FLAG_NO_BUFFERING,
	    RandomAccess = FILE_FLAG_RANDOM_ACCESS,
	    SequentialScan = FILE_FLAG_SEQUENTIAL_SCAN,
	    DeleteOnClose = FILE_FLAG_DELETE_ON_CLOSE,
	    Backup = FILE_FLAG_BACKUP_SEMANTICS,
	    Posix = FILE_FLAG_POSIX_SEMANTICS,
	    ReparsePoint = FILE_FLAG_OPEN_REPARSE_POINT,
	    NoRecall = FILE_FLAG_OPEN_NO_RECALL
	};

	Attributes & operator << (Attributes::Bits bits)
	{
		_bits |= bits;
		return *this;
	}

private:
	void Init (char const * path);
private:
	unsigned long _bits;
};

class File::NormalAttributes : public File::Attributes
{
public:
	NormalAttributes ()
	{
        MakeNormal ();
	}
};

// Note: directory attributes are not supported on Win Me/98/95
class File::DirectoryAttributes: public File::Attributes
{
public:
	DirectoryAttributes ()
	{
		*this << Attributes::Backup; // required for opening directories
	}
};

//--------------------------------------------------
// Opens file in order to extract some info about it
//--------------------------------------------------
class FileInfo: public File
{
public:
	FileInfo (std::string const & path)
		: File (path, File::ReadOnlyMode ())
	{}
};

class FileInfoWritable: public File
{
public:
	FileInfoWritable (std::string const & path)
		: File (path, File::OpenExistingMode ())
	{}
	void Touch ();
};

class FileTime: public PackedTime
{
public:
	FileTime (): PackedTime () {}
	FileTime (PackedTime const & time): PackedTime (time) {}
	FileTime (File const & file)
	{
		file.GetWriteTime (_time);
	}
	FileTime (FILETIME time)
		: PackedTime (time)
	{}
};

class FileVersion
{
public:
	FileVersion (std::string const & filePath);
	int GetFileVersionMS () const { return _version.dwFileVersionMS; }
	int GetFileVersionLS () const { return _version.dwFileVersionLS; }
private:
	VS_FIXEDFILEINFO _version;
};

#endif

