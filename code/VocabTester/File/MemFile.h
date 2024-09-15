#if !defined MEMFILE_H
#define MEMFILE_H
//------------------------------------
// (c) Reliable Software 1997, 98, 99
//------------------------------------

#include "File.h"

class MappableFile: public File
{
public:
	MappableFile (std::string const & path,
				  File::Mode const & mode);
	MappableFile (std::string const & path,
				  File::Mode const & mode,
				  File::Size size);
    MappableFile (File & file, bool readOnly);
	~MappableFile () throw () { Close (); }
	void Close () throw ();
protected:
	MappableFile () :_hMap (0) {}
	void Map (File::Size size, bool readOnly);
	void Map (bool readOnly);
	void Unmap () throw ();
protected:
	HANDLE			_hMap;
};


class MemFile: public MappableFile
{
public:
	enum { MAX_BUFFER = 0x40000000 }; // 1 GB
public:
	MemFile (std::string const & path,
			 File::Mode const & mode,
			 File::Size size);
	MemFile (std::string const & path,
			 File::Mode const & mode);
    MemFile (File & file, bool readOnly);
    ~MemFile() throw ();
	void Close () throw ();
	void Close (File::Size finalSize);
    char *  GetBuf () { return _buf; }
    char const *  GetBuf () const { return _buf; }
	unsigned long GetBufSize () const
		{ return _bufSize; }
	void Flush ();
	// invalidates previous buffer
	char * ResizeFile (File::Size size);
	char * Reallocate ( unsigned long bufSize, File::Offset offset);
    unsigned long GetCheckSum () const;
protected:
	MemFile () : _readOnly (true), _buf (0), _bufSize (0) {}
	void InitBufSize (File::Size size);
	void InitBufSize ();
	void Allocate ( File::Offset fileOffset = File::Offset (0, 0));
	void Deallocate () throw ();
protected:
	bool			_readOnly;
	char		  * _buf;
	unsigned long	_bufSize;
};

class MemFileReadOnly: public MemFile
{
public:
    MemFileReadOnly (std::string const & path)
		: MemFile (path, File::ReadOnlyMode ())
	{}
    MemFileReadOnly (File & file);
};

class MemFileNew: public MemFile
{
public:
	MemFileNew (std::string const & path, File::Size size)
		: MemFile (path, File::CreateAlwaysMode (), size)
	{}
};

class MemFileExisting: public MemFile
{
public:
    MemFileExisting (std::string const & path)
		: MemFile (path, File::OpenExistingMode ())
	{}
};

class MemFileAlways: public MemFile
{
public:
	MemFileAlways (std::string const & path)
		: MemFile (path, File::OpenAlwaysMode ())
	{}
};

class LokFile: public MemFile
{
public:
    LokFile () {}
    void New (char const *path);
    bool Open (char const *path);
	void Stamp (unsigned long timeStamp);
	unsigned long GetStamp () const { return *(reinterpret_cast<unsigned long const *> (_buf)); }

	static bool IsLocked (char const *path) throw ();
};

#endif
