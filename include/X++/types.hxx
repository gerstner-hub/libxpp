#ifndef XPP_TYPES_HXX
#define XPP_TYPES_HXX

// X11
#include <X11/X.h>

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

} // end ns

#endif // inc. guard
