#ifndef WINIMAGELIST_H
#define WINIMAGELIST_H

#include "Templates/RefPtr.h"
#include "Icon.h"
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <cassert>

namespace Win
{

class SimpleImageList
{
  public: // Methods
    int AddBitmap( HBITMAP hBitmap, HBITMAP hMask = 0 )
    { assert(_himl != 0);
      return ::ImageList_Add(_himl, hBitmap, hMask);
    }

    int AddMaskedBitmap( HBITMAP hBitmap, COLORREF maskColor )
    { assert(_himl != 0);
      return ::ImageList_AddMasked(_himl, hBitmap, maskColor);
    }

    void BeginDrag( int imgIdx, int xHotspot, int yHotspot ) const
    { assert(_himl != 0);
      ::ImageList_BeginDrag(_himl, imgIdx, xHotspot, yHotspot);
    }

    void CopyImage( int dstIdx, int srcIdx )
    { assert(_himl != 0);
      ::ImageList_Copy(_himl, dstIdx, _himl, srcIdx, ILCF_MOVE);
    }

    void SwapImages( int dstIdx, int srcIdx )
    { assert(_himl != 0);
      ::ImageList_Copy(_himl, dstIdx, _himl, srcIdx, ILCF_SWAP);
    }

    void Draw( int imgIdx, HDC hdc, int x, int y, UINT style = ILD_TRANSPARENT ) const
    { assert(_himl != 0);
      ::ImageList_Draw(_himl, imgIdx, hdc, x, y, style);
    }

    void DrawEx( int imgIdx, HDC hdc, int x, int y, COLORREF bkColor, COLORREF fgColor, UINT style ) const
    { assert(_himl != 0);
      ::ImageList_Draw(_himl, imgIdx, hdc, x, y, style);
    }

    void DrawIndirect( IMAGELISTDRAWPARAMS& params ) const
    { assert(_himl != 0);
      params.himl = _himl;
      ::ImageList_DrawIndirect(&params);
    }

    COLORREF GetBkColor( void ) const
    { assert(_himl != 0);
      return ::ImageList_GetBkColor(_himl);
    }

    Win::Icon GetIcon( int imgIdx, UINT flags = ILD_TRANSPARENT ) const
    { return Win::Icon(::ImageList_GetIcon(_himl, imgIdx, flags));
    }

    void GetImageSize( int& cx, int& cy ) const
    { assert(_himl != 0);
      ::ImageList_GetIconSize(_himl, &cx, &cy);
    }

    int GetImageCount( void ) const
    { assert(_himl != 0);
      return ::ImageList_GetImageCount(_himl);
    }

    void GetImageInfo( int imgIdx, IMAGEINFO& info ) const
    { assert(_himl != 0);
      ::ImageList_GetImageInfo(_himl, imgIdx, &info);
    }

    void RemoveImage( int imgIdx )
    { assert(_himl != 0);
      ::ImageList_Remove(_himl, imgIdx);
    }

    void ReplaceImage( int imgIdx, HBITMAP hBitmap, HBITMAP hMask = 0 )
    { assert(_himl != 0);
      ::ImageList_Replace(_himl, imgIdx, hBitmap, hMask);
    }

    void ReplaceIcon( int imgIdx, HICON hIcon )
    { assert(_himl != 0);
      ::ImageList_ReplaceIcon(_himl, imgIdx, hIcon);
    }

    int AddIcon( HICON hIcon )
    { assert(_himl != 0);
      return ::ImageList_ReplaceIcon(_himl, -1, hIcon);
    }

    COLORREF SetBkColor( COLORREF color )
    { assert(_himl != 0);
	return ::ImageList_SetBkColor(_himl, color);
    }

    void SetDragCursorImage( int imgIdx, int xHotspot, int yHotspot ) const
    { assert(_himl != 0);
      ::ImageList_SetDragCursorImage(_himl, imgIdx, xHotspot, yHotspot);
    }

