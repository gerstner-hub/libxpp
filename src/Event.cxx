// cosmos
#include "cosmos/error/RuntimeError.hxx"

// X++
#include "X++/Event.hxx"

namespace xpp {

void Event::raiseMismatch() const {
	cosmos_throw (cosmos::RuntimeError("XEvent type access mismatch"));
}

} // end ns
