#pragma once

// xpp
#include <xpp/Event.hxx>

namespace xpp {

/// Wrapper around the XSelectionClearEvent type.
class SelectionClearEvent {
public: // functions

	explicit SelectionClearEvent(const Event &ev) :
			m_ev{ev.toSelectionClear()} {}

	/// Returns the selection this is about (primary, clipboard, ...)
	AtomID selection() const { return AtomID{m_ev.selection}; }

	/// Returns the window that currently owns the selection (and now loses it).
	WinID owner() const { return WinID{m_ev.window}; }

	/// Last change time recorded for the selection.
	Time time() const { return m_ev.time; }

protected: // data
	const XSelectionClearEvent &m_ev;
};

} // end ns

