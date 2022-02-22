#include "X++/XDisplay.hxx"

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

	if (! m_dis) {
		cosmos_throw(DisplayOpenError());
	}
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
