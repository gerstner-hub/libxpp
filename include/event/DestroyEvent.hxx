#pragma once

// xpp
#include <xpp/event/AnyEvent.hxx>
#include <xpp/types.hxx>

namespace xpp {

/// Wrapper around the XCreateWindowEvent type.
/**
 * AnyEvent::window() provides access to the destroyed window's ID.
 **/
class DestroyEvent :
		public AnyEvent {
public: // functions

	explicit DestroyEvent(const Event &ev) :
		AnyEvent{ev},
		m_ev{ev.toDestroyNotify()} {}

	/// The window the event appeared on.
	/**
	 * This will be either the same as window() or its parent, in case of
	 * SubStructureNotify event reporting.
	 **/
	WinID eventWin() const {
		return WinID{m_ev.event};
	}

	/// The ID of the window which was destroyed.
	/**
	 * This overrides AnyEvent::window() which refers to the eventWin()
	 * instead of to the window which was actually destroyed.
	 **/
	WinID window() const {
		return WinID{m_ev.window};
	}

protected: // data

	const XDestroyWindowEvent &m_ev;
};

} // end ns

