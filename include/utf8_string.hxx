#pragma once

// C++
#include <string>
#include <string_view>

namespace xpp {

/// A type used for differentiation between a plain ASCII string and an Xlib utf8 string.
/**
 * This is just a container for a string_view. You need to be carefully
 * managing the lifetime validity of the contained data. This type should
 * only be used in conjunction with the Property type.
 *
 * \note
 * The X type UTF8_STRING is a new type proposed in X.org but not yet really
 * approved:
 *
 * http://www.pps.jussieu.fr/~jch/software/UTF8_STRING/UTF8_STRING.text
 *
 * UTF8_STRING returns 8-bit encoded UTF8-Data without a NULL-terminator.
 *
 * This type is used by the EWMH standard, so we need to be able to deal with
 * it.
 **/
struct utf8_string {
	utf8_string() {}
	explicit utf8_string(const std::string_view s) :
			str(s) {}

	size_t length() { return str.length(); }

	std::string_view str;
};

} // end ns
