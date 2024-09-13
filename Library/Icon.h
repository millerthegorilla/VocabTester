#ifndef WINICON_H
#define WINICON_H

#include "Templates/RefPtr.h"
#include <windows.h>
#include <cassert>

namespace Win
{

class SimpleIcon
{
  public: // Methods
    void Draw( HDC hdc, int x, int y ) const
    { assert(_hIcon != 0);
      ::DrawIcon(hdc, x, y, _hIcon);
    }

    void GetIconInfo( ICONINFO& info ) const
    { assert(_hIcon != 0);
      ::GetIconInfo(_hIcon, &info);
    }

    operator HICON( void ) const
    { return _hIcon;
    }

  protected: // Methods
    SimpleIcon( HICON hIcon = 0 )
    : _hIcon(hIcon)
    {}

  protected: // Data members
    HICON _hIcon;
};

/*

  Icon representation

*/
class IconRep : public SimpleIcon, public RefCounted
{
  public:
    IconRep( HICON hIcon = 0 )
    : SimpleIcon(hIcon)
    {}

    IconRep( IconRep const& b )
    { Duplicate(b._hIcon);
    }

    ~IconRep( void )
    { Destroy();
    }

    void Destroy( void )
    { if ( _hIcon != 0 )
      { ::DestroyIcon(_hIcon);
      }
      _hIcon = 0;
    }

    void Duplicate( HICON src )
    { Destroy();
      if ( src != 0 )
      { _hIcon = ::DuplicateIcon(0, src);
      }
    }

    void Create( HINSTANCE hInst, int w, int h, int nPlanes, int nBitsPerPixel, unsigned char const andBits[], unsigned char const xorBits[] )
    { Destroy();
      _hIcon = ::CreateIcon(hInst, w, h, nPlanes, nBitsPerPixel, andBits, xorBits);
    }

    void CreateIndirect( HBITMAP hBitmap, HBITMAP hMask )
    { Destroy();
      ICONINFO info = {TRUE, 0, 0, hMask, hBitmap};
      _hIcon = ::CreateIconIndirect(&info);
    }

    void Load( HINSTANCE hInst, int resId, int cx, int cy, UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _hIcon = reinterpret_cast<HICON>(::LoadImage(hInst, MAKEINTRESOURCE(resId), IMAGE_ICON, cx, cy, flags));
    }

    void Load( HINSTANCE hInst, TCHAR const name[], int cx, int cy, UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _hIcon = reinterpret_cast<HICON>(::LoadImage(hInst, name, IMAGE_ICON, cx, cy, flags));
    }
};

/*

  Small icon representation

*/
class SmallIconRep : public IconRep
{
  public:
    SmallIconRep( HICON hIcon = 0 )
    : IconRep(hIcon)
    {}

    SmallIconRep( SmallIconRep const& b )
    { Duplicate(b._hIcon);
    }

    void Extract( TCHAR const fileName[], int iconIdx )
    { assert(iconIdx >= 0);
      Destroy();
      ::ExtractIconEx(fileName, iconIdx, 0, &_hIcon, 1);     
    }

    void Create( HINSTANCE hInst, int nPlanes, int nBitsPerPixel, unsigned char const andBits[], unsigned char const xorBits[] )
    { IconRep::Create(hInst, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), nPlanes, nBitsPerPixel, andBits, xorBits);
    }

    void Load( HINSTANCE hInst, int resId, UINT flags = LR_DEFAULTCOLOR )
    { IconRep::Load(hInst, resId, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), flags);
    }

    void Load( HINSTANCE hInst, TCHAR const name[], UINT flags = LR_DEFAULTCOLOR )
    { IconRep::Load(hInst, name, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), flags);
    }
};


/*

  Large icon representation

*/
class LargeIconRep : public IconRep
{
  public:
    LargeIconRep( HICON hIcon = 0 )
    : IconRep(hIcon)
    {}

    LargeIconRep( LargeIconRep const& b )
    { Duplicate(b._hIcon);
    }

    void Extract( TCHAR const fileName[], int iconIdx )
    { assert(iconIdx >= 0);
      Destroy();
      ::ExtractIconEx(fileName, iconIdx, &_hIcon, 0, 1);     
    }

    void Create( HINSTANCE hInst, int nPlanes, int nBitsPerPixel, unsigned char const andBits[], unsigned char const xorBits[] )
    { IconRep::Create(hInst, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), nPlanes, nBitsPerPixel, andBits, xorBits);
    }

    void Load( HINSTANCE hInst, int resId, UINT flags = LR_DEFAULTCOLOR )
    { IconRep::Load(hInst, resId, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), flags);
    }

    void Load( HINSTANCE hInst, TCHAR const name[], UINT flags = LR_DEFAULTCOLOR )
    { IconRep::Load(hInst, name, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), flags);
    }
};

/*

  Ref counted icon

*/
class Icon : public RefPtr<IconRep>
{
  public:
    Icon( HICON hIcon = 0 )
    : RefPtr<IconRep>(new IconRep(hIcon))
    {}

    Icon( Icon const& b )
    : RefPtr<IconRep>(b)
    {}

    void Draw( HDC hdc, int x, int y ) const
    { _p->Draw(hdc, x, y);
    }

    void GetIconInfo( ICONINFO& info ) const
    { _p->GetIconInfo(info);
    }

    operator HICON( void ) const
    { return static_cast<HICON>(*_p);
    }

    void Destroy( void )
    { Release();
      _p = new IconRep(0);
    }

    void Duplicate( HICON src )
    { Destroy();
      _p->Duplicate(src);
    }

