//-----------------------------------------
// (c) Reliable Software 1997 -- 2004
//-----------------------------------------

#include <WinLibBase.h>
#include "RegKey.h"

#include <Win/Geom.h>
#include <Sys/Synchro.h>
#include <Sys/SysVer.h>

namespace RegKey
{
	// map handles into pairs: <parent handle, key name>
	class PathMap
	{
	public:
		PathMap ();
		~PathMap ();
		// Revisit: switch from using HKEY to RegKey::Handle
		void Add (HKEY h, HKEY keyParent, char const * subKeyName)
		{
			Win::Lock lock (_critSect);
			_map [h] = std::make_pair (keyParent, subKeyName);
		}
		std::string MakePath (HKEY keyParent, char const * subKeyName);
		std::string GetPath (HKEY h);
		void Remove (HKEY h) throw ();
	private:
		Win::CritSection	_critSect;
		std::map<HKEY, std::pair<HKEY, std::string> > _map;
	};

	PathMap ThePathMap;
}

template<>
void Win::Disposal<RegKey::Handle>::Dispose (RegKey::Handle h) throw ()
{
	::RegCloseKey (h.ToNative ());
	RegKey::ThePathMap.Remove (h.ToNative ());
}

namespace RegKey
{
	PathMap::PathMap ()
	{
		// add top-level keys
		_map [HKEY_LOCAL_MACHINE] = std::make_pair (HKEY (0), "HKEY_LOCAL_MACHINE");	
		_map [HKEY_CURRENT_USER] = std::make_pair (HKEY (0), "HKEY_CURRENT_USER");	
		_map [HKEY_USERS] = std::make_pair (HKEY (0), "HKEY_USERS");	
		_map [HKEY_CLASSES_ROOT] = std::make_pair (HKEY(0), "HKEY_CLASSES_ROOT");
	}

	PathMap::~PathMap ()
	{
		size_t left = _map.size ();
		Assert (left == 4);
	}
		
	std::string PathMap::MakePath (HKEY keyParent, char const * subKeyName)
	{
		// GetPath takes a lock
		std::string path = GetPath (keyParent);
		path += '\\';
		path += subKeyName;
		return path;
	}

	std::string PathMap::GetPath (HKEY h)
	{
		Win::Lock lock (_critSect);
		std::string path;
		while (h != 0)
		{
			std::pair<HKEY, std::string> pair = _map [h];
			h = pair.first;
			path.insert (0, pair.second);
			path.insert (0, 1, '\\');
		}
		path.erase (path.begin ());
		return path;
	}

	void PathMap::Remove (HKEY h) throw ()
	{
		Win::Lock lock (_critSect);
		_map.erase (h);
	}

	bool Handle::SetValueString (char const * valueName, char const * value, bool quiet) const
	{
		long err = ::RegSetValueEx (H (), 
									valueName, 
									0, 
									REG_SZ, 
									reinterpret_cast<unsigned char const *> (value),
									strlen (value) + 1) ;
		if (!quiet && err != ERROR_SUCCESS)
		{
			throw Win::Exception ("Registry operation failed. Writing a string.", valueName, err);
		}
		return err == ERROR_SUCCESS;
	}

	bool Handle::SetValueLong (char const * valueName, unsigned long value, bool quiet) const
	{
		long err = ::RegSetValueEx (H (), 
									valueName,
									0,
									REG_DWORD,
									reinterpret_cast<unsigned char const *> (&value),
									sizeof (unsigned long));
		if (!quiet && err != ERROR_SUCCESS)
		{
			throw Win::Exception ("Registry operation failed. Writing a long.", valueName, err);
		}
		return err == ERROR_SUCCESS;
	}

	bool Handle::SetValueBinary (char const * valueName, void const * valueBuffer, unsigned bufferSize, bool quiet) const
	{
		long err = ::RegSetValueEx (H (), 
									valueName,
									0,
									REG_BINARY,
									(BYTE const *) valueBuffer,
									bufferSize);
		if (!quiet && err != ERROR_SUCCESS)
		{
			throw Win::Exception ("Registry operation failed. Writing a binary.", valueName, err);
		}
		return err == ERROR_SUCCESS;
	}

