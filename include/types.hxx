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

/// constant for an invalid XID value
/**
 * Many primitive X types like Window or Pixmap are actually just typedefs for
 * XID, which in turn is just an unsigned long. I couldn't find official
 * documentation stating what an invalid XID value might be, but in the
 * implementation of libX11 I found this here used to flag invalid XID values
 * so let's do that, too.
 **/
constexpr unsigned long INVALID_XID = ~0UL;

/// Strong type representation for X11 Window handles
enum class WinID : Window {
	INVALID = INVALID_XID
};

/// Strong type representation for X11 Atom handles
/**
 * In the Xlib world we have Atoms which are unique identifiers for:
 *
 * - property names
 * - property types
 *
 *  Properties are attached to windows. For example WM_NAME is the name of a
 *  window property that contains a STRING data type that makes up the title
 *  of a window.
 *
 *  The property names and types are Latin1 encoded strings (at least the
 *  manual says so). Xlib uses Atoms as alternative representations for
 *  these strings. This is for efficiency reasons as it is cheaper than
 *  passing strings around.
 *
 *  In the Xlib the Atom type is some integer-like type that doesn't provide
 *  much type safety. This this enum class makes this safer within libX++.
 **/
enum class AtomID : Atom {
	INVALID = None
};

enum class PixMapID : Pixmap {
	INVALID = INVALID_XID
};

/// can be both a WinID or a PixMapID
enum class DrawableID : Drawable {
	INVALID = INVALID_XID
};

/// A vector of primitive Atom values
using AtomVector = std::vector<Atom>;

/// A vector of strongly typed AtomID values
using AtomIDVector = std::vector<AtomID>;

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

using WindowAttrMask = cosmos::BitMask<WindowAttr>;

/// XEvent structure type differentiation
/**
 * XEvent is a C union type. The interpretation of which depends on the type
 * value modeled in this enum.
 *
 * NOTE: the constants use Ev prefixes, because the X11 constants *don_t* use
 * them, and since they're preprocessor defines, we'd get into trouble reusing
 * them.
 **/
enum class EventType : int {
	INVALID             = 0, /* zero seems unused in the X11 headers */
	Ev_ConfigureNotify  = ConfigureNotify,
	Ev_MapNotify        = MapNotify,
	Ev_VisibilityNotify = VisibilityNotify,
	Ev_FocusIn          = FocusIn,
	Ev_FocusOut         = FocusOut,
	Ev_KeyPress         = KeyPress,
	Ev_ClientMessage    = ClientMessage,
	Ev_ButtonRelease    = ButtonRelease,
	Ev_ButtonPress      = ButtonPress,
	Ev_PropertyNotify   = PropertyNotify,
	Ev_SelectionNotify  = SelectionNotify,
	Ev_SelectionRequest = SelectionRequest
};

enum class EventMask : long {
	NoEvent              = NoEventMask,
	KeyPresses           = KeyPressMask,
	KeyReleases          = KeyReleaseMask,
	ButtonPresses        = ButtonPressMask,
	ButtonReleases       = ButtonReleaseMask,
	EnterWindow          = EnterWindowMask,
	LeaveWindow          = LeaveWindowMask,
	PointerMotion        = PointerMotionMask,
	PointerMotionHint    = PointerMotionHintMask,
	Button1Motion        = Button1MotionMask,
	Button2Motion        = Button2MotionMask,
	Button3Motion        = Button3MotionMask,
	Button4Motion        = Button4MotionMask,
	Button5Motion        = Button5MotionMask,
	ButtonMotion         = ButtonMotionMask,
	KeymapState          = KeymapStateMask,
	Exposure             = ExposureMask,
	VisibilityChange     = VisibilityChangeMask,
	StructureNotify      = StructureNotifyMask,
	ResizeRedirect       = ResizeRedirectMask,
	SubstructureNotify   = SubstructureNotifyMask,
	SubstructureRedirect = SubstructureRedirectMask,
	FocusChange          = FocusChangeMask,
	PropertyChange       = PropertyChangeMask,
	ColormapChange       = ColormapChangeMask,
	OwnerGrabButton      = OwnerGrabButtonMask
};

using EventSelectionMask = cosmos::BitMask<EventMask>;

} // end ns

#endif // inc. guard
