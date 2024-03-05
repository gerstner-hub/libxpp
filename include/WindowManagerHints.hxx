#pragma once

// C++
#include <type_traits>

// X11
#include <X11/Xutil.h>

// Cosmos
#include <cosmos/BitMask.hxx>
#include <cosmos/memory.hxx>
#include <cosmos/utils.hxx>

namespace xpp {

/// Wrapper around the XWMHints structure.
/**
 * This wrapper adds a couple of helper functions to operate on the struct's
 * members using libxpp strong types.
 **/
class WindowManagerHints :
		public XWMHints {
public: // types

	enum class Flags : long {
		INPUT         = InputHint,
		STATE         = StateHint,
		ICON_PIXMAP   = IconPixmapHint,
		ICON_WINDOW   = IconWindowHint,
		ICON_POSITION = IconPositionHint,
		ICON_MASK     = IconMaskHint,
		WINDOW_GROUP  = WindowGroupHint,
		URGENCY       = XUrgencyHint
	};

	using Mask = cosmos::BitMask<Flags>;

public: // functions

	void clear() {
		static_assert(sizeof(WindowManagerHints) == sizeof(XWMHints));
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

	void setWMInputHandling(const bool on_off) {
		this->input = on_off;
	}
};

} // end ns
