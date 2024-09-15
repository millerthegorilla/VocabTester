// ---------------------------
// (c) Reliable Software, 2003
// ---------------------------
#include <WinLibBase.h>
#include "FtpDownloader.h"
#include "Ftp.h"
#include "File/FileIo.h"
#include "Sys/GlobalUniqueName.h"

namespace Ftp
{
	void Downloader::StartGetFile (std::string const & server,
				                   std::string const & remoteFile,
								   FilePath const & localFolder,
							       std::string const & localFile)
	{
		std::string serverUrl = "ftp." + server;
		_worker.reset (new Worker (serverUrl, remoteFile, localFolder, localFile, _sink));
		_worker.SetWaitForDeath (30*1000);
	}

	Downloader::Worker::Worker (
			std::string const & server,
			std::string const & remoteFile,
			FilePath const & localFolder,
			std::string const & localFile,
			DownloadSink & sink)
		: _server (server),
		  _remoteFile (remoteFile),
		  _sink (sink)
	{
		// download to a temporary file first, then copy to the destination file
		_localFile = localFolder.GetFilePath (localFile);
		GlobalUniqueName uniqueName;
		std::string tmpFileName = uniqueName.GetName ();
		tmpFileName.append (".tmp");
		_tmpFile = localFolder.GetFilePath (tmpFileName);
		_event.Release ();
	}
	Downloader::Worker::~Worker ()
	{
		File::DeleteNoEx (_tmpFile.c_str ());
	}

	int const ChunkSize = 16 * 1024;

	void Downloader::Worker::Run ()
	{
		for (;;)
		{
			_event.Wait ();
			if (IsDying ())
				return;

			try
			{
				FileIo destFile (_tmpFile, File::CreateAlwaysMode ());
				_sink.OnConnecting ();
				Internet::Access access ("Code Co-op");
				Internet::AutoHandle hInternet (access.Open ());
				if (IsDying ())
					return;
				// anonymous FTP
				Ftp::Session session (hInternet);
				session.SetServer (_server);
				Ftp::AutoHandle hFtp (session.Connect ());
				if (IsDying ())
					return;
				Ftp::FileReadable remoteFile (hFtp, _remoteFile.c_str ());
				File::Size remoteFileSize = remoteFile.GetSize ();
				if (remoteFileSize.IsLarge ())
					throw Win::Exception ("Remote file size > 4GB", _remoteFile.c_str (), 0);
				unsigned long sourceFileSize = remoteFileSize.Low ();
				char buf [ChunkSize];
				unsigned long size = 0;
				int sizeDownloaded = 0;
				do
				{
					size = sizeof (buf);
					remoteFile.Read (buf, size);

					if (IsDying ())
						return;
					
					if (size == 0)
						break;

					sizeDownloaded += size;
					destFile.Write (buf, size);

					if (!_sink.OnProgress (sourceFileSize, sizeDownloaded))
						return;

				} while (true);
				destFile.Close ();
				File::Delete (_localFile.c_str ());
				File::Move (_tmpFile.c_str (), _localFile.c_str ());
				_sink.OnCompleted (DownloadSink::Success);
			}
			catch (Win::Exception)
			{
				_sink.OnFatalError ();
			}
			catch (...)
			{
				_sink.OnFatalError ();
			}
		}
	}
}
