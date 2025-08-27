#pragma once

// xpp
#include <xpp/Event.hxx>
#include <xpp/types.hxx>

namespace xpp {

/// Wrapper around the XCreateWindowEvent type.
class CreateEvent {
public: // functions

	explicit CreateEvent(const Event &ev) :
		m_ev{ev.toCreateNotify()} {
	}

	WinID window() const {
		return WinID{m_ev.window};
	}

	/// Parent of the window which was newly created.
	WinID parent() const {
		return WinID{m_ev.parent};
	}

	/// Window size and location.
	WindowSpec spec() const {
		return WindowSpec{m_ev.x, m_ev.y,
			m_ev.width >= 0 ? static_cast<unsigned int>(m_ev.width) : 0,
			m_ev.height >= 0 ? static_cast<unsigned int>(m_ev.height) : 0};
	}

	/// Window border width.
	int borderWidth() const {
		return m_ev.border_width;
	}

	/// Creation should be overriden.
	bool overrideRedirect() const {
		return m_ev.override_redirect != False;
	}

protected: // data

	const XCreateWindowEvent &m_ev;
};

} // end ns
