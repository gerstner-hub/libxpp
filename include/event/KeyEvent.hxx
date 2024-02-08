#pragma once

// xpp
#include "xpp/event/InputEvent.hxx"

namespace xpp {

/// Wrapper around the XKeyEvent structure.
class KeyEvent :
		public InputEvent<XKeyEvent> {
public: // functions

	explicit KeyEvent(const Event &ev) :
			InputEvent{ev.toKeyEvent()} {}
};

} // end ns
