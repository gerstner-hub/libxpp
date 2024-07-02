// libX11
#include <X11/XKBlib.h>

// xpp
#include <xpp/helpers.hxx>
#include <xpp/keyboard.hxx>
#include <xpp/XWindow.hxx>
#include <xpp/XDisplay.hxx>

namespace xpp {

bool ring_bell(const XWindow &win,
		const BellVolume volume,
		const std::optional<AtomID> name,
		std::optional<XDisplay*> p_display) {

	auto res = ::XkbBell(
		p_display ? **p_display : xpp::display,
		xpp::raw_win(win.id()),
		cosmos::to_integral(volume),
		name ? raw_atom(*name) : None
	);

	return res ? true : false;
}

std::optional<KeySymID> string_to_keysym(const cosmos::SysString str) {
	auto res = ::XStringToKeysym(str.raw());

	if (res == NoSymbol) {
		return std::nullopt;
	}

	return KeySymID{res};
}

std::optional<InputModifier> string_to_input_mod(const cosmos::SysString str) {
	const auto label = cosmos::to_upper(str.str());

	if (label == "SHIFT")
		return InputModifier::SHIFT;
	if (label == "LOCK")
		return InputModifier::LOCK;
	if (label == "CONTROL")
		return InputModifier::CONTROL;
	if (label == "MOD1")
		return InputModifier::MOD1;
	if (label == "MOD2")
		return InputModifier::MOD2;
	if (label == "MOD3")
		return InputModifier::MOD3;
	if (label == "MOD4")
		return InputModifier::MOD4;
	if (label == "MOD5")
		return InputModifier::MOD5;
	if (label == "BUTTON1")
		return InputModifier::BUTTON1;
	if (label == "BUTTON2")
		return InputModifier::BUTTON2;
	if (label == "BUTTON3")
		return InputModifier::BUTTON3;
	if (label == "BUTTON4")
		return InputModifier::BUTTON4;
	if (label == "BUTTON5")
		return InputModifier::BUTTON5;
	if (label == "NONE")
		return InputModifier::NONE;

	return std::nullopt;
}

} // end ns
