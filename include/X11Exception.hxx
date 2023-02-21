#ifndef XPP_X11EXCEPTION_HXX
#define XPP_X11EXCEPTION_HXX

// C++
#include <string_view>

// main xlib header
#include <X11/Xlib.h>

// cosmos
#include "cosmos/error/CosmosError.hxx"

namespace xpp {

/// Exception type for X11 errors.
struct X11Exception :
		public cosmos::CosmosError {

	/// libX11 error text constructor.
	/**
	 * This constructor takes an libX11 int return code for which an error
	 * description is produced within the class. The related Display
	 * instance is also necessary for this.
	 **/
	X11Exception(Display *dis, const int errcode) :
			CosmosError("X11Exception") {
		char errtext[128];
		(void)XGetErrorText(dis, errcode, errtext, sizeof(errtext));
		m_msg += errtext;
	}

	/// libX11 custom error text constructor.
	/**
	 * This constructor is for situations when there is no proper libX11
	 * error code. It only takes a custom error description to be provided
	 * by the caller.
	 **/
	explicit X11Exception(const std::string_view &problem) :
			CosmosError("X11Exception", problem) {
	}

	COSMOS_ERROR_IMPL;
};

} // end ns

#endif // inc. guard
