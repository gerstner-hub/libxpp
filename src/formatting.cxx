// C++
#include <ostream>

// cosmos
#include <cosmos/formatting.hxx>

// xpp
#include <xpp/AtomMapper.hxx>
#include <xpp/formatting.hxx>
#include <xpp/utf8_string.hxx>
#include <xpp/XWindow.hxx>

std::ostream& operator<<(std::ostream &o, const xpp::utf8_string &s) {
	o << s.str;
	return o;
}

std::ostream& operator<<(std::ostream &o, const xpp::AtomID atom) {
	o << xpp::raw_atom(atom) << " (" << xpp::atom_mapper.mapName(atom) << ")";

	return o;
}

std::ostream& operator<<(std::ostream &o, const xpp::XWindow &w) {
	auto raw = xpp::raw_win(w.id());
	auto num = cosmos::HexNum(raw, 8);
	o << num << " (" << raw << ")";

	return o;
}
