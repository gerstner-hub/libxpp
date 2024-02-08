#pragma once

// C++
#include <map>
#include <string>
#include <stdint.h>

// cosmos
#include "cosmos/thread/RWLock.hxx"

// xpp
#include "xpp/dso_export.h"
#include "xpp/types.hxx"

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
 * There is a global instance of this type `xpp::atom_mapper` that should be
 * used for centralized access to atom mapping features.
 *
 * \note
 *
 *  I'm not sure about the efficiency of the Xlib functions that map strings
 *  to atoms and vice versa. I decided to provide this dedicated caching
 *  facility, to be on the safe side. Also the interface of this class allows
 *  for more C++-like usage.
 **/
class XPP_API AtomMapper {
public: // functions

	/// Returns the atom representation of the given property name
	/**
	 * The function performs caching of resolved atom values to avoid
	 * having to excessively talk to the XServer
	 **/
	AtomID mapAtom(const std::string_view s) const;

	/// tries to do a reverse lookup to get the name of `atom`
	const std::string& mapName(const AtomID atom) const;

	AtomMapper() = default;
	// the cache should not be copied for performance reasons
	AtomMapper(const AtomMapper&) = delete;

protected: // functions

	const std::string& cacheMiss(const AtomID atom) const;
	AtomID cacheMiss(const std::string_view s) const;

protected: // types

	/// The map container that maps strings to X atoms
	using AtomMapping = std::map<std::string, AtomID>;

protected: // data

	/// contains the actual cached mappings
	mutable AtomMapping m_mappings;
	/// synchronizes parallel read and update of `m_mappings`
	cosmos::RWLock m_mappings_lock;
};

/// Global AtomMapper instance in libxpp for centralized caching.
extern XPP_API AtomMapper atom_mapper;

} // end ns
