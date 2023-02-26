#ifndef XPP_ATOMMAPPER_HXX
#define XPP_ATOMMAPPER_HXX

// C++
#include <map>
#include <string>
#include <stdint.h>
#include <iosfwd>

// cosmos
#include "cosmos/thread/RWLock.hxx"

// X++
#include "X++/dso_export.h"
#include "X++/types.hxx"
#include "X++/X11Exception.hxx"

namespace xpp {

/// Caching of Property name/type to AtomID Mappings.
/**
 * This class allows to cache Xlib atom mappings that can be quickly retrieved
 * in the future. If a mapping is not cached already then it is retrieved via
 * Xlib.
 * 
 * This class is thread safe by means of a read-write lock. Read-access can
 * occur in parallel, write accesses (due to cache misses) are exclusive.
 * 
 * This class is implemented as a singleton type. Get the global instance of
 * the mapper via the getInstance() function.
 *
 * \note
 *
 *  There are Xlib functions that allow to map strings to atoms and vice
 *  versa. However I'm not sure how efficient that mapping is done. One
 *  should assume that any mappings established should be efficiently cached
 *  locally within Xlib. Thus when we request the same mapping again the
 *  lookup will be very fast.
 *
 *  I decided to provide an own caching facility, however, to be on the safe
 *  side. Also the interface of this class allows for more C++-like usage.
 **/
class XPP_API AtomMapper {
public: // functions

	/// Retrieves the global mapper instance
	static AtomMapper& getInstance();

	/// Returns the atom representation of the given property name
	/**
	 * The function performs caching of resolved atom values to avoid
	 * having to excessively talk to the XServer
	 **/
	AtomID getAtom(const std::string_view s) const;

	/// tries to do a reverse lookup to get the name of \c atom
	const std::string& getName(const AtomID atom) const;

protected: // functions

	/// protected constructor to enforce singleton usage
	AtomMapper() {};
	AtomMapper(const AtomMapper&) = delete;

	const std::string& cacheMiss(const AtomID atom) const;
	AtomID cacheMiss(const std::string_view s) const;

protected: // types

	/// The map container that maps strings to X atoms
	using AtomMapping = std::map<std::string, AtomID>;

protected: // data

	/// contains the actual cached mappings
	mutable AtomMapping m_mappings;
	/// synchronizes parallel read and update of \c m_mappings
	cosmos::RWLock m_mappings_lock;
};

} // end ns

/// Output operator that prints the human readable name of an atom.
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::AtomID atom);

#endif // inc. guard
