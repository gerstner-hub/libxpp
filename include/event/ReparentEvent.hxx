#pragma once

// xpp
#include <xpp/event/AnyEvent.hxx>
#include <xpp/types.hxx>

namespace xpp {

/// Wrapper around the XReparentEvent type.
class ReparentEvent :
		public AnyEvent {
public: // functions

	explicit ReparentEvent(const Event &ev) :
		AnyEvent{ev.toAnyEvent()},
		m_ev{ev.toReparentNotify()} {
	}

	/// New parent of the window which was reparented.
	WinID newParent() const {
		return WinID{m_ev.parent};
	}

	/// The window which was reparented.
	WinID reparentedWindow() const {
		return WinID{m_ev.window};
	}

	/// Either the reparented window or the old/new parent.
	/**
	 * The interpretation of this depends on whether StructureNotify or
	 * SubstructureNotify was selected.
	 **/
	WinID eventWindow() const {
		return WinID{m_ev.event};
	}

	/// Event should be overriden.
	bool overrideRedirect() const {
		return m_ev.override_redirect != False;
	}

	/// The reparented Window's coordinates relative to the new parent window's origin.
	Coord upperLeftPos() const {
		return Coord{m_ev.x, m_ev.y};
	}

protected: // data

	const XReparentEvent &m_ev;
};

} // end ns
