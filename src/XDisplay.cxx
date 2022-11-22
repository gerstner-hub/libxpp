// X++
#include "X++/Event.hxx"
#include "X++/RootWin.hxx"
#include "X++/XDisplay.hxx"

// cosmos
#include "cosmos/errors/UsageError.hxx"

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

} // end ns
