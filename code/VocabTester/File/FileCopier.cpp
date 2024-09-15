#include <WinLibBase.h>
#include "FileCopier.h"


void FileCopier::AddFile (char const * path)
{
	_contents.insert (_contents.end (), path, path + strlen (path) + 1);
}

void FileCopier::CopyTo (char const * destDirectory , Win::Dow::Handle & winParent, char const * title)
{
	if (_contents.empty ())
		return;
	_contents.push_back ('\0');
	std::vector<char> toPath (destDirectory, destDirectory + strlen (destDirectory) + 1);
	toPath.push_back ('\0');

	SHFILEOPSTRUCT fileInfo;
	memset (&fileInfo, 0, sizeof (fileInfo));
	fileInfo.hwnd = winParent.ToNative ();
	fileInfo.wFunc = FO_COPY; 
	fileInfo.pFrom = &_contents [0];
	fileInfo.pTo = &toPath [0];
	fileInfo.fFlags = _flags;
	fileInfo.lpszProgressTitle = title;
	if (::SHFileOperation (&fileInfo))
	{
		if (!fileInfo.fAnyOperationsAborted)
			throw Win::Exception ("Internal error: Cannot copy files to:", &toPath [0]);
	}
	_contents.pop_back ();// '\0'
}
