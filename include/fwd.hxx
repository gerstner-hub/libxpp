/**
 * @file
 *
 * This header contains forward declarations of often required types found in
 * the libxpp library.
 **/

namespace xpp {
	class Event;
	class GraphicsContext;
	class Pixmap;
	class RootWin;
	class SetWindowAttributes;
	class SizeHints;
	class WindowManagerHints;
	class XColor;
	class XCursor;
	class XDisplay;
	class XWindow;
	class XWindowAttrs;
	struct utf8_string;
	template <typename PROPTYPE> class Property;

	class ButtonEvent;
	class ClientMessageEvent;
	class ConfigureEvent;
	class FocusChangeEvent;
	class KeyEvent;
	class PointerMovedEvent;
	class PropertyEvent;
	class SelectionEvent;
	class SelectionRequestEvent;
	class SelectionClearEvent;
	class VisibilityEvent;

	extern XDisplay display;
}