	std::string Handle::GetStringVal (char const * valueName) const
	{
		unsigned long bufSize = 0;
		unsigned long type;
		long err = ::RegQueryValueEx (H (),
									valueName,
									0, // reserved
									&type,
									0, // no buffer, query size only
									&bufSize);

		if (err == ERROR_FILE_NOT_FOUND) // Value not found
			return std::string ();
		if (err != ERROR_SUCCESS)
			throw Win::Exception ("Registry operation failed. Cannot query value.", valueName, err);
		if (type != REG_SZ && type != REG_EXPAND_SZ)
			throw Win::Exception ("Registry operation failed. Wrong type of value.", valueName);

		std::string buf;
		// bufSize includes the terminating null!
		buf.resize (bufSize);
		if (bufSize != 0)
		{
			err = ::RegQueryValueEx (H (),
									valueName,
									0,
									&type,
									reinterpret_cast<unsigned char *> (&buf [0]),
									&bufSize);
			if (err != ERROR_SUCCESS)
				throw Win::Exception ("Registry operation failed. Cannot retrieve key string value.", 
										valueName, err);
			buf.resize (strlen (&buf [0]));
		}
		return buf;
	}

	bool Handle::GetValueLong (char const * valueName, unsigned long & value) const
	{
		unsigned long type = 0;
		unsigned long size = sizeof (unsigned long);
		unsigned long dw = 1;

		long err = ::RegQueryValueEx (H (),
									valueName,
									0,
									&type,
									reinterpret_cast<unsigned char *> (&dw),
									&size);

		if (err == ERROR_FILE_NOT_FOUND)
		{
			// Value not found
			Win::ClearError ();
			return false;
		}
		if (type != REG_DWORD)
			throw Win::Exception ("Registry operation failed. Wrong type of value.", valueName);
		if (err != ERROR_SUCCESS)
			throw Win::Exception ("Registry operation failed. Cannot retrieve key binary value.", valueName, err);
		value = dw;
		return true;
	}

	bool Handle::GetValueBinary (char const * valueName, void *valueBuffer, unsigned long bufferSize, unsigned long & bytesNeeded) const
	{
		unsigned long type = 0;
		unsigned long cb = bufferSize;

		long err = ::RegQueryValueEx (H (),
									valueName,
									0,
									&type,
									(LPBYTE) valueBuffer,
									&cb);

		if (err == ERROR_FILE_NOT_FOUND)
		{
			// Value not found
			Win::ClearError ();
			bytesNeeded = 0;
			return false;
		}

		if (type != REG_BINARY)
			throw Win::Exception ("Registry operation failed. Wrong type of value.", valueName);

		if (err == ERROR_MORE_DATA)
		{
			//	Buffer too small
			Win::ClearError ();
			bytesNeeded = cb;	//	how many bytes needed
			return false;
		}

		if (err != ERROR_SUCCESS)
			throw Win::Exception ("Registry operation failed. Cannot retrieve key binary value.", valueName, err);

		bytesNeeded = cb;	//	how bytes actually filled in
		return true;
	}

	void Handle::DeleteValue (char const * valueName) const
	{
		long err = ::RegDeleteValue (H (), valueName);
		if (err != ERROR_SUCCESS)
		{
			if (err == ERROR_FILE_NOT_FOUND)
				Win::ClearError ();
			else
				throw Win::Exception ("Registry operation failed - cannot delete value", valueName, err);
		}
	}

	void Handle::DeleteSubKey (char const * subKeyName) const
	{
		long err = ::RegDeleteKey (H (), subKeyName);
		if (err != ERROR_SUCCESS)
		{
			if (err == ERROR_FILE_NOT_FOUND)
				Win::ClearError ();
			else
				throw Win::Exception ("Registry operation failed - cannot delete key",
										ThePathMap.MakePath (H (), subKeyName).c_str (), 
										err);
		}
	}

