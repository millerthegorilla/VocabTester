#if !defined (FILEIO_H)
#define FILEIO_H
//-----------------------------------
// (c) Reliable Software 1998 -- 2002
//-----------------------------------

#include "File.h"

// Standard file i/o interface
class FileIo : public File
{
public:
	FileIo () {}
	FileIo (std::string const & path, 
			File::Mode const & mode, 
			File::Attributes const & attrib = File::NormalAttributes ())
		: File (path, mode, attrib)
	{}
	FileIo (File::File_Ref ref)
		: File (ref)
	{}
	File::Offset GetPosition () const;
	File::Offset SetPosition (File::Offset pos);
	void	Read (void * buf, unsigned long size);
	void	Write (void const * buf, unsigned long size);
	void	FillBuf (void * buf, unsigned long & size);
};

#endif
