// C++
#include <cstdlib>
#include <iostream>

// cosmos
#include "cosmos/thread/RWLock.hxx"

// X++
#include "X++/AtomMapper.hxx"
#include "X++/XDisplay.hxx"
#include "X++/private/Xpp.hxx"

namespace xpp {

AtomMapper atom_mapper;

AtomID AtomMapper::getAtom(const std::string_view s) const {
	{
		cosmos::ReadLockGuard g{m_mappings_lock};

		if (auto it = m_mappings.find(std::string{s}); it != m_mappings.end()) {
			return AtomID{it->second};
		}
	}

	return cacheMiss(s);
}

const std::string& AtomMapper::getName(const AtomID atom) const {
	{
		cosmos::ReadLockGuard g{m_mappings_lock};

		for (const auto &pair: m_mappings) {
			if (pair.second == atom) {
				return pair.first;
			}
		}
	}

	return cacheMiss(atom);
}

const std::string& AtomMapper::cacheMiss(const AtomID atom) const {
	const auto name = XDisplay::getInstance().getName(atom);

	{
		cosmos::WriteLockGuard g(m_mappings_lock);
		auto ret = m_mappings.insert(std::make_pair(name, atom));

		return (ret.first)->first;
	}
}

AtomID AtomMapper::cacheMiss(const std::string_view s) const {
	auto &logger = Xpp::getLogger();
	AtomID ret{XDisplay::getInstance().getAtom(s)};

	logger.debug() << "Resolved atom id for '" << s << "' is " << raw_atom(ret) << std::endl;

	{
		cosmos::WriteLockGuard g{m_mappings_lock};
		m_mappings.insert(std::make_pair(s, ret));
	}

	return ret;
}

} // end ns

std::ostream& operator<<(std::ostream &o, const xpp::AtomID atom) {
	o << xpp::raw_atom(atom) << " (" << xpp::atom_mapper.getName(atom) << ")";

	return o;
}
