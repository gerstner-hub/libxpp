#ifndef XPP_CLASSHINTS_HXX
#define XPP_CLASSHINTS_HXX

// C++
#include <string_view>

// X11
#include <X11/Xutil.h>

namespace xpp {

/// Wrapper around the XClassHint structure.
struct ClassHints :
		public XClassHint {
public: // functions

	ClassHints() {
		reset();
	}

	ClassHints(const std::string_view name, const std::string_view clazz) {
		this->res_name = const_cast<char*>(name.data());
		this->res_class = const_cast<char*>(clazz.data());
	}

	std::string_view name() const { return this->res_name; }
	std::string_view clazz() const { return this->res_class; }

	void reset() {
		this->res_name = nullptr;
		this->res_class = nullptr;
	}
};

} // end ns

#endif // inc. guard
