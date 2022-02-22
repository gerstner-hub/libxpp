#ifndef XPP_ATTRIBUTES_HXX
#define XPP_ATTRIBUTES_HXX

// Xlib
#include "X11/Xlib.h"

namespace xpp {

struct XWindowAttrs : public XWindowAttributes {
	bool isMapped() const;
};

} // end ns

#endif // inc. guard
