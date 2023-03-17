#ifndef XPP_KEYBOARD_HXX
#define XPP_KEYBOARD_HXX

// C++
#include <optional>

// X++
#include "X++/dso_export.h"
#include "X++/types.hxx"

namespace xpp {

class XDisplay;
class XWindow;

enum class BellVolume : int {
	MIN_VOL = -100,
	NONE    =    0,
	MAX_VOL =  100
};

/// Ring the keyboard bell.
/**
 * This call rings the bell in the context of the given window. If \c name is
 * given then this should be a string mapped to an atom that will be provided
 * to event consumers ("named bell").
 **/
bool XPP_API ring_bell(const XWindow &win,
		const BellVolume volume,
		const std::optional<AtomID> name = {},
		std::optional<XDisplay*> p_display = {});

} // end ns

#endif // inc. guard
