#pragma once

// xpp
#include "xpp/event/InputEvent.hxx"

namespace xpp {

class ButtonEvent :
		public InputEvent<XButtonEvent> {
public: // functions

	explicit ButtonEvent(const Event &ev) :
			InputEvent{ev.toButtonEvent()} {}

	Button buttonNr() const { return Button{m_ev.button}; }
};

} // end ns
