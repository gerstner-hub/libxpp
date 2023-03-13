// cosmos
#include "cosmos/algs.hxx"
#include "cosmos/error/UsageError.hxx"

// X++
#include "X++/Event.hxx"
#include "X++/RootWin.hxx"
#include "X++/XDisplay.hxx"

namespace xpp {

// only initialize this in xpp::init, otherwise we conflict with XInitThreads()
XDisplay display{XDisplay::Initialize{false}};
Visual *visual = nullptr;
ColorMapID colormap = ColorMapID::INVALID;
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
		unsigned int clazz,
		const std::optional<XWindow*> parent,
		const std::optional<int> &depth,
		const std::optional<Visual*> &p_visual,
		const std::optional<unsigned long> &value_mask,
		const std::optional<XSetWindowAttributes*> &attrs) {

	if (value_mask && !attrs) {
		cosmos_throw (cosmos::UsageError("attrs cannot be nullptr if value_mask is given"));
	}

	static RootWin root_win;

	auto res = ::XCreateWindow(
		m_dis,
		raw_win(parent.value_or(&root_win)->id()),
		spec.x, spec.y, spec.width, spec.height,
		border_width,
		depth ? *depth : defaultDepth(),
		clazz,
		p_visual ? *p_visual : defaultVisual(),
		value_mask ? *value_mask : 0,
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

PixMapID XDisplay::createPixmap(
		const WinID win,
		const Extent &extent,
		const std::optional<int> depth) const {

	auto pm = ::XCreatePixmap(
			m_dis, raw_win(win), extent.width, extent.height,
			depth ? *depth : defaultDepth());
	return PixMapID{pm};
}

void XDisplay::freePixmap(PixMapID pm) const {
	::XFreePixmap(m_dis, cosmos::to_integral(pm));
}

std::shared_ptr<struct _XGC>
XDisplay::createGraphicsContext(DrawableID d, const GcOptMask &mask, const XGCValues &vals) {
	auto gc = ::XCreateGC(m_dis, cosmos::to_integral(d), mask.raw(), const_cast<XGCValues*>(&vals));

	if (!gc) {
		cosmos_throw (cosmos::RuntimeError("failed to allocate GC"));
	}

	return GcSharedPtr{gc, [this](GC c){ ::XFreeGC(*this, c); }};
}

std::optional<WinID> XDisplay::selectionOwner(const AtomID selection) const {
	auto win = ::XGetSelectionOwner(m_dis, raw_atom(selection));

	if (win == None)
		return {};

	return WinID{win};
}

} // end ns
