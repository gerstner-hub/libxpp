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

/// Strongly typed enum to represent a Screen number on a Display
enum class ScreenID : int {
	INVALID = -1
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

/// Strong type representation for X11 Window handles
enum class WinID : ::Window {
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
enum class AtomID : ::Atom {
	INVALID = None
};

enum class PixmapID : ::Pixmap {
	INVALID = INVALID_XID
};

/// can be both a WinID or a PixmapID
enum class DrawableID : ::Drawable {
	INVALID = INVALID_XID
};

enum class ColormapID : ::Colormap {
	INVALID = INVALID_XID
};

/// Represents a color index with respect to an active colormap.
enum class ColormapIndex : unsigned long {
};

enum class CursorID : ::Cursor {
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

	explicit RawProperty(const size_t len = 0, const size_t off = 0) :
			length{len}, offset{off} {}

	/// returns a read-only string_view object representing the data
	auto view() const { return std::string_view{reinterpret_cast<const char*>(data.get()), length}; }
};

enum class WindowAttr : unsigned long {
	BACK_PIXMAP       = CWBackPixmap,
	BACK_PIXEL        = CWBackPixel,
	BORDER_PIXMAP     = CWBorderPixmap,
	BORDER_PIXEL      = CWBorderPixel,
	BIT_GRAVITY       = CWBitGravity,
	WIN_GRAVITY       = CWWinGravity,
	BACKING_STORE     = CWBackingStore,
	BACKING_PLANES    = CWBackingPlanes,
	BACKING_PIXEL     = CWBackingPixel,
	OVERRIDE_REDIRECT = CWOverrideRedirect,
	SAVE_UNDER        = CWSaveUnder,
	EVENT_MASK        = CWEventMask,
	DONT_PROPAGATE    = CWDontPropagate,
	COLORMAP          = CWColormap,
	CURSOR            = CWCursor
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
	INVALID           = 0, /* zero seems unused in the X11 headers */
	KEY_PRESS         = KeyPress,
	KEY_RELEASE       = KeyRelease,
	BUTTON_PRESS      = ButtonPress,
	BUTTON_RELEASE    = ButtonRelease,
	MOTION_NOTIFY     = MotionNotify,
	ENTER_NOTIFY      = EnterNotify,
	LEAVE_NOTIFY      = LeaveNotify,
	FOCUS_IN          = FocusIn,
	FOCUS_OUT         = FocusOut,
	KEYMAP_NOTIFY     = KeymapNotify,
	EXPOSE            = Expose,
	GRAPHICS_EXPOSE   = GraphicsExpose,
	NOEXPOSE          = NoExpose,
	VISIBILITY_NOTIFY = VisibilityNotify,
	CREATE_NOTIFY     = CreateNotify,
	DESTROY_NOTIFY    = DestroyNotify,
	UNMAP_NOTIFY      = UnmapNotify,
	MAP_NOTIFY        = MapNotify,
	MAP_REQUEST       = MapRequest,
	REPARENT_NOTIFY   = ReparentNotify,
	CONFIGURE_NOTIFY  = ConfigureNotify,
	CONFIGURE_REQUEST = ConfigureRequest,
	GRAVITY_NOTIFY    = GravityNotify,
	RESIZE_REQUEST    = ResizeRequest,
	CIRCULATE_NOTIFY  = CirculateNotify,
	CIRCULATE_REQUEST = CirculateRequest,
	PROPERTY_NOTIFY   = PropertyNotify,
	SELECTION_CLEAR   = SelectionClear,
	SELECTION_REQUEST = SelectionRequest,
	SELECTION_NOTIFY  = SelectionNotify,
	COLORMAP_NOTIFY   = ColormapNotify,
	CLIENT_MESSAGE    = ClientMessage,
	MAPPING_NOTIFY    = MappingNotify
};

enum class EventMask : long {
	NO_EVENT              = NoEventMask,
	KEY_PRESSES           = KeyPressMask,
	KEY_RELEASES          = KeyReleaseMask,
	BUTTON_PRESSES        = ButtonPressMask,
	BUTTON_RELEASES       = ButtonReleaseMask,
	ENTER_WINDOW          = EnterWindowMask,
	LEAVE_WINDOW          = LeaveWindowMask,
	POINTER_MOTION        = PointerMotionMask,
	POINTER_MOTION_HINT   = PointerMotionHintMask,
	BUTTON1_MOTION        = Button1MotionMask,
	BUTTON2_MOTION        = Button2MotionMask,
	BUTTON3_MOTION        = Button3MotionMask,
	BUTTON4_MOTION        = Button4MotionMask,
	BUTTON5_MOTION        = Button5MotionMask,
	BUTTON_MOTION         = ButtonMotionMask,
	KEYMAP_STATE          = KeymapStateMask,
	EXPOSURE              = ExposureMask,
	VISIBILITY_CHANGE     = VisibilityChangeMask,
	STRUCTURE_NOTIFY      = StructureNotifyMask,
	RESIZE_REDIRECT       = ResizeRedirectMask,
	SUBSTRUCTURE_NOTIFY   = SubstructureNotifyMask,
	SUBSTRUCTURE_REDIRECT = SubstructureRedirectMask,
	FOCUS_CHANGE          = FocusChangeMask,
	PROPERTY_CHANGE       = PropertyChangeMask,
	COLORMAP_CHANGE       = ColormapChangeMask,
	OWNER_GRAB_BUTTON     = OwnerGrabButtonMask
};

using EventSelectionMask = cosmos::BitMask<EventMask>;

enum class Gravity {
	FORGET     = ForgetGravity,
	NORTH_WEST = NorthWestGravity,
	NORTH      = NorthGravity,
	NORTH_EAST = NorthEastGravity,
	WEST       = WestGravity,
	CENTER     = CenterGravity,
	EAST       = EastGravity,
	SOUTH_WEST = SouthWestGravity,
	SOUTH      = SouthGravity,
	SOUTH_EAST = SouthEastGravity,
	STATIC     = StaticGravity
};

enum class WindowClass : long {
	/// take over the parent window's class
	COPY_FROM_PARENT = CopyFromParent,
	/// A regular I/O window that can also do drawing operations
	INPUT_OUTPUT     = InputOutput,
	/// An invisible window that only deals with things like input events, grabbing and cursors
	INPUT_ONLY       = InputOnly
};

enum class NotifyMode : int {
	NORMAL        = NotifyNormal,
	GRAB          = NotifyGrab,
	UNGRAB        = NotifyUngrab,
	WHILE_GRABBED = NotifyWhileGrabbed
};

/// Keyboard and Mouse event input modifiers.
enum class InputModifier : unsigned int {
	NONE = 0,
	SHIFT = ShiftMask,
	LOCK = LockMask,
	CONTROL = ControlMask,
	MOD1 = Mod1Mask,
	MOD2 = Mod2Mask,
	MOD3 = Mod3Mask,
	MOD4 = Mod4Mask,
	MOD5 = Mod5Mask,
	BUTTON1 = Button1Mask,
	BUTTON2 = Button2Mask,
	BUTTON3 = Button3Mask,
	BUTTON4 = Button4Mask,
	BUTTON5 = Button5Mask,
	ANY = AnyModifier,
	// XKB extention group index bits, see
	// https://www.x.org/releases/X11R7.7/doc/libX11/XKB/xkblib.html
	// "Xkb state to core protocol state transformation"
	XKB_GROUP_INDEX = (1 << 13 | 1 << 14)
};

using InputMask = cosmos::BitMask<InputModifier>;

/// Represents a button number in ButtonEvent.
/**
 * Apart from the five button constants present in Xlib there could
 * potentially also be a higher numbered button without a dedicated constant.
 **/
enum class Button : unsigned int {
	BUTTON1 = Button1,
	BUTTON2 = Button2,
	BUTTON3 = Button3,
	BUTTON4 = Button4,
	BUTTON5 = Button5
};

} // end ns

#endif // inc. guard
