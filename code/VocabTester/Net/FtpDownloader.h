#if !defined (FTPDOWNLOADER_H)
#define FTPDOWNLOADER_H
// ---------------------------
// (c) Reliable Software, 2003
// ---------------------------

#include "Download.h"
#include <Sys/Synchro.h>
#include <Sys/Active.h>
#include "File/Path.h"

namespace Ftp
{
	class Downloader : public ::Downloader
	{
		class Worker : public ActiveObject
		{
		public:
			Worker (std::string const & server,
				    std::string const & remoteFile,
					FilePath const & localFolder,
					std::string const & localFile,
					DownloadSink & sink);
			~Worker ();
			// Active Object
			void Run ();
			void FlushThread () { _event.Release (); }
		private:
			std::string    _server;
			std::string    _remoteFile;
			std::string    _localFile;
			std::string    _tmpFile;
			DownloadSink & _sink;

			Win::Event	   _event;
		};
	public:
		Downloader (std::string const & name, DownloadSink & sink)
			: ::Downloader (name, sink)
		{}
		bool IsAvailable () const { return true; }
		unsigned int CountDownloads () { return _worker.empty () ? 0 : 1; }
		void StartGetFile (std::string const & server,
				           std::string const & remoteFile,
						   FilePath const & localFolder,
						   std::string const & localFile);
		bool Continue (std::string const & localFile) { return false; }
		void CancelCurrent () { _worker.reset (); }
		void CancelAll () { CancelCurrent (); }
	private:
		auto_active<Worker> _worker;
	};
}

#endif
