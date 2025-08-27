#pragma once

// xpp
#include <xpp/Event.hxx>
#include <xpp/types.hxx>

namespace xpp {

/// Base class for XMapEvent and XUnmapEvent wrappers.
template <typename EV>
class MapEventBase {
public: // functions

	explicit MapEventBase(const EV &ev) :
		m_ev{ev} {}

	EventType type() const { return EventType{m_ev.type}; }

	/// The destroyed window
	WinID window() const {
		return WinID{m_ev.window};
	}

	/// The window the event appeared on.
	/**
	 * This will be either the same as window() or its parent, in case of
	 * SubStructureNotify event reporting.
	 **/
	WinID eventWin() const {
		return WinID{m_ev.event};
	}

	/// Whether override-redirect is set for the event.
	bool overrideRedirect() const {
		return m_ev.override_redirect != False;
	}

protected: // data

	const EV &m_ev;
};

class MapEvent :
		public MapEventBase<XMapEvent> {
public:
	explicit MapEvent(const Event &ev) :
		MapEventBase{ev.toMapNotify()} {
	}
};

class UnmapEvent :
		public MapEventBase<XUnmapEvent> {
public:
	explicit UnmapEvent(const Event &ev) :
		MapEventBase{ev.toUnmapNotify()} {
	};
};

} // end ns
