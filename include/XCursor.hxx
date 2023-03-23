#ifndef XPP_XCURSOR_HXX
#define XPP_XCURSOR_HXX

// X11
#include <X11/cursorfont.h>

// X++
#include "X++/dso_export.h"
#include "X++/fwd.hxx"
#include "X++/types.hxx"

namespace xpp {

/// Different cursor font types that can be used to create an XCursor.
enum class CursorFont : unsigned int {
	X_CURSOR            = XC_X_cursor,
	ARROW               = XC_arrow,
	BASED_ARROW_DOWN    = XC_based_arrow_down,
	BASED_ARROW_UP      = XC_based_arrow_up,
	BOAT                = XC_boat,
	BOGOSITY            = XC_bogosity,
	BOTTOM_LEFT_CORNER  = XC_bottom_left_corner,
	BOTTOM_RIGHT_CORNER = XC_bottom_right_corner,
	BOTTOM_SIDE         = XC_bottom_side,
	BOTTOM_TEE          = XC_bottom_tee,
	BOX_SPIRAL          = XC_box_spiral,
	CENTER_PTR          = XC_center_ptr,
	CIRCLE              = XC_circle,
	CLOCK               = XC_clock,
	COFFEE_MUG          = XC_coffee_mug,
	CROSS               = XC_cross,
	CROSS_REVERSE       = XC_cross_reverse,
	CROSSHAIR           = XC_crosshair,
	DIAMOND_CROSS       = XC_diamond_cross,
	DOT                 = XC_dot,
	DOTBOX              = XC_dotbox,
	DOUBLE_ARROW        = XC_double_arrow,
	DRAFT_LARGE         = XC_draft_large,
	DRAFT_SMALL         = XC_draft_small,
	DRAPED_BOX          = XC_draped_box,
	EXCHANGE            = XC_exchange,
	FLEUR               = XC_fleur,
	GOBBLER             = XC_gobbler,
	GUMBY               = XC_gumby,
	HAND1               = XC_hand1,
	HAND2               = XC_hand2,
	HEART               = XC_heart,
	ICON                = XC_icon,
	IRON_CROSS          = XC_iron_cross,
	LEFT_PTR            = XC_left_ptr,
	LEFT_SIDE           = XC_left_side,
	LEFT_TEE            = XC_left_tee,
	LEFT_BUTTON         = XC_leftbutton,
	LL_ANGLE            = XC_ll_angle,
	LR_ANGLE            = XC_lr_angle,
	MAN                 = XC_man,
	MIDDLEBUTTON        = XC_middlebutton,
	MOUSE               = XC_mouse,
	PENCIL              = XC_pencil,
	PIRATE              = XC_pirate,
	PLUS                = XC_plus,
	QUESTION_ARROW      = XC_question_arrow,
	RIGHT_PTR           = XC_right_ptr,
	RIGHT_SIDE          = XC_right_side,
	RIGHT_TEE           = XC_right_tee,
	RIGHT_BUTTON        = XC_rightbutton,
	RTL_LOGO            = XC_rtl_logo,
	SAIL_BOAT           = XC_sailboat,
	SB_DOWN_ARROW       = XC_sb_down_arrow,
	SB_HDOUBLE_ARROW    = XC_sb_h_double_arrow,
	SB_LEFT_ARROW       = XC_sb_left_arrow,
	SB_RIGHT_ARROW      = XC_sb_right_arrow,
	SB_UP_ARROW         = XC_sb_up_arrow,
	SB_V_DOUBLE_ARROW   = XC_sb_v_double_arrow,
	SHUTTLE             = XC_shuttle,
	SIZING              = XC_sizing,
	SPIDER              = XC_spider,
	SPRAYCAN            = XC_spraycan,
	STAR                = XC_star,
	TARGET              = XC_target,
	TCROSS              = XC_tcross,
	TOP_LEFT_ARROW      = XC_top_left_arrow,
	TOP_LEFT_CORNER     = XC_top_left_corner,
	TOP_RIGHT_CORNER    = XC_top_right_corner,
	TOP_SIDE            = XC_top_side,
	TOP_TEE             = XC_top_tee,
	TREK                = XC_trek,
	UL_ANGLE            = XC_ul_angle,
	UMBRELLA            = XC_umbrella,
	UR_ANGLE            = XC_ur_angle,
	WATCH               = XC_watch,
	XTERM               = XC_xterm
};

/// Represents an XCursor definition obtained from XDisplay::createFontCursor().
/**
 * This is a move-only type with ownership semantics. Upon destruction the
 * cursor will be freed. Even after freeing the cursor will still be usable in
 * objects (windows) that use it. After all users of the cursor have vanished
 * the cursor will be freed automatically then.
 **/
class XPP_API XCursor {
	XCursor(const XCursor&) = delete;
	XCursor& operator=(const XCursor&) = delete;
public: // functions

	/// Creates a new font based cursor of the given type.
	explicit XCursor(const CursorFont which, XDisplay &disp = xpp::display);

	XCursor(XCursor &&o) {
		*this = std::move(o);
	}

	XCursor& operator=(XCursor &&o) {
		m_id = o.m_id;
		m_display = o.m_display;
		o.invalidate();
		return *this;
	}

	~XCursor() {
		destroy();
	}

	bool valid() const {
		return m_id != CursorID::INVALID;
	}

	void destroy();

	/// Defines new foreground and background colors to the used for the cursor
	void recolorCursor(const XColor &fg, const XColor &bg);

	auto id() const { return m_id; }

protected: // functions

	void invalidate() {
		m_id = CursorID::INVALID;
		m_display = nullptr;
	}

protected: // data

	XDisplay *m_display = nullptr;
	CursorID m_id = CursorID::INVALID;
};

} // end ns

#endif // inc. guard
