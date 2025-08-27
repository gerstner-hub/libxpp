#pragma once

// C++
#include <iosfwd>

// xpp
#include <xpp/dso_export.h>
#include <xpp/fwd.hxx>
#include <xpp/types.hxx>

/**
 * @file
 *
 * A place for declaring output operators for libxpp basic types.
 **/

XPP_API std::ostream& operator<<(std::ostream &o, const xpp::utf8_string &s);

/// Output operator that prints the human readable name of an atom.
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::AtomID atom);

/// output operator that prints out the X11 window ID associated with `w` onto the stream in hex and dec
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::XWindow &w);

/// output the window ID as a regular integer.
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::WinID id);
