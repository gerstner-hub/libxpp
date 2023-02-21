// X++
#include "X++/Event.hxx"
#include "X++/RootWin.hxx"
#include "X++/XDisplay.hxx"

// cosmos
#include "cosmos/error/UsageError.hxx"

namespace xpp
{

XDisplay::~XDisplay() {
	XCloseDisplay(m_dis);
	m_dis = nullptr;
}

XDisplay::XDisplay() {
	// if nullptr is specified, then the value of DISPLAY
	// environment will be used
	m_dis = XOpenDisplay(nullptr);

	if (!m_dis) {
		cosmos_throw(DisplayOpenError());
	}
}

void XDisplay::getNextEvent(Event &event) {
	// xlib unconditionally returns 0 here (not documented)
	(void)XNextEvent(m_dis, event.raw());
}

XWindow XDisplay::createWindow(
		const WindowSpec &spec,
		unsigned int border_width,
		unsigned int clazz,
		const std::optional<XWindow*> parent,
		const std::optional<int> &depth,
		const std::optional<Visual*> &visual,
		const std::optional<unsigned long> &value_mask,
		const std::optional<XSetWindowAttributes*> &attrs) {

	if (value_mask && !attrs) {
		cosmos_throw(cosmos::UsageError("attrs cannot be nullptr if value_mask is given"));
	}

	static RootWin root_win;

	auto res = XCreateWindow(
		m_dis,
		parent.value_or(&root_win)->id(),
		spec.x, spec.y, spec.width, spec.height,
		border_width,
		depth ? *depth : getDefaultDepth(),
		clazz,
		visual ? *visual : getDefaultVisual(),
		value_mask ? *value_mask : 0,
		attrs ? *attrs : nullptr
	);

	return XWindow(res);
}

void XDisplay::mapWindow(const XWindow &win) {
	// this should never fail looking at current libX11 code, but you
	// never know ...
	if (XMapWindow(m_dis, win) != 1) {
		cosmos_throw(cosmos::RuntimeError("failed to map window"));
	}
}

void XDisplay::setSynchronized(bool on_off) {
	// this returns a callback function pointer we don't need
	(void)::XSynchronize(m_dis, on_off ? True : False);
}

XDisplay& XDisplay::getInstance() {
	static XDisplay dis;

	return dis;
}

XDisplay::AtomMappingError::AtomMappingError(Display *dis, const int errcode, const std::string &s) :
	X11Exception(dis, errcode) {

	m_msg = std::string("Trying to map atom '") + s + std::string("':") + m_msg;
}

XDisplay::DisplayOpenError::DisplayOpenError() :
	CosmosError("DisplayOpenError")
{
	m_msg = "Unable to open X11 display: \"";
	m_msg += XDisplayName(nullptr);
	m_msg += "\". ";
}

PixMap XDisplay::createPixmap(
	const XWindow &win,
	const Extent &extent,
	const std::optional<int> depth) const {

	auto pm = XCreatePixmap(
			m_dis, win.id(), extent.width, extent.height,
			depth ? *depth : getDefaultDepth());
	return PixMap(pm);
}

void XDisplay::freePixmap(PixMap &pm) const {
	XFreePixmap(m_dis, pm.id());
	pm.reset();
}

std::shared_ptr<struct _XGC>
XDisplay::createGraphicsContext(Drawable d, const GcOptMask &mask, const XGCValues &vals) {
	auto gc = XCreateGC(m_dis, d, mask.raw(), const_cast<XGCValues*>(&vals));

	if (!gc) {
		cosmos_throw(cosmos::RuntimeError("failed to allocate GC"));
	}

	return GcSharedPtr(gc, [this](GC c){ XFreeGC(*this, c); });
}

std::optional<Window> XDisplay::getSelectionOwner(const XAtom &selection) const {
	auto win = XGetSelectionOwner(m_dis, selection);

	if (win == None)
		return {};

	return win;
}

} // end ns
