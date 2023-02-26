#ifndef XPP_EVENT_HXX
#define XPP_EVENT_HXX

// C++
#include <optional>

// xlib
#include <X11/Xlib.h>

// cosmos
#include "cosmos/algs.hxx"

// xpp
#include "X++/XWindow.hxx"

namespace xpp {

/// Wraps an XEvent libX11 union
/**
 * The wrapper provides safe cast helpers like toConfigureNotify() that return
 * the properly typed variant of the union but throw an exception if the type
 * field doesn't match.
 **/
class XPP_API Event {
public: // functions

	explicit Event(const XEvent &ev) :
			m_ev{ev} {}

	Event() {
		m_ev.type = (int)EventType::INVALID;
	}

	XEvent* raw() { return &m_ev; }
	const XEvent* raw() const { return &m_ev; }

	/// Check whether the given event should be filtered.
	/**
	 * Certain layers above libX11 (like XOpenIM) make use of events on
	 * their own. To avoid getting confused by these events and to make
	 * these layers work correctly this call should be used to check
	 * whether this is the case for this event.
	 * 
	 * If no window is passed then the window value stored in the event
	 * structure will be used.
	 *
	 * \return \c true if the event is to be filtered, \c false if the
	 *         caller should continue processing the event.
	 **/
	bool filterEvent(std::optional<const XWindow> w = std::nullopt) const {
		// the call shouldn't modify the event, thus const_cast it
		//
		// if it *is* modified then only if filtering applies in which
		// case the event shouldn't be processed by the caller anyway
		auto filter = ::XFilterEvent(const_cast<XEvent*>(&m_ev),
				w ? raw_win(w->id()) : None);
		return filter == True;
	}

	bool isConfigureNotify() const  { return type() == EventType::Ev_ConfigureNotify; }
	bool isMapNotify() const        { return type() == EventType::Ev_MapNotify; }
	bool isVisibilityNotify() const { return type() == EventType::Ev_VisibilityNotify; }
	bool isFocusChange() const      { return type() == EventType::Ev_FocusIn || type() == EventType::Ev_FocusOut; }
	bool isKeyPress() const         { return type() == EventType::Ev_KeyPress; }
	bool isClientMessage() const    { return type() == EventType::Ev_ClientMessage; }
	bool isButtonEvent() const      { return type() == EventType::Ev_ButtonRelease || type() == EventType::Ev_ButtonPress; }
	bool isPropertyNotify() const   { return type() == EventType::Ev_PropertyNotify; }
	bool isSelectionNotify() const  { return type() == EventType::Ev_SelectionNotify; }
	bool isSelectionRequest() const { return type() == EventType::Ev_SelectionRequest; }

	EventType type() const { return EventType{m_ev.type}; }

	void onMismatch(const bool matches) const {
		if (!matches)
			raiseMismatch();
	}

	auto toConfigureNotify() const {
		onMismatch(isConfigureNotify());
		return m_ev.xconfigure;
	}

	auto toVisibilityNotify() const {
		onMismatch(isVisibilityNotify());
		return m_ev.xvisibility;
	}

	auto toFocusChangeEvent() const {
		onMismatch(isFocusChange());
		return m_ev.xfocus;
	}

	auto toKeyEvent() const {
		onMismatch(isKeyPress());
		return m_ev.xkey;
	}

	auto toClientMessage() const {
		onMismatch(isClientMessage());
		return m_ev.xclient;
	}

	auto toButtonEvent() const {
		onMismatch(isButtonEvent());
		return m_ev.xbutton;
	}

	auto toProperty() const {
		onMismatch(isPropertyNotify());
		return m_ev.xproperty;
	}

	auto toSelectionRequest() const {
		onMismatch(isSelectionRequest());
		return m_ev.xselectionrequest;
	}

	auto toSelectionNotify() const {
		onMismatch(isSelectionNotify());
		return m_ev.xselection;
	}

protected: // functions

	void raiseMismatch() const;

protected: // data

	XEvent m_ev;
};

} // end ns

#endif // inc. guard
