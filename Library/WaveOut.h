#ifndef WINWAVEOUT_H
#define WINWAVEOUT_H

#include <cassert>
#include "XWin.h"
#include <mmsystem.h>

namespace Win
{

class XWaveOut : public Exception
{
  public:
    XWaveOut( MMRESULT res, TCHAR const msg[] );
};

class WaveOut
{
  public: // Enums
    enum OpenResult
    {
      ALLOK,
      INUSE,
      NODEVICE,
    };

  public: // Methods
    WaveOut( void )
    : _h(0)
    {}
    
    ~WaveOut( void )
    { Close(); 
    }

    static bool DeviceInstalled( void ) { return ::waveOutGetNumDevs() > 0; }

    OpenResult Open( WAVEFORMATEX& format, HANDLE hEvent ) throw (XWaveOut)
    { Close();
      MMRESULT res = ::waveOutOpen(&_h, WAVE_MAPPER, &format, reinterpret_cast<DWORD>(hEvent), 0, CALLBACK_EVENT);
      switch ( res )
      { case MMSYSERR_NOERROR: return ALLOK;
        case MMSYSERR_ALLOCATED: return INUSE;
        case MMSYSERR_NODRIVER: return NODEVICE;
        default: throw XWaveOut(res, TEXT("::waveOutOpen failed"));
      }
    }
    
    OpenResult Open( WAVEFORMATEX& format, void (CALLBACK *waveOutProc)( HWAVEOUT, UINT, DWORD, DWORD, DWORD ), DWORD userData = 0 ) throw (XWaveOut)
    { Close();
      MMRESULT res = ::waveOutOpen(&_h, WAVE_MAPPER, &format, reinterpret_cast<DWORD>(waveOutProc), userData, CALLBACK_FUNCTION);
      switch ( res )
      { case MMSYSERR_NOERROR: return ALLOK;
        case MMSYSERR_ALLOCATED: return INUSE;
        case MMSYSERR_NODRIVER: return NODEVICE;
        default: throw XWaveOut(res, TEXT("::waveOutOpen failed"));
      }
    }
    
    void Write( WAVEHDR& hdr ) throw (XWaveOut)
    { assert(_h != 0);
      MMRESULT res = ::waveOutWrite(_h, &hdr, sizeof(hdr));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutWrite failed"));
      }
    }
    
    void Close( void ) throw ()
    { if (_h != 0)
      { ::waveOutClose(_h);
      }
      _h = 0;
    }
    
    void Prepare( WAVEHDR& hdr ) throw (XWaveOut)
    { assert(_h != 0);
      MMRESULT res = ::waveOutPrepareHeader(_h, &hdr, sizeof(hdr));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutPrepareHeader failed"));
      }
    }
    
    void Unprepare( WAVEHDR& hdr ) throw (XWaveOut)
    { assert(_h != 0);
      MMRESULT res = ::waveOutUnprepareHeader(_h, &hdr, sizeof(hdr));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutUnprepareHeader failed"));
      }
    }
    
    void Pause( void ) throw (XWaveOut)
    { assert(_h != 0);
      MMRESULT res = ::waveOutPause(_h);
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutPause failed"));
      }
    }
    
    void Resume( void ) throw (XWaveOut)
    { assert(_h != 0);
      MMRESULT res = ::waveOutRestart(_h);
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutRestart failed"));
      }
    }
    
    void Reset( void ) throw ()
    { assert(_h != 0);
      ::waveOutReset(_h);
    }
    
    void SetVolume( WORD volume ) throw (XWaveOut)
    { MMRESULT res = ::waveOutSetVolume(reinterpret_cast<HWAVEOUT>(WAVE_MAPPER), MAKELONG(volume,volume));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutSetVolume failed"));
      }
    }
    
    void SetVolume( WORD left, WORD right )
    { MMRESULT res = ::waveOutSetVolume(reinterpret_cast<HWAVEOUT>(WAVE_MAPPER), MAKELONG(left,right));
      if ( res != MMSYSERR_NOERROR ) 
      { throw XWaveOut(res, TEXT("::waveOutSetVolume failed"));
      }
    }
    
    void GetVolume( WORD& left, WORD& right ) throw (XWaveOut)
    { DWORD vol = 0;
      MMRESULT res = ::waveOutGetVolume(reinterpret_cast<HWAVEOUT>(WAVE_MAPPER), &vol);
      if ( res != MMSYSERR_NOERROR )
      { throw XWaveOut(res, TEXT("::waveOutGetVolume failed"));
      }
      left = LOWORD(vol); right = HIWORD(vol);
    }
    
    bool IsOk( void ) const throw () { return _h != 0; }
    
  private: // Data members
    HWAVEOUT _h;
};

} // namespace Win

#endif // WINMEDIAEXCEPTION_H