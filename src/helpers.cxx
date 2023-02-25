// C++
#include <sstream>
#include <string>

// cosmos
#include "cosmos/algs.hxx"

// X++
#include "X++/helpers.hxx"

namespace xpp {

std::string to_string(const WinID id) {
	std::stringstream ss;
	ss << cosmos::to_integral(id);

	return ss.str();
}

} // end ns
