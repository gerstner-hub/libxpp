#pragma once

// C++
#include <string_view>

// Cosmos
#include "cosmos/SysString.hxx"

// X11
#include <X11/Xutil.h>

namespace xpp {

/// Wrapper around the XClassHint structure.
class ClassHints :
		public XClassHint {
public: // functions

	ClassHints() {
		reset();
	}

	ClassHints(const cosmos::SysString name, const cosmos::SysString clazz) {
		this->res_name = const_cast<char*>(name.raw());
		this->res_class = const_cast<char*>(clazz.raw());
	}

	std::string_view name() const { return this->res_name; }
	std::string_view clazz() const { return this->res_class; }

	void reset() {
		this->res_name = nullptr;
		this->res_class = nullptr;
	}
};

} // end ns
