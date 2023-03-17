// libX11
#include <X11/XKBlib.h>

// X++
#include "X++/helpers.hxx"
#include "X++/keyboard.hxx"
#include "X++/XWindow.hxx"
#include "X++/XDisplay.hxx"

namespace xpp {

bool ring_bell(const XWindow &win,
		const BellVolume volume,
		const std::optional<AtomID> name,
		std::optional<XDisplay*> p_display) {

	auto res = ::XkbBell(
		p_display ? **p_display : xpp::display,
		xpp::raw_win(win.id()),
		cosmos::to_integral(volume),
		name ? raw_atom(*name) : None
	);

	return res ? true : false;
}

} // end ns
