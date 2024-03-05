// xpp
#include <xpp/PropertyTraits.hxx>

namespace xpp {

AtomID PropertyTraits<utf8_string>::x_type = AtomID::INVALID;
AtomID PropertyTraits<std::vector<utf8_string>>::x_type = AtomID::INVALID;

} // end ns
