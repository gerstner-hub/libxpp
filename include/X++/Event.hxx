#ifndef XPP_EVENT_HXX
#define XPP_EVENT_HXX

// C++
#include <optional>

// xlib
#include <X11/Xlib.h>

// xpp
#include "X++/XWindow.hxx"

namespace xpp {

/// Wraps an XEvent libX11 union
class Event {
public: // functions
	explicit Event(const XEvent &ev) :
		m_ev(ev) {}

	Event() {
		m_ev.type = 0;
	}

	XEvent* raw() { return &m_ev; }
	const XEvent* raw() const { return &m_ev; }

	/**
	 * \brief
	 * 	Check whether the given event should be filtered
	 * \details
	 * 	Certain layers above libX11 (like XOpenIM) make use of events
	 * 	on their own. To avoid getting confused by these events and to
	 * 	make these layers work correctly this call should be used to
	 * 	check whether this is the case for this event.
	 *
	 * 	If no window is passed then the window value stored in the
	 * 	event structure will be used.
	 * \return
	 * 	\c true if the event is to be filtered, \c false if the caller
	 * 	should continue processing the event.
	 **/
	bool filterEvent(std::optional<const XWindow> w = std::nullopt) const {
		// the call shouldn't modify the event, thus const_cast it
		//
		// if it *is* modified then only if filtering applies in which
		// case the event shouldn't be processed by the caller anyway
		return XFilterEvent(const_cast<XEvent*>(&m_ev), w ? *w : None) == True;
	}

	bool isConfigureNotify() const { return m_ev.type == ConfigureNotify; }
	bool isMapNotify() const { return m_ev.type == MapNotify; }

	auto toConfigureNotify() {
		if (!isConfigureNotify())
			raiseMismatch();

		return m_ev.xconfigure;
	}

protected: // functions

	void raiseMismatch();

protected: // data
	XEvent m_ev;
};

} // end ns

#endif // inc. guard
