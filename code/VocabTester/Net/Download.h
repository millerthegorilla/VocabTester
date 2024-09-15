#if !defined (DOWNLOAD_H)
#define DOWNLOAD_H
// ---------------------------
// (c) Reliable Software, 2003
// ---------------------------

class FilePath;

class DownloadSink
{
	public:
		enum CompletionStatus
		{
			Unrecognized,
			Success,
			AlreadyHandled,
			PartialSuccess,
			CannotDeleteTempFiles
		};
public:
	virtual ~DownloadSink () {}
	// all methods returning boolean value
	// return false, if the download should be terminated
	virtual bool OnConnecting () = 0;
	virtual bool OnProgress (int bytesTotal, int bytesTransferred) = 0;
	virtual bool OnCompleted (CompletionStatus status) = 0;
	virtual void OnTransientError () = 0;
	virtual void OnFatalError () = 0;
};

class Downloader
{
public: 
	virtual ~Downloader () {}
	virtual bool IsAvailable () const = 0;
	virtual unsigned int CountDownloads () = 0;
	virtual void StartGetFile (std::string const & server,
				               std::string const & remoteFile,
							   FilePath const & localFolder,
							   std::string const & localFile) = 0;
	virtual bool Continue (std::string const & localFile) = 0;
	virtual void CancelCurrent () = 0;
	virtual void CancelAll () = 0;
protected:
	Downloader (std::string const & name, DownloadSink & sink)
		: _name (name), 
		  _sink (sink)
	{}
protected:
	std::string	   _name;
	DownloadSink & _sink;
};

#endif
