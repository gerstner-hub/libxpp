// X++
#include "X++/helpers.hxx"
#include "X++/Pixmap.hxx"

namespace xpp {

Pixmap::Pixmap(
		const WinID win,
		const Extent extent,
		const std::optional<int> depth,
		XDisplay &disp) {

	auto pm = ::XCreatePixmap(
			disp, raw_win(win), extent.width, extent.height,
			depth ? *depth : display.defaultDepth());

	m_display = &disp;
	m_id = PixmapID{pm};
}

void Pixmap::destroy() {
	if (valid()) {
		::XFreePixmap(*m_display, cosmos::to_integral(m_id));
		invalidate();
	}
}

} // end ns
