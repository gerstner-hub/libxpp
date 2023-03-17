#ifndef XPP_WINDOWMANAGERHINTS_HXX
#define XPP_WINDOWMANAGERHINTS_HXX

// X11
#include <X11/Xutil.h>

// Cosmos
#include "cosmos/BitMask.hxx"
#include "cosmos/memory.hxx"

namespace xpp {

enum class HintFlags : long {
	Input = InputHint,
	State = StateHint,
	IconPixmap = IconPixmapHint,
	IconWindow = IconWindowHint,
	IconPosition = IconPositionHint,
	IconMask = IconMaskHint,
	WindowGroup = WindowGroupHint,
	Urgency = XUrgencyHint
};

using HintMask = cosmos::BitMask<HintFlags>;

/// Wrapper around the XWMHints structure.
/**
 * This wrapper adds a couple of helper functions to operate on the struct's
 * members using libX++ strong types.
 **/
struct WindowManagerHints {
public:
	WindowManagerHints() :
			m_hints(new XWMHints) {
		cosmos::zero_object(*m_hints);
	}

	explicit WindowManagerHints(std::shared_ptr<XWMHints> hints) :
			m_hints(hints)
	{}

	void setFlags(const HintMask hint_mask) {
		m_hints->flags = hint_mask.raw();
	}

	void changeFlag(const HintFlags flag, const bool on_off) {
		if (on_off) {
			m_hints->flags |= cosmos::to_integral(flag);
		} else {
			m_hints->flags &= ~cosmos::to_integral(flag);
		}
	}

	void setWMInputHandling(const bool on_off) {
		m_hints->input = on_off;
	}

	operator XWMHints*() {
		return &*m_hints;
	}

	bool valid() const {
		return m_hints.get() != nullptr;
	}

protected:

	std::shared_ptr<XWMHints> m_hints;
};

} // end ns

#endif // inc. guard
