// X++
#include "X++/CachedAtom.hxx"
#include "X++/AtomMapper.hxx"

namespace xpp {

void CachedAtom::resolve() const {
	m_id = atom_mapper.mapAtom(m_name);
}

} // end ns
