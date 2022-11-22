// cosmos
#include "cosmos/errors/RuntimeError.hxx"

// Xpp
#include "X++/Event.hxx"

namespace xpp {

void Event::raiseMismatch() {
	cosmos_throw( cosmos::RuntimeError("XEvent type access mismatch") );
}

} // end ns
