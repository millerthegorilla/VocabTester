#if !defined (BASE64_H)
#define BASE64_H
// ---------------------------
// (c) Reliable Software, 2005
// ---------------------------

#include <GenericIo.h>
#include <bitset>
#include <unmovable_vector.h>

// Based on RFC #3548

namespace Base64
{
	std::string Encode (std::string const & src);
	void Encode (char const * buf, unsigned int bufSize, GenericOutput & output);

	class Decoder
	{
	public:
		Decoder ();
		void Decode (GenericInput<'\0'> & input);

		typedef unmovable_vector<char>::const_iterator const_iterator;
		const_iterator begin () const { return _buf.begin (); }
		const_iterator end () const { return _buf.end (); }
	private:
		unmovable_vector<char>	_buf;
	private:
		unsigned char	 DecodingTable [256];
		std::bitset<256> _isLetterLookup;
	};
}

#endif
