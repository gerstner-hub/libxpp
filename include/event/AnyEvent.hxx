#pragma once

// xpp
#include <xpp/Event.hxx>

namespace xpp {

/// Wrapper around the base fields common to all X event structures.
class AnyEvent {
public: // functions

	explicit AnyEvent(const Event &ev) :
		m_any{ev.toAnyEvent()} {
	}

	/// The window the event is about.
	WinID window() const {
		return WinID{m_any.window};
	}

	EventType type() const { return EventType{m_any.type}; }

protected: // data
	const XAnyEvent &m_any;
};

} // end ns
