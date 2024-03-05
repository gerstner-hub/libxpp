// C++
#include <iostream>

// cosmos
#include <cosmos/cosmos.hxx>
#include <cosmos/io/StdLogger.hxx>

// xpp
#include <xpp/AtomMapper.hxx>
#include <xpp/atoms.hxx>
#include <xpp/CachedAtom.hxx>
#include <xpp/helpers.hxx>
#include <xpp/Xpp.hxx>

constexpr xpp::CachedAtom window_name{"_NET_WM_NAME"};

int main() {
	cosmos::Init cosmos_init;
	cosmos::StdLogger logger;
	xpp::Init init(&logger);

	auto id = static_cast<xpp::AtomID>(window_name);

	std::cout << window_name.name() << " = " << xpp::raw_atom(id) << "\n";
	const auto &client_machine = xpp::atoms::icccm_client_machine;
	std::cout << client_machine.name() << " = " << xpp::raw_atom(client_machine) << "\n";

	return 0;
}
