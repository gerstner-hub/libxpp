#ifndef XPP_TYPES_HXX
#define XPP_TYPES_HXX

/* a place for miscellaneous types used in Xpp interfaces */

namespace xpp {

/// represents a window location and dimension
struct WindowSpec {
	int x = 0;
	int y = 0;
	unsigned int width = 0;
	unsigned int height = 0;
};

} // end ns

#endif // inc. guard
