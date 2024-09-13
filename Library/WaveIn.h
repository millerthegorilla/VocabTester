#ifndef WINWAVEIN_H
#define WINWAVEIN_H

#include <cassert>
#include "XWin.h"
#include <mmsystem.h>

namespace Win
{

class XWaveIn : public Exception
{
  public:
    XWaveIn( MMRESULT res, TCHAR const msg[] );
};

class WaveIn
{
  public: // Enums
    enum OpenResult
    {
      ALLOK,
      INUSE,
      NODEVICE,
    };

  public: // Methods
    WaveIn( void )
    : _h(0)
    {}
    
    ~WaveIn( void )
    { Close(); 
    }

    static bool DeviceInstalled( void ) { return ::waveInGetNumDevs() > 0; }

    OpenResult Open( UINT devId, WAVEFORMATEX& format, HANDLE hEvent ) throw (XWaveIn)
    { Close();
      MMRESULT res = ::waveInOpen(&_h, devId, &format, reinterpret_cast<DWORD>(hEvent), 0, CALLBACK_EVENT);
      switch ( res )
      { case MMSYSERR_NOERROR: return ALLOK;
        case MMSYSERR_ALLOCATED: return INUSE;
        case MMSYSERR_NODRIVER: return NODEVICE;
        default: throw XWaveIn(res, TEXT("::waveInOpen failed"));
      }
    }
    
    OpenResult Open( UINT devId, WAVEFORMATEX& format, void (CALLBACK *waveInProc)( HWAVEIN, UINT, DWORD, DWORD, DWORD ), DWORD userData = 0 ) throw (XWaveIn)
    { Close();
      MMRESULT res = ::waveInOpen(&_h, devId, &format, reinterpret_cast<DWORD>(waveInProc), userData, CALLBACK_FUNCTION);
      switch ( res )
      { case MMSYSERR_NOERROR: return ALLOK;
        case MMSYSERR_ALLOCATED: return INUSE;
        case MMSYSERR_NODRIVER: return NODEVICE;
        default: throw XWaveIn(res, TEXT("::waveInOpen failed"));
      }
    }
    
    void AddBuffer( WAVEHDR& hdr ) throw (XWaveIn)
    { assert(_h != 0);
      MMRESULT res = ::waveInAddBuffer(_h, &hdr, sizeof(hdr));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveIn(res, TEXT("::waveInAddBuffer failed"));
      }
    }
    
    void Close( void ) throw ()
    { if (_h != 0) ::waveInClose(_h);
      _h = 0;
    }
    
    void Prepare( WAVEHDR& hdr ) throw (XWaveIn)
    { assert(_h != 0);
      MMRESULT res = ::waveInPrepareHeader(_h, &hdr, sizeof(hdr));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveIn(res, TEXT("::waveInPrepareHeader failed"));
      }
    }
    
    void Unprepare( WAVEHDR& hdr ) throw (XWaveIn)
    { assert(_h != 0);
      MMRESULT res = ::waveInUnprepareHeader(_h, &hdr, sizeof(hdr));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveIn(res, TEXT("::waveInUnprepareHeader failed"));
      }
    }

    void Start( void ) throw (XWaveIn)
    { assert(_h != 0);
      MMRESULT res = ::waveInStart(_h);
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveIn(res, TEXT("::waveInStart failed"));
      }
    }

    void Stop( void ) throw ()
    { assert(_h != 0);
      ::waveInStop(_h);
    }
    
    void Reset( void ) throw ()
    { assert(_h != 0);
      ::waveInReset(_h);
    }
    
    bool IsOk( void ) const throw () { return _h != 0; }
    
  private: // Data members
    HWAVEIN _h;
};

} // namespace Win

#endif // WINMEDIAEXCEPTION_H