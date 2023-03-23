#ifndef XPP_FORMATTING_HXX
#define XPP_FORMATTING_HXX

// C++
#include <iosfwd>

// X++
#include "X++/dso_export.h"
#include "X++/fwd.hxx"
#include "X++/types.hxx"

XPP_API std::ostream& operator<<(std::ostream &o, const xpp::utf8_string &s);

/// Output operator that prints the human readable name of an atom.
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::AtomID atom);

/// output operator that prints out the X11 window ID associated with \c w onto the stream in hex and dec
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::XWindow &w);

#endif // inc. guard