    void Create( HINSTANCE hInst, int w, int h, int nPlanes, int nBitsPerPixel, unsigned char const andBits[], unsigned char const xorBits[] )
    { Destroy();
      _p->Create(hInst, w, h, nPlanes, nBitsPerPixel, andBits, xorBits);
    }

    void CreateIndirect( HBITMAP hBitmap, HBITMAP hMask )
    { Destroy();
      _p->CreateIndirect(hBitmap, hMask);
    }

    static int GetIconCount( TCHAR const fileName[] )
    { return ::ExtractIconEx(fileName, -1, 0, 0, 0);
    }

    void Load( HINSTANCE hInst, int resId, int cx, int cy, UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _p->Load(hInst, resId, cx, cy, flags);
    }

    void Load( HINSTANCE hInst, TCHAR const name[], int cx, int cy, UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _p->Load(hInst, name, cx, cy, flags);
    }
};

/*

  Ref counted small icon

*/
class SmallIcon : public RefPtr<SmallIconRep>
{
  public:
    SmallIcon( HICON hIcon = 0 )
    : RefPtr<SmallIconRep>(new SmallIconRep(hIcon))
    {}

    SmallIcon( SmallIcon const& b )
    : RefPtr<SmallIconRep>(b)
    {}

    void Draw( HDC hdc, int x, int y ) const
    { _p->Draw(hdc, x, y);
    }

    static int GetIconCount( TCHAR const fileName[] )
    { ::ExtractIconEx(fileName, -1, 0, 0, 0);
    }

    void GetIconInfo( ICONINFO& info ) const
    { _p->GetIconInfo(info);
    }

    operator HICON( void ) const
    { return static_cast<HICON>(*_p);
    }

    void Destroy( void )
    { Release();
      _p = new SmallIconRep(0);
    }

    void Duplicate( HICON src )
    { Destroy();
      _p->Duplicate(src);
    }

    void Create( HINSTANCE hInst, int nPlanes, int nBitsPerPixel, unsigned char const andBits[], unsigned char const xorBits[] )
    { Destroy();
      _p->Create(hInst, nPlanes, nBitsPerPixel, andBits, xorBits);
    }

    void CreateIndirect( HBITMAP hBitmap, HBITMAP hMask )
    { Destroy();
      _p->CreateIndirect(hBitmap, hMask);
    }

    void Extract( TCHAR const fileName[], int iconIdx )
    { Destroy();
      _p->Extract(fileName, iconIdx);
    }

    void Load( HINSTANCE hInst, int resId, UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _p->Load(hInst, resId, flags);
    }

    void Load( HINSTANCE hInst, TCHAR const name[], UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _p->Load(hInst, name, flags);
    }
};

/*

  Ref counted large icon

*/
class LargeIcon : public RefPtr<LargeIconRep>
{
  public:
    LargeIcon( HICON hIcon = 0 )
    : RefPtr<LargeIconRep>(new LargeIconRep(hIcon))
    {}

    LargeIcon( LargeIcon const& b )
    : RefPtr<LargeIconRep>(b)
    {}

    void Draw( HDC hdc, int x, int y ) const
    { _p->Draw(hdc, x, y);
    }

    static int GetIconCount( TCHAR const fileName[] )
    { ::ExtractIconEx(fileName, -1, 0, 0, 0);
    }

    void GetIconInfo( ICONINFO& info ) const
    { _p->GetIconInfo(info);
    }

    operator HICON( void ) const
    { return static_cast<HICON>(*_p);
    }

    void Destroy( void )
    { Release();
      _p = new LargeIconRep(0);
    }

    void Duplicate( HICON src )
    { Destroy();
      _p->Duplicate(src);
    }

    void Create( HINSTANCE hInst, int nPlanes, int nBitsPerPixel, unsigned char const andBits[], unsigned char const xorBits[] )
    { Destroy();
      _p->Create(hInst, nPlanes, nBitsPerPixel, andBits, xorBits);
    }

    void CreateIndirect( HBITMAP hBitmap, HBITMAP hMask )
    { Destroy();
      _p->CreateIndirect(hBitmap, hMask);
    }

    void Extract( TCHAR const fileName[], int iconIdx )
    { Destroy();
      _p->Extract(fileName, iconIdx);
    }

    void Load( HINSTANCE hInst, int resId, UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _p->Load(hInst, resId, flags);
    }

    void Load( HINSTANCE hInst, TCHAR const name[], UINT flags = LR_DEFAULTCOLOR )
    { Destroy();
      _p->Load(hInst, name, flags);
    }
};


/*

  Non owned icon

*/
class StrangerIcon : public SimpleIcon
{
  public:
    StrangerIcon( HICON hIcon = 0 )
    : SimpleIcon(hIcon)
    {}

    void Init( HICON hIcon )
    { _hIcon = hIcon;
    }
};

class EnumIcons
{
  public:
    EnumIcons( TCHAR const fileName[] )
    : _fileName(fileName),
      _nIcons(Icon::GetIconCount(_fileName)),
      _curIcon(0)
    {}

    bool AtEnd( void ) const
    { return _curIcon >= _nIcons;
    }

    void Advance( void )
    { ++_curIcon;
    }

    SmallIcon GetSmallIcon( void )
    { SmallIcon icon;
      icon.Extract(_fileName, _curIcon);
      return icon;
    }

    LargeIcon GetLargeIcon( void )
    { LargeIcon icon;
      icon.Extract(_fileName, _curIcon);
      return icon;
    }

  private:
    TCHAR const* _fileName;
    int          _nIcons;
    int          _curIcon;
};

} // namespace Win

#endif // WINICON_H