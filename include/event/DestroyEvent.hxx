#pragma once

// xpp
#include <xpp/Event.hxx>
#include <xpp/types.hxx>

namespace xpp {

/// Wrapper around the XCreateWindowEvent type.
class DestroyEvent {
public: // functions

	explicit DestroyEvent(const Event &ev) :
		m_ev{ev.toDestroyNotify()} {}

	/// The destroyed window
	WinID window() const {
		return WinID{m_ev.window};
	}

	/// The window the event appeared on.
	/**
	 * This will be either the same as window() or its parent, in case of
	 * SubStructureNotify event reporting.
	 **/
	WinID eventWin() const {
		return WinID{m_ev.event};
	}

protected: // data

	const XDestroyWindowEvent &m_ev;
};

} // end ns

