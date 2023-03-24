#ifndef XPP_CLIENTMESSAGEEEVENT_HXX
#define XPP_CLIENTMESSAGEEEVENT_HXX

// X++
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

	auto format() const { return m_ev.format; }

	/// returns the raw event message data
	const auto& data() const { return m_ev.data; }

protected: // data

	const XClientMessageEvent &m_ev;
};

} // end ns

#endif // inc. guard

