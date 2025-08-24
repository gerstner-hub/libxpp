// cosmos
#include <cosmos/error/RuntimeError.hxx>

// xpp
#include <xpp/helpers.hxx>
#include <xpp/Pixmap.hxx>
#include <xpp/XDisplay.hxx>

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

Pixmap::Pixmap(
		const DrawableID drawable,
		const std::string_view data,
		Extent extent,
		XDisplay &disp) {
	auto pm = XCreateBitmapFromData(
			disp, cosmos::to_integral(drawable),
			data.data(), extent.width, extent.height);

	if (pm == None) {
		throw cosmos::RuntimeError{"failed to allocate bitmap from data"};
	}

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
