// C++
#include <iostream>

// cosmos
#include <cosmos/cosmos.hxx>

// Xpp
#include <xpp/keyboard.hxx>
#include <xpp/Xpp.hxx>

int main() {
	cosmos::Init cosmos_init;
	xpp::Init init;

	if (auto sym_opt = xpp::string_to_keysym("F12"); sym_opt != std::nullopt) {
		if (*sym_opt != xpp::KeySymID::F12) {
			std::cerr << "Bad translation of F12 into KeySymID!" << std::endl;
			return 1;
		} else {
			std::cout << "Successfully translated KeySymID" << std::endl;
		}
	} else {
		std::cerr << "couldn't translate F12 into KeySymID!" << std::endl;
		return 1;
	}

	if (auto mod_opt = xpp::string_to_input_mod("mod4"); mod_opt != std::nullopt) {
		if (*mod_opt != xpp::InputModifier::MOD4) {
			std::cerr << "Bad translation of MOD4 into InputModifier!" << std::endl;
			return 1;
		} else {
			std::cout << "Successfully translated InputModifier" << std::endl;
		}
	} else {
		std::cerr << "couldn't translate MOD4 into InputModifier!" << std::endl;
		return 1;
	}

	return 0;
}
