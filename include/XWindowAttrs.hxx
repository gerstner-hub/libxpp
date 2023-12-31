#pragma once

// Xlib
#include <X11/Xlib.h>

namespace xpp {

class XWindowAttrs :
		public XWindowAttributes {
public: // functions
	bool isMapped() const;
};

} // end ns
