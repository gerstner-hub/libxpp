#ifndef XPP_CONFIGUREEVENT_HXX
#define XPP_CONFIGUREEVENT_HXX

// X++
#include "X++/Event.hxx"

namespace xpp {

/// Wrapper around the XConfigureEvent type.
class ConfigureEvent {
public: // functions

	explicit ConfigureEvent(const Event &ev) :
		m_ev{ev.toConfigureNotify()} {}

	/// Returns the inner size of the window not including borders.
	Extent extent() const {
		return Extent{
			 m_ev.width > 0 ? static_cast<unsigned int>(m_ev.width)  : 0,
			m_ev.height > 0 ? static_cast<unsigned int>(m_ev.height) : 0
		};
	}

protected: // data
	const XConfigureEvent &m_ev;
};

} // end ns

#endif // inc. guard
