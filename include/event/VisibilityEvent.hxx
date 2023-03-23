#ifndef XPP_VISIBILITYEVENT_HXX
#define XPP_VISIBILITYEVENT_HXX

// X++
#include "X++/Event.hxx"

namespace xpp {

/// Strong state type for possible VisibilityEvent status reports.
enum class VisibilityState : int {
	UNOBSCURED = VisibilityUnobscured,
	PARTIALLY_OBSCURED = VisibilityPartiallyObscured,
	FULLY_OBSCURED = VisibilityFullyObscured
};

/// Wrapper around the XVisibilityEvent type.
class VisibilityEvent {
public: // functions

	explicit VisibilityEvent(const Event &ev) :
		m_ev{ev.toVisibilityNotify()} {}

	VisibilityState state() const { return VisibilityState{m_ev.state}; }

protected: // data
	const XVisibilityEvent &m_ev;
};

} // end ns

#endif // inc. guard