	bool Handle::IsKeyEmpty () const
	{
		long    status;
		unsigned long	valueCount;
		unsigned long   subkeyCount;
		status = ::RegQueryInfoKey (H (),			// handle of key to query 
									0,				// address of buffer for class string 
									0,				// address of size of class string buffer 
									0,				// reserved 
									&subkeyCount,	// address of buffer for number of subkeys 
									0,				// address of buffer for longest subkey name length  
									0,				// address of buffer for longest class string length 
									&valueCount,	// address of buffer for number of value entries 
									0,				// address of buffer for longest value name length 
									0,				// address of buffer for longest value data length 
									0,				// address of buffer for security descriptor length 
									0);				// address of buffer for last write time 
		if (status != ERROR_SUCCESS)
			throw Win::Exception ("Registry operation failed - cannot query key info", 
									ThePathMap.GetPath (H ()).c_str (), 
									status);
		return valueCount + subkeyCount == 0;
	}

	void SaveWinPlacement (Win::Placement const & placement, Handle regKey)
	{
		regKey.SetValueLong ("Flags", placement.GetFlags ());
		Win::Point pt;
		placement.GetMinCorner (pt);
		regKey.SetValueLong ("XMin", pt.x);
		regKey.SetValueLong ("YMin", pt.y);
		placement.GetMaxCorner (pt);
		regKey.SetValueLong ("XMax", pt.x);
		regKey.SetValueLong ("YMax", pt.y);
		regKey.SetValueLong ("Maximize", placement.IsMaximized () ? 1 : 0);
		regKey.SetValueLong ("Minimize", placement.IsMinimized () ? 1 : 0);
		Win::Rect rect;
		placement.GetRect (rect);
		regKey.SetValueLong ("Top", rect.top);
		regKey.SetValueLong ("Left", rect.left);
		regKey.SetValueLong ("Width", rect.Width ());
		regKey.SetValueLong ("Height", rect.Height ());
	}

	void ReadWinPlacement (Win::Placement & placement, RegKey::Handle regKey)
	{
		if (!regKey.IsNull ())
		{
			unsigned long value;
			if (regKey.GetValueLong ("Maximize", value))
			{
				if (value == 1)
					placement.SetMaximized ();
			}
			else
				return;
			if (regKey.GetValueLong ("Minimize", value))
			{
				if (value == 1)
					placement.SetMinimized ();
			}
			else
				return;
			// Normal window rectangle
			Win::Rect rect;
			if (regKey.GetValueLong ("Top", value))
				rect.top = value;
			else
				return;
			if (regKey.GetValueLong ("Left", value))
				rect.left = value;
			else
				return;
			if (regKey.GetValueLong ("Width", value))
				rect.right = rect.left + value;
			else
				return;
			if (regKey.GetValueLong ("Height", value))
				rect.bottom = rect.top + value;
			else
				return;
			placement.SetRect (rect);
			// flags
			if (regKey.GetValueLong ("Flags", value))
				placement.SetFlags (value);
			else
				return;
			// Corner when minimized
			Win::Point pt;
			if (regKey.GetValueLong ("XMin", value))
				pt.x = value;
			else
				return;
			if (regKey.GetValueLong ("YMin", value))
				pt.y = value;
			else
				return;
			placement.SetMinCorner (pt);
			// Corner when maximized
			if (regKey.GetValueLong ("XMax", value))
				pt.x = value;
			else
				return;
			if (regKey.GetValueLong ("YMax", value))
				pt.y = value;
			else
				return;
			placement.SetMinCorner (pt);
		}
	}

	// Global (recursive) functions
	void DeleteTree (RegKey::Handle key)
	{
		// Delete values
		for (ValueSeq valueSeq (key); !valueSeq.AtEnd (); valueSeq.Advance ())
		{
			key.DeleteValue (valueSeq.GetName ());
		}
		// Delete subkeys
		std::vector<std::string> subKeyName;
		for (Seq keySeq (key); !keySeq.AtEnd (); keySeq.Advance ())
		{
			subKeyName.push_back (keySeq.GetKeyName ());
		}
		std::vector<std::string>::iterator it = subKeyName.begin ();
		std::vector<std::string>::iterator end = subKeyName.end ();
		while (it != end)
		{
			{
				RegKey::Existing subKey (key, it->c_str ());
				RegKey::DeleteTree (subKey);
			}
			key.DeleteSubKey (it->c_str ());
			++it;
		}
	}

