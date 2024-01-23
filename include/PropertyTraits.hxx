#pragma once

// C++
#include <cstring>
#include <vector>

// X11
#include <X11/Xatom.h>

// X++
#include "X++/atoms.hxx"
#include "X++/utf8_string.hxx"

namespace xpp {

/// Type traits for X properties.
/**
 * This is a generic template class for getting and setting X properties.
 *
 * This generic template is not intended for use but only for explicit
 * template specialications for concrete types. This generic template merely
 * serves the purpose of documentation.
 *
 * The traits are to be used together with the Property class to smartly
 * send/receive data of a certain format to/from the X-server.
 *
 * The X11 protocol uses RPC type definitions that need to be specified at the
 * Xlib's C-interface by means of setting the right types, pointers, sizes
 * etc. This is pretty low-level and unattractive to use.
 *
 * These traits and the Property class help to make these operations more type
 * safe, better readable and less redundant.
 *
 * The approach is that the traits and Property types are instantiated using
 * the C or C++ types that are locally used in the program. Then when we need
 * to retrieve or send that type from/to the X server the Property class
 * together with the traits types know internally how to transform the local
 * data into data the X server understands and vice versa.
 **/
template <typename PROPTYPE>
class PropertyTraits {
public: // constants

	/// Xlib atom data type corresponding to PROPTYPE that is passed to the X functions for identification.
	static constexpr AtomID x_type = AtomID{XA_CARDINAL};
	/// if PROPTYPE has got a fixed size then this constant denotes that size in bytes, otherwise set to zero.
	static constexpr unsigned long FIXED_SIZE = 0;
	/// The format in X terms, determines the width of a single sequence
	/// item in bits (e.g. for arrays of 8-bit, 16-bit or 32-bit items).
	static constexpr char FORMAT = 0;
	/// A pointer to PROPTYPE that can be passed to the X11 functions for retrieving / passing data.
	using XPtrType = float*;

public: // functions

	/// Returns the number of elements the given property has in X terms
	static int numElements(const PROPTYPE &val) {
		(void)val;
		return 0;
	}

	/// Set the current value of the native PROPTYPE from the given raw X data.
	/**
	 * The contract of this function is as follows:
	 *
	 * The first parameter is an out parameter, a reference to PROPTYPE,
	 * where the function should store the new value for PROPTYPE.
	 *
	 * The new value for PROPTYPE is provided in the second parameter
	 * which is a pointer to the raw X data received. The third parameter
	 * determines the number of sequence items that can be found in the
	 * second parameter, if applicable.
	 **/
	static void x2native(PROPTYPE &i, XPtrType data, unsigned int count) {
		(void)i; (void)data; (void)count;
		// prevent the generic template from being instantiated
		static_assert(sizeof(PROPTYPE) < 0);
	}

	/// Transform the current value of the native PROPTYPE into raw X data
	static void native2x(const PROPTYPE &s, XPtrType &data) {
		(void)s; (void)data;
	}

	// never instantiate this type
	PropertyTraits() = delete;
};

template <>
class PropertyTraits<int> {
public: // constants

	static constexpr AtomID x_type = AtomID{XA_CARDINAL};
	static constexpr unsigned long FIXED_SIZE = sizeof(int);
	static constexpr char FORMAT = 32;
	using XPtrType = long*;

public: // functions

	static int numElements(const int&) { return 1; }


	static void x2native(int &i, const XPtrType data, unsigned int count) {
		i = static_cast<int>(*data);
		(void)count;
	}

	static void native2x(const int &s, XPtrType &data) {
		// We simply set the pointer to the PROPTYPE item as a flat copy.
		data = (XPtrType)&s;
	}
};

template <>
class PropertyTraits<AtomID> {
public: // constants

	static constexpr AtomID x_type = AtomID{XA_ATOM};
	static constexpr unsigned long FIXED_SIZE = sizeof(AtomID);
	static constexpr char FORMAT = 32;
	using XPtrType = long*;

public: // functions

	static int numElements(const AtomID&) { return 1; }


	static void x2native(AtomID &a, const XPtrType data, unsigned int count) {
		a = AtomID{static_cast<Atom>(*data)};
		(void)count;
	}

	static void native2x(const AtomID &a, XPtrType &data) {
		// We simply set the pointer to the PROPTYPE item as a flat copy.
		data = (XPtrType)&a;
	}
};

template <>
class PropertyTraits<const char*> {
public: // constants

