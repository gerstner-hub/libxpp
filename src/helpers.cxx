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
	ss << xpp::raw_win(id);

	return ss.str();
}

} // end ns
