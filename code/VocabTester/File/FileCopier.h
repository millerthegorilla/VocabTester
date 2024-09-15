#if !defined FileCopier_h
#define FileCopier_h

#include "Win/Win.h"

class FileCopier
{
public :
	FileCopier (): _flags (0) {}
	void MakeSilent () { _flags |= FOF_SILENT; }
	void DontConfirm () { _flags |= FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR; }
	void AllowUndo () { _flags |= FOF_ALLOWUNDO; }
	void AddFile (char const * path);
	void CopyTo (char const * destDirectory, 
				 Win::Dow::Handle & winParent = Win::Dow::Handle (0), 
				 char const * title = 0);
	void Clear () {_contents.clear ();}
private :
	std::vector<char>	_contents;
	FILEOP_FLAGS		_flags;
};

#endif
