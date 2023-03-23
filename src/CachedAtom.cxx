// X++
#include "X++/AtomMapper.hxx"
#include "X++/CachedAtom.hxx"

namespace xpp {

void CachedAtom::resolve() const {
	m_id = atom_mapper.mapAtom(m_name);
}

} // end ns
