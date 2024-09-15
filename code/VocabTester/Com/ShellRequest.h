#if !defined (COPYREQUEST_H)
#define COPYREQUEST_H
//---------------------------------------
//  (c) Reliable Software, 1998 -- 2003
//---------------------------------------
#include <Com/Shell.h>

namespace ShellMan
{
	class FileRequest
	{
	public:
		FileRequest ();

		bool empty () const { return _from.empty (); }

		void MakeItQuiet ()
		{
			_flags |= FOF_SILENT;			// No UI
		}

		void AddFile (char const * path);
		void MakeReadWrite ();
		void MakeReadOnly ();
		void DoDelete (Win::Dow::Handle win, bool quiet = false);

	protected:
		FILEOP_FLAGS	_flags;
		std::string		_from;
	};

	class CopyRequest : public FileRequest
	{
	public:
		CopyRequest (bool quiet = false);

		void OverwriteExisting ()
		{
			_flags |= FOF_NOCONFIRMATION;	//Respond with Yes to All for any dialog box that is displayed.
		}
		void DoCopy (Win::Dow::Handle win, char const * title);
		void AddCopyRequest (char const * from, char const * to);
		void MakeDestinationReadWrite () const;
		void MakeDestinationReadOnly () const;
		void Cleanup () const;
#if !defined (NDEBUG)
		void Dump ();
#else
		void Dump () {}
#endif

	private:
		std::string			_to;
		std::vector<int>	_willOverwrite;
	};
}

#endif
