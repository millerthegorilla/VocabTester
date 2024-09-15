// ---------------------------
// (c) Reliable Software, 2003
// ---------------------------
#include <WinLibBase.h>
#include "BitsDownloader.h"
#include <StringOp.h>
#include "File/Path.h"

namespace BITS
{
	Downloader::Downloader (std::string const & name,
							DownloadSink & sink)
		: ::Downloader (name, sink),
		  _callbacks (new StatusCallbacks (sink))
	{
		try
		{
			_bits.reset (new BITS::Service ());
		}
		catch (...)
		{
			// BITS not installed or failed on very basic level
			_bits.reset ();
		}
	}

	unsigned int Downloader::CountDownloads ()
	{
		BITS::JobSeq  seq (*_bits.get (), _name);
		return seq.Count ();
	}

	void Downloader::StartGetFile (
			std::string const & server,
			std::string const & remoteFile,
			FilePath const & localFolder,
			std::string const & localFile)
	{
		try
		{
			_job.reset (new Job (*_bits, _name));
			std::string remoteFileUrl = "http://www." + server + "/" + remoteFile;
			_job->AddFile (remoteFileUrl, localFolder.GetFilePath (localFile));
			_job->SetCallbacks (_callbacks);
			_job->Resume ();
		}
		catch (...)
		{
			if (_job.get () != 0)
				_job->Cancel ();

			throw;
		}
	}

	bool Downloader::Continue (std::string const & localFile)
	{
		for (BITS::JobSeq  seq (*_bits.get (), _name); !seq.AtEnd (); seq.Advance ())
		{
			BITS::FileSeq fileSeq (seq.GetCurr ());
			if (fileSeq.GetFileCount () == 1)
			{
				if (IsNocaseEqual (fileSeq.GetLocalFileName (), localFile))
				{
					_job.reset (new Job (seq.GetCurr ()));
					break;
				}
			}
		}
		if (_job.get () == 0)
			return false;

		_job->SetCallbacks (_callbacks);
		return true;
	}

	void Downloader::CancelCurrent ()
	{
		if (_job.get () != 0)
			_job->Cancel ();
	}

	void Downloader::CancelAll ()
	{
		for (BITS::JobSeq  seq (*_bits.get (), _name); !seq.AtEnd (); seq.Advance ())
		{
			seq.Cancel ();
		}
	}
}
