#if !defined (VPATH_H)
#define VPATH_H
// (c) Reliable Software, 2003

// File Path stored as a vector of strings
// Easy to manipulate

class Vpath
{
public:
	typedef std::vector<std::string>::iterator iterator;
	typedef std::vector<std::string>::const_iterator const_iterator;
	iterator begin () { return _segments.begin (); }
	iterator end () { return _segments.end (); }
	const_iterator begin () const { return _segments.begin (); }
	const_iterator end () const { return _segments.end (); }
public:
	Vpath () {}
	explicit Vpath (std::string const & strPath)
	{
		SetPath (strPath);
	}
	Vpath (Vpath const & vpath);
	Vpath & operator = (Vpath const & vpath);
	void SetPath (std::string const & strPath);
	void DirDown (std::string const & segment)
	{
		_segments.push_back (segment);
	}
	void AppendFileName (std::string const & name)
	{
		_segments.push_back (name);
	}
	void DirUp ()
	{
		_segments.pop_back ();
	}
	void RemoveFileName ()
	{
		_segments.pop_back ();
	}
	unsigned Depth () const { return _segments.size (); }
	std::string ToString () const;
	std::string GetFilePath (std::string const & fileName);
private:
	static char SEPARATOR;
private:
	std::vector<std::string> _segments;
};

#endif