	void CopyTree (RegKey::Handle from, RegKey::Handle to)
	{
		// Copy values
		for (ValueSeq valueSeq (from); !valueSeq.AtEnd (); valueSeq.Advance ())
		{
			switch (valueSeq.GetType ())
			{
			case REG_DWORD:
				to.SetValueLong (valueSeq.GetName (), valueSeq.GetLong ());
				break;
			case REG_SZ:
				to.SetValueString (valueSeq.GetName (), valueSeq.GetString ());
				break;
			case REG_BINARY:
				to.SetValueBinary (valueSeq.GetName (), valueSeq.GetBytes (), valueSeq.GetValueLen ());
				break;
			default:
				throw Win::Exception ("Internal error: CopyTree -- unsupported registry value format");
				break;
			}
		}
		// Copy subkeys
		for (Seq keySeq (from); !keySeq.AtEnd (); keySeq.Advance ())
		{
			New toSubKey (to, keySeq.GetKeyName ());
			Existing fromSubKey (from, keySeq.GetKeyName ());
			RegKey::CopyTree (fromSubKey, toSubKey);
		}
	}

	// Specialized keys

	Existing::Existing (Handle keyParent, char const * subKeyName)
	{
		HKEY h;
		long err = ::RegOpenKeyEx (keyParent.ToNative (), 
								subKeyName, 
								0,
								KEY_READ | KEY_WRITE, 
								&h);
		if (err != ERROR_SUCCESS)
		{
			throw Win::Exception ("Cannot open registry key.", 
								  ThePathMap.MakePath (keyParent.ToNative (), subKeyName).c_str (), 
								  err);
		}
		Reset (h);
		ThePathMap.Add (h, keyParent.ToNative (), subKeyName);
	}

	ReadOnly::ReadOnly (Handle keyParent, char const * subKeyName)
	{
		HKEY h;
		long err = ::RegOpenKeyEx (keyParent.ToNative (), 
								subKeyName, 
								0,
								KEY_READ, 
								&h);
		if (err != ERROR_SUCCESS)
		{
			throw Win::Exception ("Registry operation failed - cannot open read-only key.",
									ThePathMap.MakePath (keyParent.ToNative (), subKeyName).c_str (), 
									err);
		}
		Reset (h);
		ThePathMap.Add (h, keyParent.ToNative (), subKeyName);
	}

	New::New (Handle keyParent, char const * subKeyName)
	{
		HKEY h;
		unsigned long disposition;
		long err = ::RegCreateKeyEx (keyParent.ToNative (), 
									subKeyName, 
									0, 
									"",
									REG_OPTION_NON_VOLATILE, 
									KEY_READ | KEY_WRITE, 
									0,
									&h,
									&disposition);
		if (err != ERROR_SUCCESS)
		{
			throw Win::Exception ("Registry operation failed - cannot create key.",
									ThePathMap.MakePath (keyParent.ToNative (), subKeyName).c_str (), 
									err);
		}
		Reset (h);
		ThePathMap.Add (h, keyParent.ToNative (), subKeyName);
	}

	Check::Check (Handle keyParent, char const * subKeyName)
		: _exists (false)
	{
		if (!keyParent.IsNull ())
		{
			HKEY hKey;
			_exists = (::RegOpenKeyEx (keyParent.ToNative (), 
									subKeyName, 
									0, 
									KEY_READ, 
									&hKey) == ERROR_SUCCESS);
			if (_exists)
			{
				Reset (hKey);
				ThePathMap.Add (hKey, keyParent.ToNative (), subKeyName);
			}
		}
	}

