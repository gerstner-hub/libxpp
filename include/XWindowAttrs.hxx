#ifndef XPP_ATTRIBUTES_HXX
#define XPP_ATTRIBUTES_HXX

// Xlib
#include <X11/Xlib.h>

namespace xpp {

class XWindowAttrs :
		public XWindowAttributes {
public: // functions
	bool isMapped() const;
};

} // end ns

#endif // inc. guard
