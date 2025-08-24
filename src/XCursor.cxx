// Cosmos
#include <cosmos/error/RuntimeError.hxx>

// xpp
#include <xpp/helpers.hxx>
#include <xpp/XColor.hxx>
#include <xpp/XCursor.hxx>
#include <xpp/XDisplay.hxx>

namespace xpp {

XCursor::XCursor(const CursorFont which, XDisplay &disp) {
	auto res = ::XCreateFontCursor(disp, cosmos::to_integral(which));

	if (res == None) {
		throw cosmos::RuntimeError{"failed to create font cursor"};
	}

	m_display = &disp;
	m_id = CursorID{res};
}

XCursor::XCursor(const Pixmap &shape, const Pixmap *mask,
		const XColor fg, const XColor bg,
		const Coord pos, XDisplay &disp) {
	auto res = ::XCreatePixmapCursor(
			disp,
			cosmos::to_integral(shape.id()),
			mask ? cosmos::to_integral(mask->id()) : None,
			const_cast<XColor*>(&fg),
			const_cast<XColor*>(&bg),
			pos.x, pos.y);

	if (res == None) {
		throw cosmos::RuntimeError{"failed to create pixmap cursor"};
	}

	m_display = &disp;
	m_id = CursorID{res};
}

void XCursor::destroy() {
	if (valid()) {
		::XFreeCursor(*m_display, cosmos::to_integral(m_id));
		invalidate();
	}
}

void XCursor::recolorCursor(const XColor &fg, const XColor &bg) {
	// does not return synchronous errors
	//
	// The call doesn't alter the input colors but the function signature
	// is broken ...
	(void)::XRecolorCursor(*m_display,
			cosmos::to_integral(m_id),
			const_cast<XColor*>(&fg),
			const_cast<XColor*>(&bg));
}

} // end ns
