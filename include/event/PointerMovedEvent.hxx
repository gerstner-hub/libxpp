#pragma once

// xpp
#include "xpp/event/InputEvent.hxx"

namespace xpp {

class PointerMovedEvent :
		public InputEvent<XPointerMovedEvent> {
public: // functions

	explicit PointerMovedEvent(const Event &ev) :
			InputEvent{ev.toPointerMovedEvent()} {}
};

} // end ns
