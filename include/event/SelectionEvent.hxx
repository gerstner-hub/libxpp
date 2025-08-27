#pragma once

// xpp
#include <xpp/event/AnyEvent.hxx>

namespace xpp {

/// Wrapper around the XSelectionEvent type.
class SelectionEvent :
		public AnyEvent {
public: // functions

	explicit SelectionEvent(const Event &ev) :
			AnyEvent{ev.toAnyEvent()},
			m_ev{ev.toSelectionNotify()} {
	}

	/// Returns the atom corresponding to the property where the selection is stored on
	AtomID property() const { return AtomID{m_ev.property}; }

	/// Returns the target conversion type
	AtomID target() const { return AtomID{m_ev.target}; }

	/// Returns the selection this is about (primary, clipboard, ...)
	AtomID selection() const { return AtomID{m_ev.selection}; }

	/// Returns the window ID that requested the selection
	WinID requestor() const { return WinID{m_ev.requestor}; }

	Time time() const { return m_ev.time; }

protected: // functions

	explicit SelectionEvent(const XSelectionEvent &ev) :
		AnyEvent{reinterpret_cast<const XAnyEvent&>(ev)},
		m_ev{ev} {
	}

protected: // data

	const XSelectionEvent &m_ev;
};

/// Wrapper around the XSelectionEvent type with data modification support.
class SelectionEventBuilder :
		public SelectionEvent {
public: // functions

	SelectionEventBuilder(Event &ev) :
		SelectionEvent{ev.toSelectionNotify()},
		m_ev{ev.toSelectionNotify()} {
	}

	void setProperty(const AtomID property) {
		m_ev.property = raw_atom(property);
	}

	void setTarget(const AtomID target) {
		m_ev.target = raw_atom(target);
	}

	void setSelection(const AtomID id) {
		m_ev.selection = raw_atom(id);
	}

	void setRequestor(const WinID id) {
		m_ev.requestor = raw_win(id);
	}

	void setTime(const Time t) {
		m_ev.time = t;
	}

protected: // data
	XSelectionEvent &m_ev;
};

} // end ns
