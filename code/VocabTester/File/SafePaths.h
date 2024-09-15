#if !defined (SAFEPATHS_H)
#define SAFEPATHS_H
// --------------------------------
// (c) Reliable Software, 1999-2004
// --------------------------------

class SafePaths
{
public:
	~SafePaths ();
	void MakeTmpCopy (char const * src, char const * dst);
	bool MakeTmpCopyNoEx (char const * src, char const * dst);
	void Remember (char const * path);
	void Remember (std::string const & path);

	bool IsEmpty () const { return _paths.empty (); }
	unsigned size () const { return _paths.size (); }
	std::string const & back () const { return _paths.back (); }

	typedef std::vector<std::string>::const_iterator iterator;
	iterator begin () const { return _paths.begin (); }
	iterator end () const { return _paths.end (); }
private:

	std::vector<std::string> _paths;
};

#endif
