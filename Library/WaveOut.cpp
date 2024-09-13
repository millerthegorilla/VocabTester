#include "WaveOut.h"
#include <tchar.h>

using namespace Win;

XWaveOut::XWaveOut( MMRESULT code, TCHAR const msg[] )
{
  _msg = stdstring(msg);
  TCHAR buf[MAXERRORLENGTH] = _T("");
  MMRESULT res = ::waveOutGetErrorText(code, buf, MAXERRORLENGTH);
  if ( res == MMSYSERR_NOERROR )
  { _msg += _T(" (");
    _msg += buf;
    _msg += _T(")");
  }
  else
  { _msg += _T(" (and ::waveOutGetErrorText failed too)");
  }
}