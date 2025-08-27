#pragma once

// xpp
#include <xpp/event/AnyEvent.hxx>

namespace xpp {

/// Wrapper around the XConfigureEvent type.
class ConfigureEvent :
		public AnyEvent {
public: // functions

	explicit ConfigureEvent(const Event &ev) :
		AnyEvent{ev},
		m_ev{ev.toConfigureNotify()} {
	}

	/// Returns the position and inner size of the window not including borders.
	WindowSpec spec() const {
		return WindowSpec{
			m_ev.x,
			m_ev.y,
			m_ev.width > 0 ? static_cast<unsigned int>(m_ev.width)  : 0,
			m_ev.height > 0 ? static_cast<unsigned int>(m_ev.height) : 0
		};
	}

protected: // data
	const XConfigureEvent &m_ev;
};

} // end ns
