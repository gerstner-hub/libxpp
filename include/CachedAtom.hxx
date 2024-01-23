#pragma once

// C++
#include <optional>
#include <string_view>

// X++
#include "X++/dso_export.h"
#include "X++/types.hxx"

namespace xpp {

/// Transparently obtained and cached AtomID
/**
 * For Atom values that need to be looked up during runtime, mapping them
 * on-the-fly from string to AtomID in application code can bloat the code,
 * even if AtomMapper is used for efficient caching.
 *
 * For storing globally accessible AtomID with const-semantics this type can
 * be used. It transparently casts to AtomID, obtains the mapping on first
 * access and is fully type safe.
 *
 * Since the atom is resolved during runtime the conversion operation can
 * throw.
 *
 * Instances of this type should be declared constexpr to avoid non-literal
 * constants being used in its construction.
 **/
class XPP_API CachedAtom {
public: // functions
	/// Prepare a CachedAtom for the given literal string
	/**
	 * TODO: Once we have C++20 use `consteval` here to prevent
	 * non-literal strings being used.
	 **/
	explicit constexpr CachedAtom(std::string_view sv) :
			m_name{sv} {
	}

	/// Create a CachedAtom for a literal AtomID, no resolving will take place
	explicit constexpr CachedAtom(const AtomID id) :
			m_id{id}
	{}

	operator AtomID() const {
		return atom();
	}

	AtomID atom() const {
		if (!m_id) {
			resolve();
		}

		return *m_id;
	}

	constexpr std::string_view name() const { return m_name; }

protected: // functions

	void resolve() const;

protected: // data
	std::string_view m_name;
	mutable std::optional<AtomID> m_id;
};

} // end ns
