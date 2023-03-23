/**
 * @file
 *
 * This header contains forward declarations of often required types found in
 * the libX++ library.
 **/

namespace xpp {
	class Event;
	class GraphicsContext;
	class Pixmap;
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

	extern XDisplay display;
}
