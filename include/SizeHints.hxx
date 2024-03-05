#pragma once

// C++
#include <type_traits>

// libX11
#include <X11/Xutil.h>

// Cosmos
#include <cosmos/BitMask.hxx>
#include <cosmos/memory.hxx>

// xpp
#include <xpp/helpers.hxx>
#include <xpp/types.hxx>

namespace xpp {

/// Wrapper around the XSizeHints structure.
/**
 * This adds helper types and wrappers to use strong types in conjunction with
 * XSizeHints.
 **/
class SizeHints :
		public XSizeHints {
public: // types

	enum class Flags : long {
		USER_POS         = USPosition, // x, y
		USER_SIZE        = USSize,     // width, height
		PROG_POS         = PPosition, 
		PROG_SIZE        = PSize,
		PROG_MIN_SIZE    = PMinSize,
		PROG_MAX_SIZE    = PMaxSize,
		PROG_RESIZE_INCREMENTS = PResizeInc,
		PROG_ASPECT      = PAspect,   // min, max aspect ratios
		PROG_BASE_SIZE   = PBaseSize, // base size for incrementing
		PROG_WIN_GRAVITY = PWinGravity
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
