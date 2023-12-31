#pragma once

// X11
#include <X11/Xlib.h>

namespace xpp {

/// C++ wrapper for the X11 XColor struct
class XColor :
		public ::XColor {
public: // functions

	/// Sets all three RGB components to the given value
	void setAll(unsigned short val) {
		this->red = val;
		this->green = val;
		this->blue = val;
	}
};

} // end ns