    void SetImageSize( int cx, int cy )
    { assert(_himl != 0);
      ::ImageList_SetIconSize(_himl, cx, cy);
    }

    void SetImageCount( UINT count )
    { assert(_himl != 0);
      ::ImageList_SetImageCount(_himl, count);
    }

    void SetOverlayImage( int imgIdx, int ovlIdx )
    { assert(_himl != 0);
      ::ImageList_SetOverlayImage(_himl, imgIdx, ovlIdx);
    }

    void Write( LPSTREAM stream ) const
    { assert(_himl != 0);
      ::ImageList_Write(_himl, stream);
    }

    void Clear( void )
    { if ( _himl != 0 )
      { RemoveImage(-1);
      }
    }

    operator HIMAGELIST( void ) const
    { return _himl;
    }

  protected: // Methods
    SimpleImageList( HIMAGELIST himl = 0 )
    : _himl(himl)
    {}

  protected: // Data members
    HIMAGELIST _himl;
};

/*

 Image list representation

*/
class ImageListRep : public SimpleImageList, public RefCounted
{
  public:
    ImageListRep( HIMAGELIST himl )
    : SimpleImageList(himl)
    {}

    ImageListRep( int cx, int cy, UINT flags, int nInitial, int nGrow )
    : SimpleImageList(::ImageList_Create(cx, cy, flags, nInitial, nGrow))
    {}

    ImageListRep( ImageListRep const& b )
    { Duplicate(b._himl);
    }

    ~ImageListRep( void )
    { Destroy();
    }

    void Create( int cx, int cy, UINT flags = ILC_COLOR24, int nInitial = 10, int nGrow = 10 )
    { Destroy();
      _himl = ::ImageList_Create(cx, cy, flags, nInitial, nGrow);
    }

    void Destroy( void )
    { if ( _himl != 0 )
      { ::ImageList_Destroy(_himl);
      }
      _himl = 0;
    }

    void Duplicate( HIMAGELIST src )
    { Destroy();
      if ( src != 0 )
      { _himl = ::ImageList_Duplicate(src);
      }
    }

    void LoadBitmap( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor = CLR_NONE, int nGrow = 10, UINT flags = LR_DEFAULTCOLOR )
    { LoadImage(hInst, name, cx, maskColor, nGrow, flags, IMAGE_BITMAP);
    }

    void LoadCursor( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor = CLR_NONE, int nGrow = 10, UINT flags = LR_DEFAULTCOLOR )
    { LoadImage(hInst, name, cx, maskColor, nGrow, flags, IMAGE_CURSOR);
    }

    void LoadIcon( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor = CLR_NONE, int nGrow = 10, UINT flags = LR_DEFAULTCOLOR )
    { LoadImage(hInst, name, cx, maskColor, nGrow, flags, IMAGE_ICON);
    }

    void MergeImages( HIMAGELIST himl1, int imgIdx1, HIMAGELIST himl2, int imgIdx2, int dx = 0, int dy = 0 )
    { Destroy();
      _himl = ::ImageList_Merge(himl1, imgIdx1, himl2, imgIdx2, dx, dy);  
    }

    void Read( LPSTREAM stream )
    { Destroy();
      _himl = ::ImageList_Read(stream);
    }

  private: // Methods
    void LoadImage( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor, int nGrow, UINT flags, UINT type )
    { Destroy();
      _himl = ::ImageList_LoadImage(hInst, name, cx, nGrow, maskColor, type, flags);
    }
};

/*

  Ref counted image list

*/
class ImageList : public RefPtr<ImageListRep>
{
  public:
    ImageList( HIMAGELIST himl = 0 )
    : RefPtr<ImageListRep>(new ImageListRep(himl))
    {}

    ImageList( int cx, int cy, UINT flags = ILC_COLOR24, int nInitial = 10, int nGrow = 10 )
    : RefPtr<ImageListRep>(new ImageListRep(cx, cy, flags, nInitial, nGrow))
    {}

