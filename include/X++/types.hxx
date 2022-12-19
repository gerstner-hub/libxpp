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

/// A two-dimensional extent in pixels (e.g. for creating pixmaps)
struct Extent {
	unsigned int width = 0;
	unsigned int height = 0;
};

struct PixMap {
	PixMap() {}
	explicit PixMap(Pixmap pm) : m_valid(true), m_pixmap(pm) {}
	operator Pixmap() const {
		return id();
	}
	Pixmap id() const { return m_pixmap; }
	void reset() { m_valid = false; }
	bool valid() const { return m_valid; }
protected:
	bool m_valid = false;
	Pixmap m_pixmap = -1;
};

} // end ns

#endif // inc. guard
