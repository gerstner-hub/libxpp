// X++
#include "X++/helpers.hxx"
#include "X++/XColor.hxx"
#include "X++/XCursor.hxx"

namespace xpp {

XCursor::XCursor(const CursorFont which, XDisplay &disp) {
	auto res = ::XCreateFontCursor(disp, cosmos::to_integral(which));

	if (res == None) {
		cosmos_throw (cosmos::RuntimeError("failed to create font cursor"));
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
