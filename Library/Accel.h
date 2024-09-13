#ifndef WINACCELERATOR_H
#define WINACCELERATOR_H

#include "Templates/SmartPtr.h"
#include "Templates/RefPtr.h"
#include "XWin.h"

namespace Win
{

class AccelRep : public RefCounted
{
  public:
    AccelRep( void )
    : _hAccel(0),
      _destroy(false)
    {}

    AccelRep( HINSTANCE hInst, int resId )
    : _hAccel(0),
      _destroy(false)
    { Load(hInst, resId);
    }

    AccelRep( ACCEL const accel[], int nEntries )
    : _hAccel(0),
      _destroy(false)
    { Create(accel, nEntries);
    }

    AccelRep( HACCEL b )
    : _hAccel(0),
      _destroy(false)
    { Duplicate(b);
    }
    
    ~AccelRep( void )
    { Destroy();
    }

    void Load( HINSTANCE hInst, int resId )
    { Destroy();
      _hAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(resId));
      if ( _hAccel == 0 )
      { throw Win::Exception(_T("AccelRep::Load: cannot load accelerator table"));
      }
      _destroy = false;
    }

    void Create( ACCEL const accel[], int nEntries )
    { Destroy();
      _hAccel = ::CreateAcceleratorTable(const_cast<ACCEL*>(accel), nEntries);
      if ( _hAccel == 0 )
      { throw Win::Exception(_T("AccelRep::Create: cannot create accelerator table"));
      }
      _destroy = true;
    }

    void Duplicate( HACCEL hAccel )
    { if ( hAccel != 0 )
      { int nEntries = ::CopyAcceleratorTable(hAccel, 0, 0);
        if ( nEntries > 0 )
        { SArr<ACCEL> accel(new ACCEL[nEntries]);
          if ( ::CopyAcceleratorTable(hAccel, accel, nEntries) != nEntries )
          { throw Win::Exception(_T("AccelRep::Duplicate: cannot copy accelerator table data"));
          }
          Create(accel, nEntries);
        }
        else
        { Destroy();
        }
      }
      else
      { Destroy();
      }
    }

    void Destroy( void )
    { if ( _hAccel != 0 && _destroy )
      { ::DestroyAcceleratorTable(_hAccel);
      }
      _hAccel = 0;
      _destroy = false;
    }

    bool IsOk( void ) const { return _hAccel != 0; }
    operator HACCEL( void ) const { return _hAccel; }

  private:
    HACCEL _hAccel;
    bool   _destroy;
};

class Accel : public RefPtr<AccelRep>
{
  public:
    Accel( void )
    : RefPtr<AccelRep>(new AccelRep)
    {}

    Accel( HINSTANCE hInst, int resId )
    : RefPtr<AccelRep>(new AccelRep(hInst, resId))
    {}

    Accel( ACCEL const accel[], int nEntries )
    : RefPtr<AccelRep>(new AccelRep(accel, nEntries))
    {}

    Accel( Accel const& b )
    : RefPtr<AccelRep>(b)
    {}

    void Load( HINSTANCE hInst, int resId )
    { Destroy();
      _p->Load(hInst, resId);
    }

    void Create( ACCEL const accel[], int nEntries )
    { Destroy();
      _p->Create(accel, nEntries);
    }

    void Duplicate( HACCEL hAccel )
    { Destroy();
      _p->Duplicate(hAccel);
    }

    void Destroy( void )
    { Release();
      _p = new AccelRep;
    }

    bool IsOk( void ) const { return _p->IsOk(); }
    operator HACCEL( void ) const { return static_cast<HACCEL>(*_p); }

  public: // Class
    class EnumEntries
    {
      public:
        EnumEntries( HACCEL hAccel )
        : _nEntries(hAccel == 0 ? 0 : ::CopyAcceleratorTable(hAccel, 0, 0)),
          _entries(_nEntries == 0 ? 0 : new ACCEL[_nEntries]),
          _cur(0)
        { if ( _nEntries > 0 )
          { ::CopyAcceleratorTable(hAccel, _entries, _nEntries);
          }
        }

        bool AtEnd( void ) const { return _cur >= _nEntries; }
        void Advance( void ) { ++_cur; }
        ACCEL const& GetEntry( void ) const { return _entries[_cur]; }

      private: // Data members
        int         _nEntries;
        SArr<ACCEL> _entries;
        int         _cur;
    };
};

} // namespace Win

#endif // WINACCELERATOR_H