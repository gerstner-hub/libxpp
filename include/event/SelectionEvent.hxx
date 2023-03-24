#ifndef XPP_SELECTIONEVENT_HXX
#define XPP_SELECTIONEVENT_HXX

// X++
#include "X++/Event.hxx"

namespace xpp {

/// Wrapper around the XSelectionEvent type.
class SelectionEvent {
public: // functions

	explicit SelectionEvent(const Event &ev) :
		m_ev{ev.toSelectionNotify()} {}

	/// Returns the atom corresponding to the property where the selection is stored on
	AtomID property() const { return AtomID{m_ev.property}; }

	/// Returns the target conversion type
	AtomID target() const { return AtomID{m_ev.target}; }

	/// Returns the selection this is about (primary, clipboard, ...)
	AtomID selection() const { return AtomID{m_ev.selection}; }

protected: // data
	const XSelectionEvent &m_ev;
};

} // end ns

#endif // inc. guard
