#if !defined (DRIVES_H)
#define DRIVES_H
// ---------------------------------
// (c) Reliable Software, 1999, 2000
// ---------------------------------

class LogicalDrives
{
public:
	LogicalDrives ();
	typedef std::vector<std::string>::const_iterator const_iterator;
	const_iterator begin () const { return _drives.begin (); }
	const_iterator end () const { return _drives.end (); }
private:
	std::vector<std::string> _drives;
};

class DriveInfo
{
public:
	DriveInfo (std::string const & rootPath);
	bool IsUnknown () const { return _type == DRIVE_UNKNOWN; }
	bool HasNoRoot () const { return _type == DRIVE_NO_ROOT_DIR; }
	bool IsRemovable () const { return _type == DRIVE_REMOVABLE; }
	bool IsFixed () const { return _type == DRIVE_FIXED; }
	bool IsRemote () const { return _type == DRIVE_REMOTE; }
	bool IsCdRom () const { return _type == DRIVE_CDROM; }
	bool IsRamDisk () const { return _type == DRIVE_RAMDISK; }

	int MbytesFree () const;
	
private:
	std::string	_rootPath;
	UINT		_type;
};

#endif
