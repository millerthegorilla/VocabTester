#if !defined (DRAGDROP_H)
#define DRAGDROP_H
//-----------------------------------------
// (c) Reliable Software 1998 -- 2003
//-----------------------------------------

#include <Win/Geom.h>
#include <File/Path.h>
#include <Win/Handles.h>
#include <Sys/WinGlobalMem.h>
#include <Com/Com.h>

class Clipboard;

namespace Win
{
	// Access to file list stored by Windows in the Clipboard
	class FileDropPackage: public Handle<HDROP>
	{
	public:
		class Sequencer : public PathSequencer
		{
		public:
			Sequencer (FileDropPackage const & fileDrop);

			unsigned int GetCount () const { return _count; }
			char const * GetFilePath () const { return _path.c_str (); }
			void Advance ();
			bool AtEnd () const { return _cur == _count; }

		private:
			Handle<HDROP>	_handle;
			unsigned int	_cur;
			unsigned int	_count;
			std::string		_path;
		};

	public:
		FileDropPackage (Clipboard const & clipboard);

	protected:
		FileDropPackage (HDROP handle = 0)
			: Handle<HDROP> (handle)
		{}
	};

	// message: WM_DROPFILES

	// Notice: call Win::Dow::Handle::DragAcceptFiles () 
	// to be able to receive this message.

	class FileDrop : public FileDropPackage
	{
	public:
		FileDrop (WPARAM wParam)
			: FileDropPackage (reinterpret_cast<HDROP> (wParam))
		{}
		~FileDrop ()
		{
			::DragFinish (ToNative ());
		}
		bool IsDropInClientArea () const
		{
			Win::Point pt;
			return GetDropPoint (pt);
		}
		// true if within client area
		bool GetDropPoint (Win::Point & pt) const
		{
			return ::DragQueryPoint (ToNative (), &pt) != FALSE;
		}
	};

	class FileDragDrop
	{
	public:
		FileDragDrop::FileDragDrop (std::vector<std::string> const & files,
									bool isRightButtonDrag);

		void Do ();

	private:
		Com::IfacePtr<IDataObject>	_fileDrop;
		Com::IfacePtr<IDropSource>	_dropSource;
		unsigned long				_dropResult;
	};
}

#endif
