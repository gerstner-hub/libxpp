#ifndef XPP_POINTERMOVEDEVENT_HXX
#define XPP_POINTERMOVEDEVENT_HXX

// X++
#include "X++/event/InputEvent.hxx"

namespace xpp {

class PointerMovedEvent :
		public InputEvent<XPointerMovedEvent> {
public: // functions

	explicit PointerMovedEvent(const Event &ev) :
			InputEvent{ev.toPointerMovedEvent()} {}
};

} // end ns

#endif // inc. guard
