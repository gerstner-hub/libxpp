#pragma once

// xpp
#include "xpp/types.hxx"
#include "xpp/helpers.hxx"

namespace xpp {

/// Wrapper around XSetWindowAttributes.
/**
 * This wrapper adds helper methods to set structure members using strong
 * libxpp types.
 **/
class SetWindowAttributes :
		public XSetWindowAttributes {
public:
	void setBitGravity(const Gravity gravity) {
		this->bit_gravity = raw_gravity(gravity);
	}

	void setWinGravity(const Gravity gravity) {
		this->win_gravity = raw_gravity(gravity);
	}

	void setEventMask(const EventSelectionMask mask) {
		this->event_mask = mask.raw();
	}

	void changeEventMask(const EventMask event, const bool on_off) {
		if (on_off) {
			this->event_mask |= cosmos::to_integral(event);
		} else {
			this->event_mask &= ~cosmos::to_integral(event);
		}
	}

	void setColormap(const ColormapID cmap) {
		this->colormap = raw_cmap(cmap);
	}
};

} // end ns