    ImageList( ImageList const& b )
    : RefPtr<ImageListRep>(b)
    {}

    int AddBitmap( HBITMAP hBitmap, HBITMAP hMask = 0 )
    { Clone();
      return _p->AddBitmap(hBitmap, hMask);
    }

    int AddMaskedBitmap( HBITMAP hBitmap, COLORREF maskColor )
    { Clone();
      return _p->AddMaskedBitmap(hBitmap, maskColor);
    }

    void BeginDrag( int imgIdx, int xHotspot, int yHotspot ) const
    { _p->BeginDrag(imgIdx, xHotspot, yHotspot);
    }

    void CopyImage( int dstIdx, int srcIdx )
    { Clone();
      _p->CopyImage(dstIdx, srcIdx);
    }

    void SwapImages( int imgIdx1, int imgIdx2 )
    { Clone();
      _p->SwapImages(imgIdx1, imgIdx2);
    }

    static void DragEnter( HWND hwndLock, int x, int y )
    { ::ImageList_DragEnter(hwndLock, x, y);
    }

    static void DragLeave( HWND hwndLock )
    { ::ImageList_DragLeave(hwndLock);
    }

    static void DragMove( int x, int y )
    { ::ImageList_DragMove(x, y);
    }

    static void DragShow( void )
    { ::ImageList_DragShowNolock(TRUE);
    }

    static void DragHide( void )
    { ::ImageList_DragShowNolock(FALSE);
    }

    void Draw( int imgIdx, HDC hdc, int x, int y, UINT style = ILD_TRANSPARENT ) const
    { _p->Draw(imgIdx, hdc, x, y, style);
    }

    void DrawEx( int imgIdx, HDC hdc, int x, int y, COLORREF bkColor, COLORREF fgColor, UINT style ) const
    { _p->DrawEx(imgIdx, hdc, x, y, bkColor, fgColor, style);
    }

    void DrawIndirect( IMAGELISTDRAWPARAMS& params ) const
    { _p->DrawIndirect(params);
    }

    static void EndDrag( void )
    { ::ImageList_EndDrag();
    }

    COLORREF GetBkColor( void ) const
    { return _p->GetBkColor();
    }


    Win::Icon GetIcon( int imgIdx, UINT flags = ILD_TRANSPARENT ) const
    { return _p->GetIcon(imgIdx, flags);
    }

    void GetImageSize( int& cx, int& cy ) const
    { _p->GetImageSize(cx, cy);
    }

    int GetImageCount( void ) const
    { return _p->GetImageCount();
    }

    void GetImageInfo( int imgIdx, IMAGEINFO& info ) const
    { _p->GetImageInfo(imgIdx, info);
    }

    void RemoveImage( int imgIdx )
    { Clone();
      _p->RemoveImage(imgIdx);
    }

    void ReplaceImage( int imgIdx, HBITMAP hBitmap, HBITMAP hMask = 0 )
    { Clone();
      _p->ReplaceImage(imgIdx, hBitmap, hMask);
    }

    void ReplaceIcon( int imgIdx, HICON hIcon )
    { Clone();
      _p->ReplaceIcon(imgIdx, hIcon);
    }

    int AddIcon( HICON hIcon )
    { Clone();
      return _p->AddIcon(hIcon);
    }

    COLORREF SetBkColor( COLORREF color )
    { Clone();
      return _p->SetBkColor(color);
    }

    void SetDragCursorImage( int imgIdx, int xHotspot, int yHotspot ) const
    { _p->SetDragCursorImage(imgIdx, xHotspot, yHotspot);
    }

    void SetImageSize( int cx, int cy )
    { Clone();
      _p->SetImageSize(cx, cy);
    }

    void SetImageCout( UINT count )
    { Clone();
      _p->SetImageCount(count);
    }

    void SetOverlayImage( int imgIdx, int ovlIdx )
    { Clone();
      _p->SetOverlayImage(imgIdx, ovlIdx);
    }

