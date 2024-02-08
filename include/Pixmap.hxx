#pragma once

// C++
#include <optional>

// xpp
#include "xpp/dso_export.h"
#include "xpp/fwd.hxx"
#include "xpp/types.hxx"

namespace xpp {

/// Represents an X11 Pixmap resource.
/**
 * This is a move only type, since there is a resource behind this object that
 * needs to be freed at the appropriate time.
 **/
class XPP_API Pixmap {
	Pixmap(const Pixmap&) = delete;
	Pixmap& operator=(const Pixmap&) = delete;
public: // functions

	Pixmap() :
			m_display{nullptr},
			m_id{PixmapID::INVALID} {
	}

	/// Creates a pixmap for the given window and of the given size.
	/**
	 * \param[in] depth the depth of the pixmap, if given, otherwise the
	 *            default depth for the display and window involved.
	 **/
	Pixmap(
		const WinID win,
		const Extent extent,
		const std::optional<int> depth = std::nullopt,
		XDisplay &disp = xpp::display);

	Pixmap(Pixmap &&o) {
		*this = std::move(o);
	}

	Pixmap& operator=(Pixmap &&o) {
		m_id = o.m_id;
		m_display = o.m_display;
		o.invalidate();
		return *this;
	}

	~Pixmap() {
		destroy();
	}

	bool valid() const {
		return m_id != PixmapID::INVALID;
	}

	void destroy();

	auto id() const { return m_id; }

	operator PixmapID() const {
		return m_id;
	}

protected: // functions

	void invalidate() {
		m_display = nullptr;
		m_id = PixmapID::INVALID;
	}

protected: // data
	XDisplay *m_display = nullptr;
	PixmapID m_id = PixmapID::INVALID;
};

} // end ns
