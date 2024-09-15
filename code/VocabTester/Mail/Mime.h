#if !defined (MIME_H)
#define MIME_H
// --------------------------
// (c) Reliable Software 2005
// --------------------------
#include <Parse/NamedPair.h>

namespace MIME
{
	class ContentType
	{
	public:
		typedef NamedPair<'='> Attribute;
	public:
		void Clear ()
		{
			_type.clear ();
			_subtype.clear ();
			_encoding.clear ();
			_boundary.clear ();
			_attribute.Clear ();
		}
		void Init (std::string const & headerValue, std::string const & comment);
		void SetEncoding (std::string const & encoding) { _encoding = encoding; }
		bool IsMultiPart () const { return _type == "multipart"; }
		bool IsText () const { return _type == "text"; }
		bool IsApplication () const { return _type == "application"; }
		bool IsOctetStream () const { return _subtype == "octet-stream"; }

		bool IsBase64 () const { return _encoding == "base64"; }

		// boundary prepended with --
		std::string const & GetBoundary () const { return _boundary; }
		std::string const & GetType () const { return _type; }
		std::string const & GetSubtype () const { return _subtype; }
		std::string const & GetEncoding () const { return _encoding; }
		Attribute const & GetAttribute () const { return _attribute; }
	private:
		std::string _type;
		std::string _subtype;
		std::string _encoding;
		std::string _boundary;
		Attribute _attribute;
	};
}

#endif
