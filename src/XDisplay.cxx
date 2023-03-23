// cosmos
#include "cosmos/algs.hxx"
#include "cosmos/error/UsageError.hxx"
#include "cosmos/error/RuntimeError.hxx"

// X++
#include "X++/Event.hxx"
#include "X++/RootWin.hxx"
#include "X++/SetWindowAttributes.hxx"
#include "X++/XColor.hxx"
#include "X++/XDisplay.hxx"

namespace xpp {

// only initialize this in xpp::init, otherwise we conflict with XInitThreads()
XDisplay display{XDisplay::Initialize{false}};
Visual *visual = nullptr;
ColormapID colormap = ColormapID::INVALID;
ScreenID screen = ScreenID::INVALID;

XDisplay::~XDisplay() {
	if (m_dis) {
		::XCloseDisplay(m_dis);
		m_dis = nullptr;
	}
}

XDisplay::XDisplay(const Initialize init) {
	if (init) {
		// if nullptr is specified, then the value of DISPLAY environment will be used
		m_dis = ::XOpenDisplay(nullptr);

		if (!m_dis) {
			cosmos_throw (DisplayOpenError());
		}
	}
}

void XDisplay::nextEvent(Event &event) {
	// xlib unconditionally returns 0 here (not documented)
	(void)::XNextEvent(m_dis, event.raw());
}

WinID XDisplay::createWindow(
		const WindowSpec &spec,
		unsigned int border_width,
		const WindowClass clazz,
		const std::optional<XWindow*> parent,
		const std::optional<int> depth,
		const std::optional<Visual*> p_visual,
		const std::optional<WindowAttrMask> value_mask,
		const std::optional<SetWindowAttributes*> attrs) {

	if (value_mask && !attrs) {
		cosmos_throw (cosmos::UsageError("attrs cannot be unset if value_mask is set"));
	}

	static RootWin root_win;

	auto res = ::XCreateWindow(
		m_dis,
		raw_win(parent.value_or(&root_win)->id()),
		spec.x, spec.y, spec.width, spec.height,
		border_width,
		depth ? *depth : defaultDepth(),
		cosmos::to_integral(clazz),
		p_visual ? *p_visual : defaultVisual(),
		value_mask ? value_mask->raw() : 0,
		attrs ? *attrs : nullptr
	);

	return WinID{res};
}

void XDisplay::mapWindow(const XWindow &win) {
	// this should never fail looking at current libX11 code, but you
	// never know ...
	if (::XMapWindow(m_dis, raw_win(win.id())) != 1) {
		cosmos_throw (cosmos::RuntimeError("failed to map window"));
	}
}

void XDisplay::setSynchronized(bool on_off) {
	// this returns a callback function pointer we don't need
	(void)::XSynchronize (m_dis, on_off ? True : False);
}

XDisplay::AtomMappingError::AtomMappingError(Display *dis, const int errcode, const std::string_view s) :
		X11Exception{dis, errcode} {

	m_msg = std::string("Trying to map atom '") + std::string{s} + std::string("':") + m_msg;
}

XDisplay::DisplayOpenError::DisplayOpenError() :
		CosmosError{"DisplayOpenError"} {
	m_msg = "Unable to open X11 display: \"";
	m_msg += ::XDisplayName(nullptr);
	m_msg += "\". ";
}

void XDisplay::parseColor(XColor &out, std::string_view name, const std::optional<ColormapID> p_colormap) {
	auto res = ::XParseColor(m_dis,
			raw_cmap(p_colormap ? *p_colormap : xpp::colormap),
			name.empty() ? nullptr : name.data(),
			&out);

	if (res == None) {
		cosmos_throw (cosmos::RuntimeError("failed to parse color"));
	}
}

std::optional<WinID> XDisplay::selectionOwner(const AtomID selection) const {
	auto win = ::XGetSelectionOwner(m_dis, raw_atom(selection));

	if (win == None)
		return {};

	return WinID{win};
}

} // end ns
