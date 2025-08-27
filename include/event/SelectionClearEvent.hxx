#pragma once

// xpp
#include <xpp/event/AnyEvent.hxx>

namespace xpp {

/// Wrapper around the XSelectionClearEvent type.
class SelectionClearEvent :
		public AnyEvent {
public: // functions

	explicit SelectionClearEvent(const Event &ev) :
			AnyEvent{ev.toAnyEvent()},
			m_ev{ev.toSelectionClear()} {
	}

	/// Returns the selection this is about (primary, clipboard, ...)
	AtomID selection() const { return AtomID{m_ev.selection}; }

	/// Returns the window that currently owns the selection (and now loses it).
	WinID owner() const { return this->window(); }

	/// Last change time recorded for the selection.
	Time time() const { return m_ev.time; }

protected: // data

	const XSelectionClearEvent &m_ev;
};

} // end ns

