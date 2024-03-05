#pragma once

// xpp
#include <xpp/Event.hxx>

namespace xpp {

/// Wrapper around the XSelectionEvent type.
class SelectionEvent {
public: // functions

	explicit SelectionEvent(Event &ev) :
			m_ev{ev.toSelectionNotify()} {}

	/// Returns the atom corresponding to the property where the selection is stored on
	AtomID property() const { return AtomID{m_ev.property}; }

	void setProperty(const AtomID property) {
		m_ev.property = raw_atom(property);
	}

	/// Returns the target conversion type
	AtomID target() const { return AtomID{m_ev.target}; }

	void setTarget(const AtomID target) {
		m_ev.target = raw_atom(target);
	}

	/// Returns the selection this is about (primary, clipboard, ...)
	AtomID selection() const { return AtomID{m_ev.selection}; }

	void setSelection(const AtomID id) {
		m_ev.selection = raw_atom(id);
	}

	/// Returns the window ID that requested the selection
	WinID requestor() const { return WinID{m_ev.requestor}; }

	void setRequestor(const WinID id) {
		m_ev.requestor = raw_win(id);
	}

	Time time() const { return m_ev.time; }

	void setTime(const Time t) {
		m_ev.time = t;
	}

protected: // data
	XSelectionEvent &m_ev;
};

} // end ns
