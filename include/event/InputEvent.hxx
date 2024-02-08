#pragma once

// xpp
#include "xpp/Event.hxx"
#include "xpp/types.hxx"

namespace xpp {

/// Wrapper around input event types that mostly share the same fields..
/**
 * Input handling events like XMotionEvent and XButtonEvent share most of the
 * event fields. This templated base class allows to share code for these
 * events while still providing type safety.
 *
 * Specializations of this call need to be derived to make it useful
 **/
template <typename EVENT>
class InputEvent {
public: // functions

	EventType type() const { return EventType{m_ev.type}; }

	/// Returns key / button state related to the event.
	InputMask state() const { return InputMask{m_ev.state}; }

	/// Returns the window coordinates where the event took place.
	Coord pos() const { return Coord{m_ev.x, m_ev.y}; }

	Time time() const { return m_ev.time; }

	auto& raw() const { return m_ev; }

protected: // functions

	explicit InputEvent(const EVENT &ev) :
			m_ev{ev} {}

protected: // data
	const EVENT &m_ev;
};

} // end ns
