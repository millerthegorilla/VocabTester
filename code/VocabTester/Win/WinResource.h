#if !defined (WINRESOURCE_H)
#define WINRESOURCE_H
//---------------------------
// (c) Reliable Software 2005
//---------------------------
class Resource
{
public:
	Resource (Win::Dow::Handle owner, int resId, char const * type);

	bool IsOk () const { return _hRes != 0 && _hGlob !=0; }
	template<class T>
	T * Lock ();

private:
	HRSRC	_hRes;
	HGLOBAL	_hGlob;
};

template<class T>
T * Resource::Lock ()
{
	return static_cast<T *> (::LockResource (_hGlob));
}
#endif
