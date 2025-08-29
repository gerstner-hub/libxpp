#pragma once

// C++
#include <optional>

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

	/// Either the reconfigured window or its parent.
	/**
	 * Which window this refers to depends on whether StructureNotify or
	 * SubStructureNotify was used.
	 **/
	WinID eventWin() const {
		return WinID{m_ev.event};
	}

	/// The window which changed whose configuration changed.
	/**
	 * This override AnyEvent::window() which refers to the eventWin()
	 * instead.
	 **/
	WinID window() const {
		return WinID{m_ev.window};
	}

	/// Returns the sibling window for stacking operations.
	/**
	 * If this is not available then the affected window is at the bottom
	 * of the stack. Otherwise the affected window is on top (above) the
	 * returned window.
	 **/
	std::optional<WinID> aboveWin() const {
		if (m_ev.above == None)
			return {};

		return WinID{m_ev.above};
	}

	/// Event should be overriden.
	bool overrideRedirect() const {
		return m_ev.override_redirect;
	}

protected: // data
	const XConfigureEvent &m_ev;
};

} // end ns
