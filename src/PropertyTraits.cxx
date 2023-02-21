// X++
#include "X++/PropertyTraits.hxx"

namespace xpp {

XAtom PropertyTraits<utf8_string>::x_type = XAtom{0};
XAtom PropertyTraits<std::vector<utf8_string>>::x_type = XAtom{0};

} // end ns
