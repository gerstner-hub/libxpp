// C++
#include <sstream>
#include <string>

// cosmos
#include <cosmos/utils.hxx>

// xpp
#include <xpp/helpers.hxx>

namespace xpp {

std::string to_string(const WinID id) {
	std::stringstream ss;
	ss << xpp::raw_win(id);

	return ss.str();
}

} // end ns
