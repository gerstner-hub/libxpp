#pragma once

// xpp
#include <xpp/Event.hxx>

namespace xpp {

/// Wrapper around the XSelectionRequestEvent type.
class SelectionRequestEvent {
public: // functions

	explicit SelectionRequestEvent(Event &ev) :
			m_ev{ev.toSelectionRequest()} {}

	/// Returns the atom corresponding to the property where the selection is stored on
	AtomID property() const { return AtomID{m_ev.property}; }

	/// Returns the target conversion type
	AtomID target() const { return AtomID{m_ev.target}; }

	/// Returns the selection this is about (primary, clipboard, ...)
	AtomID selection() const { return AtomID{m_ev.selection}; }

	/// Returns the window ID that requested the selection
	WinID requestor() const { return WinID{m_ev.requestor}; }

	Time time() const { return m_ev.time; }
protected: // data
	XSelectionRequestEvent &m_ev;
};

} // end ns
