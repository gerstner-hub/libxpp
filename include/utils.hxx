#ifndef XPP_UTILS_HXX
#define XPP_UTILS_HXX

// C++
#include <string_view>

// X11
#include <X11/Xutil.h> // for GeometrySettings constants

// Cosmos
#include "cosmos/BitMask.hxx"

// X++
#include "X++/types.hxx"

/**
 * @file
 *
 * A place for wrappers of global X library functions that aren't tied to the
 * Display type or other handles.
 **/

namespace xpp {

/// Special return bitmask values returned from XParseGeometry()
/**
 * These constants are preprocessor defines thus we need to avoid clashing
 * with their names and need to use alternative enum constants.
 **/
enum class GeometrySettings : int {
	HaveNone   = 0,           // this is not a bit position, use only == comparison
	HaveX      = XValue,      // an x value was specified
	HaveY      = YValue,      // a  y value was specified
	HaveWidth  = WidthValue,  // a width value is present
	HaveHeight = HeightValue, // a height value is present
	HaveAll    = AllValues,   // all values above are present
	NegativeX  = XNegative,   // the specified XVal is negative
	NegativeY  = YNegative    // the specified YVal is negative
};

using GeometrySettingsMask = cosmos::BitMask<GeometrySettings>;

/// Parses an X11 window size and placement string and returns the results.
/**
 * This parses a string of the following form:
 *
 * [=][<width>{xX}<height>][{+-}<xoffset>{+-}<yoffset>]
 *
 * The parsed values will be stored in \c geometry. This call does have any
 * explicit error reporting. On error none of the values will be set and an
 * empty mask is returned. The \c geometry will be reset to zero during this
 * call to prevent undefined data being used in case of errors.
 *
 * The returned bitmask indicates which of the values in \c geometry could be
 * parsed and whether negative X or Y values have been encountered.
 **/
inline GeometrySettingsMask parse_geometry(const std::string_view str, WindowSpec &geometry) {
	geometry = WindowSpec{};
	auto mask = XParseGeometry(str.data(), &geometry.x, &geometry.y, &geometry.width, &geometry.height);
	return GeometrySettingsMask{mask};
}

} // end ns

#endif // inc. guard
