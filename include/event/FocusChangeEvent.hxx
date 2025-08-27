#pragma once

// xpp
#include <xpp/event/AnyEvent.hxx>

namespace xpp {

/// Wrapper around the XFocusChangeEvent type.
class FocusChangeEvent :
		public AnyEvent {
public: // functions

	explicit FocusChangeEvent(const Event &ev) :
		AnyEvent{ev},
		m_ev{ev.toFocusChangeEvent()} {
	}

	NotifyMode mode() const { return NotifyMode{m_ev.mode}; }

	bool haveFocus() const { return EventType{m_ev.type} == EventType::FOCUS_IN; }

protected: // data
	const XFocusChangeEvent &m_ev;
};

} // end ns
