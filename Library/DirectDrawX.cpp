#include "DirectDraw.h"

namespace Win
{
namespace Direct
{

XDraw::XDraw( HRESULT hr, TCHAR const msg[] )
{
  _msg = stdstring(msg);
  if ( hr == DD_OK ) return; // So what the hell is going on?
  
  _msg += _T(" (");
  switch ( hr )
  {
    case DDERR_ALREADYINITIALIZED:
      _msg += _T("DDERR_ALREADYINITIALIZED: The object has already been initialized");
      break;

    case DDERR_BLTFASTCANTCLIP:
      _msg += _T("DDERR_BLTFASTCANTCLIP: A DirectDrawClipper object is attached to a source surface that has passed into a call to the IDirectDrawSurface7::BltFast method");
      break;
    
    case DDERR_CANNOTATTACHSURFACE:
      _msg += _T("DDERR_CANNOTATTACHSURFACE: A surface cannot be attached to another requested surface");
      break;
    
    case DDERR_CANNOTDETACHSURFACE:
      _msg += _T("DDERR_CANNOTDETACHSURFACE: A surface cannot be detached from another requested surface");
      break;
    
    case DDERR_CANTCREATEDC:
      _msg += _T("DDERR_CANTCREATEDC: Windows cannot create any more device contexts (DCs), or a DC has requested a palette-indexed surface when the surface had no palette and the display mode was not palette-indexed (in this case DirectDraw cannot select a proper palette into the DC)");
      break;

    case DDERR_CANTDUPLICATE:
      _msg += _T("DDERR_CANTDUPLICATE: Primary and 3-D surfaces, or surfaces that are implicitly created, cannot be duplicated");
      break;

    case DDERR_CANTLOCKSURFACE:
      _msg += _T("DDERR_CANTLOCKSURFACE: Access to this surface is refused because an attempt was made to lock the primary surface without DCI support");
      break;

    case DDERR_CANTPAGELOCK:
      _msg += _T("DDERR_CANTPAGELOCK: An attempt to page-lock a surface failed. Page lock does not work on a display-memory surface or an emulated primary surface");
      break;

    case DDERR_CANTPAGEUNLOCK:
      _msg += _T("DDERR_CANTPAGEUNLOCK: An attempt to page-unlock a surface failed. Page unlock does not work on a display-memory surface or an emulated primary surface");
      break;

    case DDERR_CLIPPERISUSINGHWND:
      _msg += _T("DDERR_CLIPPERISUSINGHWND: An attempt was made to set a clip list for a DirectDrawClipper object that is already monitoring a window handle");
      break;

    case DDERR_COLORKEYNOTSET:
      _msg += _T("DDERR_COLORKEYNOTSET: No source color key is specified for this operation");
      break;

    case DDERR_CURRENTLYNOTAVAIL:
      _msg += _T("DDERR_CURRENTLYNOTAVAIL: No support is currently available");
      break;

    case DDERR_DDSCAPSCOMPLEXREQUIRED:
      _msg += _T("DDERR_DDSCAPSCOMPLEXREQUIRED: The surface requires the DDSCAPS_COMPLEX flag");
      break;

    case DDERR_DCALREADYCREATED:
      _msg += _T("DDERR_DCALREADYCREATED: A device context (DC) has already been returned for this surface. Only one DC can be retrieved for each surface");
      break;

    case DDERR_DEVICEDOESNTOWNSURFACE:
      _msg += _T("DDERR_DEVICEDOESNTOWNSURFACE: Surfaces created by one DirectDraw device cannot be used directly by another DirectDraw device");
      break;

    case DDERR_DIRECTDRAWALREADYCREATED:
      _msg += _T("DDERR_DIRECTDRAWALREADYCREATED: A DirectDraw object representing this driver has already been created for this process");
      break;

    case DDERR_EXCEPTION:
      _msg += _T("DDERR_EXCEPTION: An exception was encountered while performing the requested operation");
      break;

    case DDERR_EXCLUSIVEMODEALREADYSET:
      _msg += _T("DDERR_EXCLUSIVEMODEALREADYSET: An attempt was made to set the cooperative level when it was already set to exclusive");
      break;

    case DDERR_EXPIRED:
      _msg += _T("DDERR_EXPIRED: The data has expired and is therefore no longer valid");
      break;
    
    case DDERR_GENERIC:
      _msg += _T("DDERR_GENERIC: There is an undefined error condition");
      break;

    case DDERR_HEIGHTALIGN:
      _msg += _T("DDERR_HEIGHTALIGN: The height of the provided rectangle is not a multiple of the required alignment");
      break;

    case DDERR_HWNDALREADYSET:
      _msg += _T("DDERR_HWNDALREADYSET: The DirectDraw cooperative-level window handle has already been set. It cannot be reset while the process has surfaces or palettes created");
      break;

    case DDERR_HWNDSUBCLASSED:
      _msg += _T("DDERR_HWNDSUBCLASSED: DirectDraw is prevented from restoring state because the DirectDraw cooperative-level window handle has been subclassed");
      break;

    case DDERR_IMPLICITLYCREATED:
      _msg += _T("DDERR_IMPLICITLYCREATED: The surface cannot be restored because it is an implicitly created surface");
      break;

    case DDERR_INCOMPATIBLEPRIMARY:
      _msg += _T("DDERR_INCOMPATIBLEPRIMARY: The primary surface creation request does not match the existing primary surface");
      break;

    case DDERR_INVALIDCAPS:
      _msg += _T("DDERR_INVALIDCAPS: One or more of the capability bits passed to the callback function are incorrect");
      break;

    case DDERR_INVALIDCLIPLIST:
      _msg += _T("DDERR_INVALIDCLIPLIST: DirectDraw does not support the provided clip list");
      break;

    case DDERR_INVALIDDIRECTDRAWGUID:
      _msg += _T("DDERR_INVALIDDIRECTDRAWGUID: The globally unique identifier (GUID) passed to the DirectDrawCreate function is not a valid DirectDraw driver identifier");
      break;

    case DDERR_INVALIDMODE:
      _msg += _T("DDERR_INVALIDMODE: DirectDraw does not support the requested mode");
      break;

    case DDERR_INVALIDOBJECT:
      _msg += _T("DDERR_INVALIDOBJECT: DirectDraw received a pointer that was an invalid DirectDraw object");
      break;

    case DDERR_INVALIDPARAMS:
      _msg += _T("DDERR_INVALIDPARAMS: One or more of the parameters passed to the method are incorrect");
      break;

    case DDERR_INVALIDPIXELFORMAT:
      _msg += _T("DDERR_INVALIDPIXELFORMAT: The pixel format was invalid as specified");
      break;

    case DDERR_INVALIDPOSITION:
      _msg += _T("DDERR_INVALIDPOSITION: The position of the overlay on the destination is no longer legal");
      break;

    case DDERR_INVALIDRECT:
      _msg += _T("DDERR_INVALIDRECT: The provided rectangle was invalid");
      break;

    case DDERR_INVALIDSTREAM:
      _msg += _T("DDERR_INVALIDSTREAM: The specified stream contains invalid data");
      break;

    case DDERR_INVALIDSURFACETYPE:
      _msg += _T("DDERR_INVALIDSURFACETYPE: The surface was of the wrong type");
      break;

    case DDERR_LOCKEDSURFACES:
      _msg += _T("DDERR_LOCKEDSURFACES: One or more surfaces are locked, causing the failure of the requested operation");
      break;

    case DDERR_MOREDATA:
      _msg += _T("DDERR_MOREDATA: There is more data available than the specified buffer size can hold");
      break;

    case DDERR_NEWMODE:
      _msg += _T("DDERR_NEWMODE: When IDirectDraw7::StartModeTest is called with the DDSMT_ISTESTREQUIRED flag, it may return this value to denote that some or all of the resolutions can and should be tested. IDirectDraw7::EvaluateMode returns this value to indicate that the test has switched to a new display mode");
      break;

    case DDERR_NO3D:
      _msg += _T("DDERR_NO3D: No 3-D hardware or emulation is present");
      break;

    case DDERR_NOALPHAHW:
      _msg += _T("DDERR_NOALPHAHW: No alpha-acceleration hardware is present or available, causing the failure of the requested operation");
      break;

    case DDERR_NOBLTHW:
      _msg += _T("DDERR_NOBLTHW: No blitter hardware is present");
      break;

    case DDERR_NOCLIPLIST:
      _msg += _T("DDERR_NOCLIPLIST: No clip list is available");
      break;

    case DDERR_NOCLIPPERATTACHED:
      _msg += _T("DDERR_NOCLIPPERATTACHED: No DirectDrawClipper object is attached to the surface object");
      break;

    case DDERR_NOCOLORCONVHW:
      _msg += _T("DDERR_NOCOLORCONVHW: No color-conversion hardware is present or available");
      break;

    case DDERR_NOCOLORKEY:
      _msg += _T("DDERR_NOCOLORKEY: The surface does not currently have a color key");
      break;

    case DDERR_NOCOLORKEYHW:
      _msg += _T("DDERR_NOCOLORKEYHW: There is no hardware support for the destination color key");
      break;

    case DDERR_NOCOOPERATIVELEVELSET:
      _msg += _T("DDERR_NOCOOPERATIVELEVELSET: A create function was called without the IDirectDraw7::SetCooperativeLevel method");
      break;

    case DDERR_NODC:
      _msg += _T("DDERR_NODC: No device context (DC) has ever been created for this surface");
      break;

    case DDERR_NODDROPSHW:
      _msg += _T("DDERR_NODDROPSHW: No DirectDraw raster-operation (ROP) hardware is available");
      break;

    case DDERR_NODIRECTDRAWHW:
      _msg += _T("DDERR_NODIRECTDRAWHW: Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware");
      break;

    case DDERR_NODIRECTDRAWSUPPORT:
      _msg += _T("DDERR_NODIRECTDRAWSUPPORT: DirectDraw support is not possible with the current display driver");
      break;

    case DDERR_NODRIVERSUPPORT:
      _msg += _T("DDERR_NODRIVERSUPPORT: Testing cannot proceed because the display adapter driver does not enumerate refresh rates");
      break;

    case DDERR_NOEMULATION:
      _msg += _T("DDERR_NOEMULATION: Software emulation is not available");
      break;

    case DDERR_NOEXCLUSIVEMODE:
      _msg += _T("DDERR_NOEXCLUSIVEMODE: The operation requires the application to have exclusive mode, but the application does not have exclusive mode");
      break;

    case DDERR_NOFLIPHW:
      _msg += _T("DDERR_NOFLIPHW: Flipping visible surfaces is not supported");
      break;

    case DDERR_NOFOCUSWINDOW:
      _msg += _T("DDERR_NOFOCUSWINDOW: An attempt was made to create or set a device window without first setting the focus window");
      break;

    case DDERR_NOGDI:
      _msg += _T("DDERR_NOGDI: No GDI is present");
      break;

    case DDERR_NOHWND:
      _msg += _T("DDERR_NOHWND: Clipper notification requires a window handle, or no window handle has been previously set as the cooperative level window handle");
      break;

    case DDERR_NOMIPMAPHW:
      _msg += _T("DDERR_NOMIPMAPHW: No mipmap-capable texture mapping hardware is present or available");
      break;

    case DDERR_NOMIRRORHW:
      _msg += _T("DDERR_NOMIRRORHW: No mirroring hardware is present or available");
      break;

    case DDERR_NOMONITORINFORMATION:
      _msg += _T("DDERR_NOMONITORINFORMATION: Testing cannot proceed because the monitor has no associated EDID data");
      break;

    case DDERR_NONONLOCALVIDMEM:
      _msg += _T("DDERR_NONONLOCALVIDMEM: An attempt was made to allocate nonlocal video memory from a device that does not support nonlocal video memory");
      break;

    case DDERR_NOOPTIMIZEHW:
      _msg += _T("DDERR_NOOPTIMIZEHW: The device does not support optimized surfaces");
      break;

    case DDERR_NOOVERLAYDEST:
      _msg += _T("DDERR_NOOVERLAYDEST: The IDirectDrawSurface7::GetOverlayPosition method is called on an overlay that the IDirectDrawSurface7::UpdateOverlay method has not been called on to establish as a destination");
      break;

    case DDERR_NOOVERLAYHW:
      _msg += _T("DDERR_NOOVERLAYHW: No overlay hardware is present or available");
      break;

    case DDERR_NOPALETTEATTACHED:
      _msg += _T("DDERR_NOPALETTEATTACHED: No palette object is attached to this surface");
      break;

    case DDERR_NOPALETTEHW:
      _msg += _T("DDERR_NOPALETTEHW: There is no hardware support for 16- or 256-color palettes");
      break;

    case DDERR_NORASTEROPHW:
      _msg += _T("DDERR_NORASTEROPHW: No appropriate raster-operation hardware is present or available");
      break;

    case DDERR_NOROTATIONHW:
      _msg += _T("DDERR_NOROTATIONHW: No rotation hardware is present or available");
      break;

    case DDERR_NOSTEREOHARDWARE:
      _msg += _T("DDERR_NOSTEREOHARDWARE: There is no stereo hardware present or available");
      break;

    case DDERR_NOSTRETCHHW:
      _msg += _T("DDERR_NOSTRETCHHW: There is no hardware support for stretching");
      break;

    case DDERR_NOSURFACELEFT:
      _msg += _T("DDERR_NOSURFACELEFT: There is no hardware present that supports stereo surfaces");
      break;

    case DDERR_NOT4BITCOLOR:
      _msg += _T("DDERR_NOT4BITCOLOR: The DirectDrawSurface object is not using a 4-bit color palette, and the requested operation requires a 4-bit color palette");
      break;

    case DDERR_NOT4BITCOLORINDEX:
      _msg += _T("DDERR_NOT4BITCOLORINDEX: The DirectDrawSurface object is not using a 4-bit color index palette, and the requested operation requires a 4-bit color index palette");
      break;

    case DDERR_NOT8BITCOLOR:
      _msg += _T("DDERR_NOT8BITCOLOR: The DirectDrawSurface object is not using an 8-bit color palette, and the requested operation requires an 8-bit color palette");
      break;

    case DDERR_NOTAOVERLAYSURFACE:
      _msg += _T("DDERR_NOTAOVERLAYSURFACE: An overlay component is called for a nonoverlay surface");
      break;

    case DDERR_NOTEXTUREHW:
      _msg += _T("DDERR_NOTEXTUREHW: The operation cannot be carried out because no texture-mapping hardware is present or available");
      break;

    case DDERR_NOTFLIPPABLE:
      _msg += _T("DDERR_NOTFLIPPABLE: An attempt was made to flip a surface that cannot be flipped");
      break;

    case DDERR_NOTFOUND:
      _msg += _T("DDERR_NOTFOUND: The requested item was not found");
      break;

    case DDERR_NOTINITIALIZED:
      _msg += _T("DDERR_NOTINITIALIZED: An attempt was made to call an interface method of a DirectDraw object created by CoCreateInstance before the object was initialized");
      break;

    case DDERR_NOTLOADED:
      _msg += _T("DDERR_NOTLOADED: The surface is an optimized surface, but it has not yet been allocated any memory");
      break;

    case DDERR_NOTLOCKED:
      _msg += _T("DDERR_NOTLOCKED: An attempt was made to unlock a surface that was not locked");
      break;

    case DDERR_NOTPAGELOCKED:
      _msg += _T("DDERR_NOTPAGELOCKED: An attempt was made to page-unlock a surface with no outstanding page locks");
      break;

    case DDERR_NOTPALETTIZED:
      _msg += _T("DDERR_NOTPALETTIZED: The surface being used is not a palette-based surface");
      break;

    case DDERR_NOVSYNCHW:
      _msg += _T("DDERR_NOVSYNCHW: There is no hardware support for vertical blank–synchronized operations");
      break;

    case DDERR_NOZBUFFERHW:
      _msg += _T("DDERR_NOZBUFFERHW: The operation to create a z-buffer in display memory or to perform a blit, using a z-buffer cannot be carried out because there is no hardware support for z-buffers");
      break;

    case DDERR_NOZOVERLAYHW:
      _msg += _T("DDERR_NOZOVERLAYHW: The overlay surfaces cannot be z-layered, based on the z-order because the hardware does not support z-ordering of overlays");
      break;

    case DDERR_OUTOFCAPS:
      _msg += _T("DDERR_OUTOFCAPS: The hardware needed for the requested operation has already been allocated");
      break;

    case DDERR_OUTOFMEMORY:
      _msg += _T("DDERR_OUTOFMEMORY: DirectDraw does not have enough memory to perform the operation");
      break;

    case DDERR_OUTOFVIDEOMEMORY:
      _msg += _T("DDERR_OUTOFVIDEOMEMORY: DirectDraw does not have enough display memory to perform the operation");
      break;

    case DDERR_OVERLAPPINGRECTS:
      _msg += _T("DDERR_OVERLAPPINGRECTS: The source and destination rectangles are on the same surface and overlap each other");
      break;

    case DDERR_OVERLAYCANTCLIP:
      _msg += _T("DDERR_OVERLAYCANTCLIP: The hardware does not support clipped overlays");
      break;

    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
      _msg += _T("DDERR_OVERLAYCOLORKEYONLYONEACTIVE: An attempt was made to have more than one color key active on an overlay");
      break;

    case DDERR_OVERLAYNOTVISIBLE:
      _msg += _T("DDERR_OVERLAYNOTVISIBLE: The IDirectDrawSurface7::GetOverlayPosition method was called on a hidden overlay");
      break;

    case DDERR_PALETTEBUSY:
      _msg += _T("DDERR_PALETTEBUSY: Access to this palette is refused because the palette is locked by another thread");
      break;

    case DDERR_PRIMARYSURFACEALREADYEXISTS:
      _msg += _T("DDERR_PRIMARYSURFACEALREADYEXISTS: This process has already created a primary surface");
      break;

    case DDERR_REGIONTOOSMALL:
      _msg += _T("DDERR_REGIONTOOSMALL: The region passed to the IDirectDrawClipper::GetClipList method is too small");
      break;

    case DDERR_SURFACEALREADYATTACHED:
      _msg += _T("DDERR_SURFACEALREADYATTACHED: An attempt was made to attach a surface to another surface to which it is already attached");
      break;

    case DDERR_SURFACEALREADYDEPENDENT:
      _msg += _T("DDERR_SURFACEALREADYDEPENDENT: An attempt was made to make a surface a dependency of another surface on which it is already dependent");
      break;

    case DDERR_SURFACEBUSY:
      _msg += _T("DDERR_SURFACEBUSY: Access to the surface is refused because the surface is locked by another thread");
      break;

    case DDERR_SURFACEISOBSCURED:
      _msg += _T("DDERR_SURFACEISOBSCURED: Access to the surface is refused because the surface is obscured");
      break;

    case DDERR_SURFACELOST:
      _msg += _T("DDERR_SURFACELOST: Access to the surface is refused because the surface memory is gone. Call the IDirectDrawSurface7::Restore method on this surface to restore the memory associated with it");
      break;

    case DDERR_SURFACENOTATTACHED:
      _msg += _T("DDERR_SURFACENOTATTACHED: The requested surface is not attached");
      break;

    case DDERR_TESTFINISHED:
      _msg += _T("DDERR_TESTFINISHED: When returned by the IDirectDraw7::StartModeTest method, this value means that no test could be initiated because all the resolutions chosen for testing already have refresh rate information in the registry. When returned by IDirectDraw7::EvaluateMode, the value means that DirectDraw has completed a refresh rate test");
      break;

    case DDERR_TOOBIGHEIGHT:
      _msg += _T("DDERR_TOOBIGHEIGHT: The height requested by DirectDraw is too large");
      break;

    case DDERR_TOOBIGSIZE:
      _msg += _T("DDERR_TOOBIGSIZE: The size requested by DirectDraw is too large. However, the individual height and width are valid sizes");
      break;

    case DDERR_TOOBIGWIDTH:
      _msg += _T("DDERR_TOOBIGWIDTH: The width requested by DirectDraw is too large");
      break;

    case DDERR_UNSUPPORTED:
      _msg += _T("DDERR_UNSUPPORTED: The operation is not supported");
      break;

    case DDERR_UNSUPPORTEDFORMAT:
      _msg += _T("DDERR_UNSUPPORTEDFORMAT: The pixel format requested is not supported by DirectDraw");
      break;

    case DDERR_UNSUPPORTEDMASK:
      _msg += _T("DDERR_UNSUPPORTEDMASK: The bitmask in the pixel format requested is not supported by DirectDraw");
      break;

    case DDERR_UNSUPPORTEDMODE:
      _msg += _T("DDERR_UNSUPPORTEDMODE: The display is currently in an unsupported mode");
      break;

    case DDERR_VERTICALBLANKINPROGRESS:
      _msg += _T("DDERR_VERTICALBLANKINPROGRESS: A vertical blank is in progress");
      break;

    case DDERR_VIDEONOTACTIVE:
      _msg += _T("DDERR_VIDEONOTACTIVE: The video port is not active");
      break;

    case DDERR_WASSTILLDRAWING:
      _msg += _T("DDERR_WASSTILLDRAWING: The previous blit operation that is transferring information to or from this surface is incomplete");
      break;

    case DDERR_WRONGMODE:
      _msg += _T("DDERR_WRONGMODE: This surface cannot be restored because it was created in a different mode");
      break;

    case DDERR_XALIGN:
      _msg += _T("DDERR_XALIGN: The provided rectangle was not horizontally aligned on a required boundary");
      break;
  }
  _msg += _T(" )");
}

} // namespace Direct
} // namespace Win
