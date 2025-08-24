// C++
#include <cstring>

// cosmos
#include <cosmos/error/RuntimeError.hxx>

// xpp
#include <xpp/Event.hxx>

namespace xpp {

void Event::clear() {
	std::memset(&m_ev, 0, sizeof(m_ev));
}

void Event::raiseMismatch() const {
	throw cosmos::RuntimeError{"XEvent type access mismatch"};
}

} // end ns
