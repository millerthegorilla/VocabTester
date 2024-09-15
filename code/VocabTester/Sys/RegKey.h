#if !defined REGKEY_H
#define REGKEY_H
//-----------------------------------------------
//  Registry Key.h
//  (c) Reliable Software, 1996-2002
//-----------------------------------------------

#include <Win/Handles.h>

namespace Win
{
    class Placement;

#ifdef BORLAND_COMPILER
    template<>
    void Disposal<HKEY>::Dispose (RegKey::Handle h) throw ();
#endif
}

namespace RegKey
{
	class Handle : public Win::Handle<HKEY>
	{
	public:
		Handle (HKEY h = 0) 
			: Win::Handle<HKEY> (h)
		{}
	    bool SetValueString (char const * valueName, char const * value, bool quiet = false) const;
    	bool SetValueString (char const * valueName, std::string const & value, bool quiet = false) const
		{
			return SetValueString (valueName, value.c_str (), quiet);
		}
    	bool SetValueLong (char const * valueName, unsigned long value, bool quiet = false) const;
    	bool SetValueBinary (char const * valueName, void const *valueBuffer, unsigned bufferSize, bool quiet = false) const;

		std::string GetStringVal (char const * valueName) const;
	    bool GetValueLong (char const * valueName, unsigned long & value) const;

		//	GetValueBinary returns false with bytesNeeded = 0 for value not found,
		//						   false with bytesNeeded = n for buffer too small (n actual size needed)
		//						   true with bytesNeeded = n for value found (n actual bytes retrieved)
		bool GetValueBinary (char const * valueName, void *valueBuffer, unsigned long bufferSize, unsigned long & bytesNeeded) const;

		bool IsKeyEmpty () const;

	    void DeleteValue (char const * valueName) const;
	    void DeleteSubKey (char const * subKeyName) const;
	};
	
	// Global functions
	void DeleteTree (Handle key);
	void CopyTree (Handle from, Handle to);  //  only handles value sizes up to MAX_PATH
	// Window Placement
	void SaveWinPlacement (Win::Placement const & placement, Handle regKey);
	void ReadWinPlacement (Win::Placement & placement, Handle regKey);

	class AutoHandle : public Win::AutoHandle<RegKey::Handle>
	{
	public:
		AutoHandle (HKEY h = 0) 
			: Win::AutoHandle<RegKey::Handle> (h)
		{}
	};

	class Root : public Handle
	{
	public:
		Root (HKEY key)
			: Handle (key)
		{
			// root keys are always in ThePathMap
		}
	};

	class LocalMachine : public Root
	{
	public:
		LocalMachine ()
			: Root (HKEY_LOCAL_MACHINE)
		{}
	};

	class CurrentUser : public Root
	{
	public:
		CurrentUser ()
			: Root (HKEY_CURRENT_USER)
		{}
	};

	class Existing : public AutoHandle
	{
	public:
	    Existing (Handle keyParent, char const * subKey);
	};

	class ReadOnly : public AutoHandle
	{
	public:
	    ReadOnly (Handle keyParent, char const * subKey);
	};

	class New : public AutoHandle
	{
	public:
	    New (Handle keyParent, char const * subKey);
	};

	class Check: public AutoHandle
	{
	public:
	    Check (Handle keyParent, char const * subKey);
	    bool Exists () const { return _exists; }
	private:
	    bool _exists;
	};

	// HKEY_LOCAL_MACHINE
	//      Software
	//          Microsoft
	//              Windows
	//					CurrentVersion
	class System
	{
	public:
	    System ()
	       :_keyRoot (HKEY_LOCAL_MACHINE),
	        _keyMain (_keyRoot, "Software"),
	        _keyMicrosoft (_keyMain, "Microsoft")
	    {}

		std::string GetProgramFilesPath ();
		std::string GetRegisteredOwner ();

	private:
		Root		_keyRoot;
	    ReadOnly	_keyMain;
	    ReadOnly	_keyMicrosoft;
	};

	class ValueSeq
	{
	public:
	    ValueSeq (RegKey::Handle key);

	    void Advance ();
	    bool AtEnd () const { return _status == ERROR_NO_MORE_ITEMS; }
	    void Rewind () { _cur = -1; }

	    char const * GetName () const { return _name.c_str (); }
	    unsigned long const  GetNameLen () const  { return _nameLen; }
	    char const * GetString () const;
	    BYTE const * GetBytes () const;
	    unsigned long GetLong () const
	    {
	        unsigned long const * p = 
				reinterpret_cast<unsigned long const *> (&_value [0]);
	        return *p;
	    }
	    unsigned long GetValueLen () const { return _valueLen; }
	    unsigned long GetType () const { return _type; }
		bool IsString () const { return _type == REG_SZ; }
	    unsigned long Count () const { return _count; }

	private:
	    HKEY    _key;
	    unsigned long	_count;
	    unsigned long	_cur;
	    long			_status;
	    unsigned long	_type;
	    unsigned long	_nameLen;
		std::string		_name;
	    unsigned long	_valueLen;
		std::string		_value;
	};

	class Seq
	{
	public:
	    Seq (RegKey::Handle key);

	    void Advance ();
	    bool AtEnd () const { return _status == ERROR_NO_MORE_ITEMS; }
	    void Rewind () { _cur = -1; }

	    char const * GetKeyName () const { return _name.c_str (); }
	    unsigned long const  GetKeyNameLen () const  { return _nameLen; }
	    char const * GetClassName () const { return _class.c_str (); }
	    unsigned long const  GetClassNameLen () const { return _classLen; }
	    FILETIME const & GetModificationTime () const { return _lastTime; }
	    unsigned long const  Count () const { return _count; }

	private:
	    HKEY     		_key;
	    unsigned long	_count;
	    unsigned long	_cur;
	    long			_status;
	    unsigned long	_nameLen;
		std::string		_name;
	    unsigned long	_classLen;
		std::string		_class;
	    FILETIME		_lastTime;
	};
}
#endif
