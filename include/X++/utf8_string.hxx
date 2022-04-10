#ifndef XPP_UTF8_STRING
#define XPP_UTF8_STRING

// C++
#include <ostream>
#include <string>
#include <string_view>

namespace xpp {

/**
 * \brief
 * 	A type used for differentiation between a plain ASCII string and an
 * 	Xlib utf8 string
 * \details
 * 	It is currently just a container for a char pointer.
 * \note
 *	The X type UTF8_STRING is a new type proposed in X.org but not yet
 *	really approved:
 *
 *	http://www.pps.jussieu.fr/~jch/software/UTF8_STRING/UTF8_STRING.text
 *
 *	UTF8_STRING returns 8-bit encoded UTF8-Data without a NULL-terminator.
 *
 *	This type is used by the EWMH standard, so we need to be able to deal
 *	with it.
 **/
struct utf8_string {
	utf8_string() {}
	explicit utf8_string(const std::string_view &s) : str(s) {}

	size_t length() { return str.length(); }

	std::string str;
};

} // end ns

inline std::ostream& operator<<(std::ostream &o, const xpp::utf8_string &s) {
	o << s.str;
	return o;
}

#endif // inc. guard
