// X++
#include "X++/CachedAtom.hxx"
#include "X++/AtomMapper.hxx"

namespace xpp {

void CachedAtom::resolve() const {
	auto &mapper = AtomMapper::getInstance();
	m_id = mapper.getAtom(m_name);
}

} // end ns