	static constexpr AtomID x_type = AtomID{XA_STRING};
	static constexpr unsigned long FIXED_SIZE = 0;
	static constexpr char FORMAT = 8;
	using XPtrType = const char*;

public: // functions

	static void x2native(const char *&s, XPtrType data, unsigned int count) {
		(void)count;
		s = data;
	}

	static void native2x(const char *& s, XPtrType &data) {
		data = s;
	}

	static int numElements(const char* const s) {
		// strings in X are transferred without null terminator, the
		// library always provides a terminating null byte in
		// transferred data, however
		return s ? std::strlen(s) : 0;
	}
};

template <>
class PropertyTraits<WinID> {
public: // constants

	static constexpr AtomID x_type = AtomID{XA_WINDOW};
	static constexpr unsigned long FIXED_SIZE = sizeof(WinID);
	static constexpr char FORMAT = 32;
	using XPtrType = long*;

public: // functions

	static void x2native(WinID &w, XPtrType data, unsigned int count) {
		w = WinID{static_cast<Window>(*data)};
		(void)count;
	}
};

template <>
class PropertyTraits<utf8_string> {
public: // constants

	static AtomID x_type;
	static constexpr unsigned long FIXED_SIZE = 0;
	static constexpr char FORMAT = 8;
	using XPtrType = const char*;

public: // functions

	static void init() {
		// this XLib property type atom is not available as a constant
		// in the Xlib headers but needs to be queried at runtime.
		x_type = atoms::ewmh_utf8_string;
	}

	static void x2native(utf8_string &s, XPtrType data, unsigned int count) {
		s.str = data;
		(void)count;
	}

	static void native2x(const utf8_string &s, XPtrType &data) {
		data = s.str.data();
	}

	static int numElements(const utf8_string &s) {
		// I suppose this is just for the X protocol the number of
		// bytes, not the number unicode characters
		return s.str.size();
	}
};


template <typename ELEM>
/// property type specialization for vectors of primitives
class PropertyTraits<std::vector<ELEM>> {
public: // constants

	static constexpr AtomID x_type = PropertyTraits<ELEM>::x_type;
	static constexpr unsigned long FIXED_SIZE = 0;
	static constexpr char FORMAT = PropertyTraits<ELEM>::FORMAT;
	using XPtrType = typename PropertyTraits<ELEM>::XPtrType;

public: // functions

	static void x2native(std::vector<ELEM> &v, XPtrType data, unsigned int count) {
		for (unsigned int e = 0; e < count; e++) {
			v.push_back(ELEM(data[e]));
		}
	}
};

template <>
class PropertyTraits<std::vector<utf8_string>> {
public: // constants

	static AtomID x_type;
	static constexpr unsigned long FIXED_SIZE = 0;
	static constexpr char FORMAT = PropertyTraits<utf8_string>::FORMAT;
	using XPtrType = const char*;

public: // functions

	static void init() {
		// beware: correct order of runtime initialization is
		// necessary in xpp::early_init().
		x_type = PropertyTraits<utf8_string>::x_type;
	}

	static void x2native(std::vector<utf8_string> &v, XPtrType data, unsigned int count) {
		// we get a char* sequence of zero-terminated strings here
		v.clear();

		while (count != 0) {
			v.push_back(utf8_string(data));
			const auto consumed = v.back().length() + 1;
			count -= consumed;
			data += consumed;
		}
	}
};

template <>
/// property type specialization for arrays of Window identifiers
class PropertyTraits<std::vector<int>> {
public: // constants

	static constexpr AtomID x_type = AtomID{XA_CARDINAL};
	static constexpr unsigned long FIXED_SIZE = 0;
	static constexpr char FORMAT = 32;
	using XPtrType = long*;

public: // functions

	static void x2native(std::vector<int> &v, XPtrType data, unsigned int count) {
		v.clear();

		for (unsigned int e = 0; e < count; e++) {
			v.push_back(data[e]);
		}
	}
};

template <>
class PropertyTraits<std::vector<AtomID>> {
public: // constants

	static constexpr AtomID x_type = AtomID{XA_ATOM};
	static constexpr unsigned long FIXED_SIZE = 0;
	static constexpr char FORMAT = 32;
	using XPtrType = long*;

public: // functions

	static void x2native(std::vector<AtomID> &v, XPtrType data, unsigned int count) {
		for (unsigned int e = 0; e < count; e++) {
			v.push_back(AtomID{static_cast<Atom>(data[e])});
		}
	}
};

} // end ns
