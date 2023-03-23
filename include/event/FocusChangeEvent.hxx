#ifndef XPP_FOCUSCHANGEEVENT_HXX
#define XPP_FOCUSCHANGEEVENT_HXX

// X++
#include "X++/Event.hxx"

namespace xpp {

/// Wrapper around the XFocusChangeEvent type.
class FocusChangeEvent {
public: // functions

	explicit FocusChangeEvent(const Event &ev) :
		m_ev{ev.toFocusChangeEvent()} {}

	NotifyMode mode() const { return NotifyMode{m_ev.mode}; }

	bool haveFocus() const { return EventType{m_ev.type} == EventType::FOCUS_IN; }

protected: // data
	const XFocusChangeEvent &m_ev;
};

} // end ns

#endif // inc. guard
