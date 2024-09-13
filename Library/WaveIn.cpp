#include "WaveIn.h"
#include <tchar.h>

using namespace Win;

XWaveIn::XWaveIn( MMRESULT code, TCHAR const msg[] )
{
  _msg = stdstring(msg);
  TCHAR buf[MAXERRORLENGTH] = _T("");
  MMRESULT res = ::waveInGetErrorText(code, buf, MAXERRORLENGTH);
  if ( res == MMSYSERR_NOERROR )
  { _msg += _T(" (");
    _msg += buf;
    _msg += _T(")");
  }
  else
  { _msg += _T(" (and ::waveInGetErrorText failed too)");
  }
}