    void Write( LPSTREAM stream ) const
    { _p->Write(stream);
    }

    void Create( int cx, int cy, UINT flags = ILC_COLOR24, int nInitial = 10, int nGrow = 10 )
    { Clone();
      _p->Create(cx, cy, flags, nInitial, nGrow);
    }

    void Destroy( void )
    { Clone();
      _p->Destroy();
    }

    void Duplicate( HIMAGELIST src )
    { Clone();
      _p->Duplicate(src);
    }

    void LoadBitmap( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor = CLR_NONE, int nGrow = 10, UINT flags = LR_DEFAULTCOLOR )
    { Clone();
      _p->LoadBitmap(hInst, name, cx, maskColor, nGrow, flags);
    }

    void LoadCursor( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor = CLR_NONE, int nGrow = 10, UINT flags = LR_DEFAULTCOLOR )
    { Clone();
      _p->LoadCursor(hInst, name, cx, maskColor, nGrow, flags);
    }

    void LoadIcon( HINSTANCE hInst, TCHAR const name[], int cx, COLORREF maskColor = CLR_NONE, int nGrow = 10, UINT flags = LR_DEFAULTCOLOR )
    { Clone();
      _p->LoadIcon(hInst, name, cx, maskColor, nGrow, flags);
    }

    void MergeImages( HIMAGELIST himl1, int imgIdx1, HIMAGELIST himl2, int imgIdx2, int dx = 0, int dy = 0 )
    { Clone();
      _p->MergeImages(himl1, imgIdx1, himl2, imgIdx2, dx, dy);
    }

    void Read( LPSTREAM stream )
    { Clone();
      _p->Read(stream);
    }

    void Clear( void )
    { Clone();
      _p->Clear();
    }

    operator HIMAGELIST( void ) const
    { return static_cast<HIMAGELIST>(*_p);
    }

  private:
    void Clone( void )
    { if ( _p->GetRefCount() > 1 )
      { // clone it
        ImageListRep* rep = new ImageListRep(*_p);
        Release();
        _p = rep;
      }
    }
};


/*

 Image list is not owned - for example system image list returned
 by ::SHGetFileInfo function

*/
class StrangerImageList : public SimpleImageList
{
  public:
    StrangerImageList( HIMAGELIST himl = 0 )
    : SimpleImageList(himl)
    {}

    void Init( HIMAGELIST himl )
    { _himl = himl;
    }

    void GetDragImage( POINT* pt = 0, POINT* ptHotspot = 0 )
    { _himl = ::ImageList_GetDragImage(pt, ptHotspot);
    }
};

/*

  COM must be initialized before creating this object

*/
class SystemShellImageList : public StrangerImageList
{
  public:
    SystemShellImageList( void )
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      _himl = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(TEXT(""), 0, &si, sizeof(si), SHGFI_SYSICONINDEX|SHGFI_SHELLICONSIZE));
      if ( si.hIcon != 0 )
      { ::DestroyIcon(si.hIcon);
      }
    }
};

/*

  COM must be initialized before creating this object

*/
class SystemSmallImageList : public StrangerImageList
{
  public:
    SystemSmallImageList( void )
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      _himl = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(TEXT(""), 0, &si, sizeof(si), SHGFI_SYSICONINDEX|SHGFI_SMALLICON));
      if ( si.hIcon != 0 )
      { ::DestroyIcon(si.hIcon);
      }
    }
};

/*

  COM must be initialized before creating this object

*/
class SystemLargeImageList : public StrangerImageList
{
  public:
    SystemLargeImageList( void )
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      _himl = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(TEXT(""), 0, &si, sizeof(si), SHGFI_SYSICONINDEX|SHGFI_LARGEICON));
      if ( si.hIcon != 0 )
      { ::DestroyIcon(si.hIcon);
      }
    }
};

} // namespace Win

#endif // WINIMAGELIST_H