	std::string System::GetProgramFilesPath ()
	{
	    ReadOnly keyWindows (_keyMicrosoft, "Windows");
		ReadOnly keyCurVer  (keyWindows,    "CurrentVersion");

		return keyCurVer.GetStringVal ("ProgramFilesDir"); 
	}
	std::string System::GetRegisteredOwner ()
	{
		SystemVersion osVer;
		bool isNT = osVer.IsOK () && osVer.IsWinNT ();

		ReadOnly keyWindows (_keyMicrosoft, isNT ? "Windows NT" : "Windows");
		ReadOnly keyCurVer  (keyWindows, "CurrentVersion");

		return keyCurVer.GetStringVal ("RegisteredOwner");
	}

	ValueSeq::ValueSeq (RegKey::Handle key)
		: _key (key.ToNative ()),
		  _cur (0xffffffff)
	{
		_status = ::RegQueryInfoKey (_key,       // handle of key to query 
									 0,          // address of buffer for class string 
									 0,          // address of size of class string buffer 
									 0,          // reserved 
									 0,          // address of buffer for number of subkeys 
									 0,          // address of buffer for longest subkey name length  
									 0,          // address of buffer for longest class string length 
									 &_count,    // address of buffer for number of value entries 
									 &_nameLen,  // address of buffer for longest value name length 
									 &_valueLen, // address of buffer for longest value data length 
									 0,          // address of buffer for security descriptor length 
									 0);         // address of buffer for last write time 
		if (_status != ERROR_SUCCESS)
			throw Win::Exception ("Registry error: Cannot get key info",
									ThePathMap.GetPath (key.ToNative ()).c_str (), 
									_status);
		_name.resize (_nameLen + 1);
		_value.resize (_valueLen + 1);
		Advance ();
	}

	void ValueSeq::Advance ()
	{
		_nameLen = _name.length ();
		_valueLen = _value.length ();
		_cur++;
		_status = ::RegEnumValue(_key,       // handle of key to query 
								 _cur,       // index of value to query 
								 &_name [0], // address of buffer for value string 
								 &_nameLen,  // address for size of value buffer 
								 0,          // reserved 
								 &_type,     // address of buffer for type code 
								 reinterpret_cast<unsigned char *>(&_value [0]),
											 // address of buffer for value data 
								 &_valueLen);// address for size of data buffer 
	}

	char const * ValueSeq::GetString () const
	{
		Assert (_type == REG_SZ);
		return _value.c_str ();
	}

	unsigned char const * ValueSeq::GetBytes () const
	{
		Assert (_type == REG_BINARY);
		return reinterpret_cast<unsigned char const *>(&_value [0]);
	}

	Seq::Seq (RegKey::Handle key)
		: _key (key.ToNative ()),
		  _cur (-1)
	{
		_status = ::RegQueryInfoKey (_key,     // handle of key to query 
									0,         // address of buffer for class string 
									0,         // address of size of class string buffer 
									0,         // reserved 
									&_count,   // address of buffer for number of subkeys 
									&_nameLen, // address of buffer for longest subkey name length  
									&_classLen,// address of buffer for longest class string length 
									0,         // address of buffer for number of value entries 
									0,         // address of buffer for longest value name length 
									0,         // address of buffer for longest value data length 
									0,         // address of buffer for security descriptor length 
									0);        // address of buffer for last write time 
		if (_status != ERROR_SUCCESS)
			throw Win::Exception ("Registry error: Cannot get key info:",
									ThePathMap.GetPath (key.ToNative ()).c_str (),
									_status);
		_name.resize (_nameLen + 1);
		_class.resize (_classLen + 1);
		Advance ();
	}

	void Seq::Advance ()
	{
		_nameLen = _name.length ();
		_classLen = _class.length ();
		_cur++;
		_status = ::RegEnumKeyEx (_key,       // handle of key to query 
								  _cur,       // index of subkey to enumerate 
								  &_name [0], // address of buffer for subkey name 
								  &_nameLen,  // address for size of subkey buffer 
								  0,          // reserved 
								  &_class [0],// address of buffer for class string 
								  &_classLen, // address for size of class buffer 
								  &_lastTime);// address for time key last written to 
	}
}
