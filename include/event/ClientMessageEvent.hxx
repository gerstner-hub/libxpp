#pragma once

// X++
#include "X++/atoms.hxx"
#include "X++/Event.hxx"

namespace xpp {

/// Wrapper around the XClientMessageEvent type
/**
 * A client message is not interpreted by the X server itself, but can contain
 * e.g. window manager related information. The type field contains an atom
 * that defines further content of the messsage.
 *
 * Examples for this are the XEMBED extension and the WM_PROTOCOLS extension.
 **/
class ClientMessageEvent {
public: // functions

	explicit ClientMessageEvent(const Event &ev) :
		m_ev{ev.toClientMessage()} {}

	/// returns the actual client message type
	AtomID type() const { return AtomID(m_ev.message_type); }

	void setType(const AtomID type) { m_ev.message_type = cosmos::to_integral(type); }

	auto format() const { return m_ev.format; }

	void setFormat(int format) {
		m_ev.format = format;
	}

	/// returns the raw event message data
	const auto& data() const { return m_ev.data; }

	void setDisplay(xpp::XDisplay &p_display) {
		m_ev.display = p_display;
	}

	void setWinID(const WinID id) {
		m_ev.window = cosmos::to_integral(id);
	}

protected: // data

	XClientMessageEvent &m_ev;
};

/// A ClientMessageEvent for notifying the window manager about window state.
/**
 * This event can contain up to two properties that are set, removed or
 * toggled depending on the value passeed in setOperation().
 **/
class NetWmStateEvent :
	public ClientMessageEvent {
public: // functions

	explicit NetWmStateEvent(const Event &ev) :
			ClientMessageEvent{ev} {
		setType(atoms::ewmh_wm_state);
		setFormat(32);
	}

	void setOperation(const NetWmStateAction action) {
		m_ev.data.l[0] = cosmos::to_integral(action);
	}

	void setProperties(const AtomID first, const AtomID second = AtomID::INVALID) {
		m_ev.data.l[1] = cosmos::to_integral(first);
		m_ev.data.l[2] = second == AtomID::INVALID ? 0 : cosmos::to_integral(second);
	}
};

} // end ns
