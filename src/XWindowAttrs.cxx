// xpp
#include <xpp/XWindowAttrs.hxx>

namespace xpp {

bool XWindowAttrs::isMapped() const {
	return map_state != IsUnmapped;
}

} // end ns
