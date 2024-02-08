#pragma once

// C++
#include <memory>
#include <exception>

// libX11
#include <X11/Xlib.h>

// cosmos
#include "cosmos/utils.hxx"

// xpp
#include "xpp/dso_export.h"
#include "xpp/types.hxx"

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

/// returns a string representation of the given Window ID
std::string XPP_API to_string(const WinID id);

inline DrawableID to_drawable(PixmapID id) {
	return DrawableID{cosmos::to_integral(id)};
}

inline DrawableID to_drawable(WinID id) {
	return DrawableID{cosmos::to_integral(id)};
}

inline auto raw_atom    = cosmos::to_integral<AtomID>;
inline auto raw_win     = cosmos::to_integral<WinID>;
inline auto raw_pixmap  = cosmos::to_integral<PixmapID>;
inline auto raw_screen  = cosmos::to_integral<ScreenID>;
inline auto raw_cmap    = cosmos::to_integral<ColormapID>;
inline auto raw_gravity = cosmos::to_integral<Gravity>;
inline auto raw_button  = cosmos::to_integral<Button>;

} // end ns
