#ifndef XPP_TYPES_HXX
#define XPP_TYPES_HXX

// C++
#include <memory>
#include <string_view>
#include <vector>

// X11
#include <X11/X.h>
#include <X11/Xlib.h> // for consistent declaration of _XGC

// Cosmos
#include "cosmos/BitMask.hxx"
#include "cosmos/error/RuntimeError.hxx"

/**
 * @file
 *
 * A place for miscellaneous types used in libX++ interfaces.
 **/

namespace xpp {

/// represents a window location and dimension
struct WindowSpec {
	int x = 0;
	int y = 0;
	unsigned int width = 0;
	unsigned int height = 0;
};

/// A two-dimensional extent in pixels (e.g. for creating pixmaps)
struct Extent {
	unsigned int width = 0;
	unsigned int height = 0;
};

/// A Pixel coordinate for drawing operations
struct Coord {
	int x = 0;
	int y = 0;
};

/// smart pointer for GC graphics context
/**
 * The `GC` specified in Xlib method signatures is actually a pointer
 * preprocessor define, thus we have to use the internal name of the struct
 * behind this to create a valid shared_ptr around it.
 **/
typedef std::shared_ptr<struct _XGC> GcSharedPtr;

/// A vector of primitive Atom values
using AtomVector = std::vector<Atom>;

class XAtom; // fwd. decl.

/// A vector of strongly typed XAtom values
using XAtomVector = std::vector<XAtom>;

/// constant for an invalid XID value
/**
 * Many primitive X types like Window or Pixmap are actually just typedefs for
 * XID, which in turn is just an unsigned long. I couldn't find official
 * documentation stating what an invalid XID value might be, but in the
 * implementation of libX11 I found this here used to flag invalid XID values
 * so let's do that, too.
 **/
constexpr unsigned long INVALID_XID = ~0UL;

enum class WinID : Window {
	INVALID = INVALID_XID
};

enum class AtomID : Atom {
	INVALID = None
};

enum class PixMapID : Pixmap {
	INVALID = INVALID_XID
};

/// can be both a WinID or a PixMapID
enum class DrawableID : Drawable {
};

/// represents a raw window property
struct RawProperty {
	std::shared_ptr<uint8_t> data;
	size_t length = 0; /// number of bytes found in data
	size_t offset = 0; /// byte offset of the data into the original property
	size_t left = 0; /// the number of bytes of the property left to read, if a partial read was performed

	/// returns a read-only string_view object representing the data
	auto view() const { return std::string_view(reinterpret_cast<const char*>(data.get()), length); }
};

/// type safe wrapper enum for GC* constants found in X headers
/**
 * These are bitmask values, see X.h
 **/
enum class GcOpts : long {
	Function          = GCFunction,
	PlaneMask         = GCPlaneMask,
	Foreground        = GCFunction,
	Background        = GCBackground,
	LineWidth         = GCLineWidth,
	LineStyle         = GCLineStyle,
	CapStyle          = GCCapStyle,
	JoinStyle         = GCJoinStyle,
	FillStyle         = GCFillStyle,
	FillRule          = GCFillRule,
	Tile              = GCTile,
	Stipple           = GCStipple,
	TileStipXOrigin   = GCTileStipXOrigin,
	TileStipYOrigin   = GCTileStipYOrigin,
	Font              = GCFont,
	SubwindowMode     = GCSubwindowMode,
	GraphicsExposures = GCGraphicsExposures,
	ClipXOrigin       = GCClipXOrigin,
	ClipYOrigin       = GCClipYOrigin,
	ClipMask          = GCClipMask,
	DashOffset        = GCDashOffset,
	DashList          = GCDashList,
	ArcMode           = GCArcMode
};

typedef cosmos::BitMask<GcOpts> GcOptMask;

enum class WindowAttr : unsigned long {
	BackPixmap       = CWBackPixmap,
	BackPixel        = CWBackPixel,
	BorderPixmap     = CWBorderPixmap,
	BorderPixel      = CWBorderPixel,
	BitGravity       = CWBitGravity,
	WinGravity       = CWWinGravity,
	BackingStore     = CWBackingStore,
	BackingPlanes    = CWBackingPlanes,
	BackingPixel     = CWBackingPixel,
	OverrideRedirect = CWOverrideRedirect,
	SaveUnder        = CWSaveUnder,
	EventMask        = CWEventMask,
	DontPropagate    = CWDontPropagate,
	Colormap         = CWColormap,
	Cursor           = CWCursor
};

typedef cosmos::BitMask<WindowAttr> WindowAttrMask;

} // end ns

#endif // inc. guard
