// xpp
#include <xpp/AtomMapper.hxx>
#include <xpp/CachedAtom.hxx>

namespace xpp {

void CachedAtom::resolve() const {
	m_id = atom_mapper.mapAtom(m_name);
}

} // end ns
