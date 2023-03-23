#ifndef XPP_SIZEHINTS_HXX
#define XPP_SIZEHINTS_HXX

// C++
#include <type_traits>

// libX11
#include <X11/Xutil.h>

// Cosmos
#include "cosmos/BitMask.hxx"
#include "cosmos/memory.hxx"

// X++
#include "X++/helpers.hxx"
#include "X++/types.hxx"

namespace xpp {

/// Wrapper around the XSizeHints structure.
/**
 * This adds helper types and wrappers to use strong types in conjunction with
 * XSizeHints.
 **/
struct SizeHints :
		public XSizeHints {
public: // types

	enum class Flags : long {
		UserPos        = USPosition, // x, y
		UserSize       = USSize,    // width, height
		ProgPos        = PPosition, 
		ProgSize       = PSize,
		ProgMinSize    = PMinSize,
		ProgMaxSize    = PMaxSize,
		ProgResizeIncrements = PResizeInc,
		ProgAspect     = PAspect, // min, max aspect ratios
		ProgBaseSize   = PBaseSize, // base size for incrementing
		ProgWinGravity = PWinGravity
	};

	using Mask = cosmos::BitMask<Flags>;

public: // functions

	void clear() {
		static_assert(std::is_pod<SizeHints>::value);
		cosmos::zero_object(*this);
	}

	void setFlags(const Mask hint_mask) {
		this->flags = hint_mask.raw();
	}

	void changeFlag(const Flags flag, const bool on_off) {
		if (on_off) {
			this->flags |= cosmos::to_integral(flag);
		} else {
			this->flags &= ~cosmos::to_integral(flag);
		}
	}

	void setWinGravity(const Gravity gravity) {
		this->win_gravity = raw_gravity(gravity);
	}

	void setDimensions(const int p_width, const int p_height) {
		this->width = p_width;
		this->height = p_height;
	}

	void setBaseDimensions(const int p_width, const int p_height) {
		this->base_width = p_width;
		this->base_height = p_height;
	}

	void setMinDimensions(const int p_width, const int p_height) {
		this->min_width = p_width;
		this->min_height = p_height;
	}

	void setMaxDimensions(const int p_width, const int p_height) {
		this->max_width = p_width;
		this->max_height = p_height;
	}

	void setIncrements(const int p_width, const int p_height) {
		this->width_inc = p_width;
		this->height_inc = p_height;
	}

	void setPosition(const Coord pos) {
		this->x = pos.x;
		this->y = pos.y;
	}
};

} // end ns

#endif // inc. guard
