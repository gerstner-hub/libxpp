// X++
#include "X++/helpers.hxx"
#include "X++/XColor.hxx"
#include "X++/XCursor.hxx"
#include "X++/XDisplay.hxx"

namespace xpp {

void XCursor::destroy() {
	if (valid()) {
		XFreeCursor(*m_display, cosmos::to_integral(m_id));
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
