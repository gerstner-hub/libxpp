#pragma once

// X++
#include "X++/Event.hxx"

namespace xpp {

/// Strong state type for possible PropertyEvent status reports.
enum class PropertyNotification : int {
	NEW_VALUE = PropertyNewValue,
	PROPERTY_DELETE = PropertyDelete
};

/// Wrapper around the XPropertyEvent type.
class PropertyEvent {
public: // functions

	explicit PropertyEvent(const Event &ev) :
		m_ev{ev.toPropertyNotify()} {}

	/// Returns the kind of property notification event: new value or deleted
	PropertyNotification state() const { return PropertyNotification{m_ev.state}; }

	/// Returns the atom corresponding to the property the event is about
	AtomID property() const { return AtomID{m_ev.atom}; }

protected: // data
	const XPropertyEvent &m_ev;
};

} // end ns
