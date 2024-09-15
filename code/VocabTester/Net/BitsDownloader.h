#if !defined (BITSDOWNLOADER_H)
#define BITSDOWNLOADER_H
// ---------------------------
// (c) Reliable Software, 2003
// ---------------------------

#include "Download.h"
#include "BitService.h"
#include <Com/Com.h>

namespace BITS
{
	class Downloader : public ::Downloader
	{
	public:
		Downloader (std::string const & name, DownloadSink & sink);
		
		bool IsAvailable () const { return _bits.get () != 0; }
		unsigned int CountDownloads ();

		void StartGetFile (std::string const & server,
				           std::string const & remoteFile,
						   FilePath const & localFolder,
						   std::string const & localFile);
		bool Continue (std::string const & localFile);

		void CancelCurrent ();
		void CancelAll ();
	private:
		Com::IfacePtr<BITS::StatusCallbacks>  _callbacks;
		std::auto_ptr<BITS::Service>		  _bits;
		std::auto_ptr<BITS::Job>		      _job;
	};
}

#endif
