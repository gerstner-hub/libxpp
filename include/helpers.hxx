#ifndef XPP_HELPERS_HXX
#define XPP_HELPERS_HXX

// stdlib
#include <memory>
#include <exception>

// libX11
#include <X11/Xlib.h>

namespace xpp {

/// returns a shared_ptr object that calls XFree on cleanup for XLib memory
/**
 * Additionally if a nullptr is passed in (XLib allocation error) then a
 * std::bad_alloc() exception is thrown.
 **/
template <typename T>
auto make_shared_xptr(T *ptr) {
	if (!ptr) {
		throw std::bad_alloc();
	}
	return std::shared_ptr<T>(ptr, [](T *dp) { XFree(dp); });
}

} // end ns

#endif // inc. guard
