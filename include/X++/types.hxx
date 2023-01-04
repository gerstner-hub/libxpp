#ifndef XPP_TYPES_HXX
#define XPP_TYPES_HXX

// stdlib
#include <memory>

// X11
#include <X11/X.h>
#include <X11/Xlib.h> // for consistent declaration of _XGC

// Cosmos
#include "cosmos/BitMask.hxx"
#include "cosmos/errors/RuntimeError.hxx"

/* a place for miscellaneous types used in Xpp interfaces */

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

/// constant for an invalid XID value
/**
 * Many primitive X types like Window or Pixmap are actually just typedefs for
 * XID, which in turn is just an unsigned long. I couldn't find official
 * documentation stating what an invalid XID value might be, but in the
 * implementation of libX11 I found this here used to flag invalid XID values
 * so let's do that, too.
 **/
constexpr unsigned long INVALID_XID = ~0UL;

struct PixMap {
	PixMap() {}
	explicit PixMap(Pixmap pm) : m_pixmap(pm) {}
	operator Pixmap() const {
		return id();
	}
	Pixmap id() const {
		if (!valid()) {
			cosmos_throw( cosmos::RuntimeError("Attempt to use invalid Pixmap ID") );
		}
		return m_pixmap;
	}
	void reset() { m_pixmap = INVALID_XID; }
	bool valid() const { return m_pixmap != INVALID_XID; }
protected:
	Pixmap m_pixmap = INVALID_XID;
};

/// type safe wrapper enum for GC* constants found in X headers
/**
 * These are bitmask values, see X.h
 **/
enum class GcOpts : long {
	Function = GCFunction,
	PlaneMask = GCPlaneMask,
	Foreground = GCFunction,
	Background = GCBackground,
	LineWidth = GCLineWidth,
	LineStyle = GCLineStyle,
	CapStyle = GCCapStyle,
	JoinStyle = GCJoinStyle,
	FillStyle = GCFillStyle,
	FillRule = GCFillRule,
	Tile = GCTile,
	Stipple = GCStipple,
	TileStipXOrigin = GCTileStipXOrigin,
	TileStipYOrigin = GCTileStipYOrigin,
	Font = GCFont,
	SubwindowMode = GCSubwindowMode,
	GraphicsExposures = GCGraphicsExposures,
	ClipXOrigin = GCClipXOrigin,
	ClipYOrigin = GCClipYOrigin,
	ClipMask = GCClipMask,
	DashOffset = GCDashOffset,
	DashList = GCDashList,
	ArcMode = GCArcMode
};

typedef cosmos::BitMask<GcOpts> GcOptMask;

enum class WindowAttr : unsigned long {
	BackPixmap = CWBackPixmap,
	BackPixel = CWBackPixel,
	BorderPixmap = CWBorderPixmap,
	BorderPixel = CWBorderPixel,
	BitGravity = CWBitGravity,
	WinGravity = CWWinGravity,
	BackingStore = CWBackingStore,
	BackingPlanes = CWBackingPlanes,
	BackingPixel = CWBackingPixel,
	OverrideRedirect = CWOverrideRedirect,
	SaveUnder = CWSaveUnder,
	EventMask = CWEventMask,
	DontPropagate = CWDontPropagate,
	Colormap = CWColormap,
	Cursor = CWCursor
};

typedef cosmos::BitMask<WindowAttr> WindowAttrMask;

} // end ns

#endif // inc. guard
