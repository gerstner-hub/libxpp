#ifndef XPP_XCURSOR_HXX
#define XPP_XCURSOR_HXX

// X11
#include <X11/cursorfont.h>

// X++
#include "X++/types.hxx"
#include "X++/dso_export.h"

namespace xpp {

class XDisplay;
struct XColor;

/// Represents an XCursor definition obtained from /// XDisplay::createFontCursor().
/**
 * This is a move-only type with ownership semantics. Upon destruction the
 * cursor will be freed. Even after freeing the cursor will still be usable in
 * objects (windows) that use it. After all users of the cursor have vanished
 * the cursor will be freed automatically then.
 **/
class XPP_API XCursor {
	friend class XDisplay;
	XCursor(const XCursor&) = delete;
	XCursor& operator=(const XCursor&) = delete;
public: // functions
	
	XCursor(XCursor &&o) {
		*this = std::move(o);
	}

	XCursor& operator=(XCursor &&o) {
		m_id = o.m_id;
		m_display = o.m_display;
		o.m_id = CursorID::INVALID;
		o.m_display = nullptr;
		return *this;
	}

	~XCursor();

	bool valid() const {
		return m_id != CursorID::INVALID;
	}

	/// Defines new foreground and background colors to the used for the cursor
	void recolorCursor(const XColor &fg, const XColor &bg);

	auto id() const { return m_id; }

protected: // functions
	   
	explicit XCursor(XDisplay *display, CursorID id) :
			m_display(display),
			m_id{id} {
	}

protected: // data

	XDisplay *m_display = nullptr;
	CursorID m_id = CursorID::INVALID;
};

/// Different cursor font types that can be used to create an XCursor.
enum class CursorFont : unsigned int {
	XCursor = XC_X_cursor,
	Arrow = XC_arrow,
	BasedArrowDown = XC_based_arrow_down,
	BasedArrowUp = XC_based_arrow_up,
	Boat = XC_boat,
	Bogosity = XC_bogosity,
	BottomLeftCorner = XC_bottom_left_corner,
	BottomRightCorner = XC_bottom_right_corner,
	BottomSide = XC_bottom_side,
	BottomTee = XC_bottom_tee,
	BoxSpiral = XC_box_spiral,
	CenterPtr = XC_center_ptr,
	Circle = XC_circle,
	Clock = XC_clock,
	CoffeeMug = XC_coffee_mug,
	Cross = XC_cross,
	CrossReverse = XC_cross_reverse,
	Crosshair = XC_crosshair,
	DiamondCross = XC_diamond_cross,
	Dot = XC_dot,
	Dotbox = XC_dotbox,
	DoubleArrow = XC_double_arrow,
	DraftLarge = XC_draft_large,
	DraftSmall = XC_draft_small,
	DrapedBox = XC_draped_box,
	Exchange = XC_exchange,
	Fleur = XC_fleur,
	Gobbler = XC_gobbler,
	Gumby = XC_gumby,
	Hand1 = XC_hand1,
	Hand2 = XC_hand2,
	Heart = XC_heart,
	Icon = XC_icon,
	IronCross = XC_iron_cross,
	LeftPtr = XC_left_ptr,
	LeftSide = XC_left_side,
	LeftTee = XC_left_tee,
	Leftbutton = XC_leftbutton,
	LlAngle = XC_ll_angle,
	LrAngle = XC_lr_angle,
	Man = XC_man,
	Middlebutton = XC_middlebutton,
	Mouse = XC_mouse,
	Pencil = XC_pencil,
	Pirate = XC_pirate,
	Plus = XC_plus,
	QuestionArrow = XC_question_arrow,
	RightPtr = XC_right_ptr,
	RightSide = XC_right_side,
	RightTee = XC_right_tee,
	Rightbutton = XC_rightbutton,
	RtlLogo = XC_rtl_logo,
	Sailboat = XC_sailboat,
	SbDownArrow = XC_sb_down_arrow,
	SbHDoubleArrow = XC_sb_h_double_arrow,
	SbLeftArrow = XC_sb_left_arrow,
	SbRightArrow = XC_sb_right_arrow,
	SbUpArrow = XC_sb_up_arrow,
	SbVDoubleArrow = XC_sb_v_double_arrow,
	Shuttle = XC_shuttle,
	Sizing = XC_sizing,
	Spider = XC_spider,
	Spraycan = XC_spraycan,
	Star = XC_star,
	Target = XC_target,
	Tcross = XC_tcross,
	TopLeftArrow = XC_top_left_arrow,
	TopLeftCorner = XC_top_left_corner,
	TopRightCorner = XC_top_right_corner,
	TopSide = XC_top_side,
	TopTee = XC_top_tee,
	Trek = XC_trek,
	UlAngle = XC_ul_angle,
	Umbrella = XC_umbrella,
	UrAngle = XC_ur_angle,
	Watch = XC_watch,
	Xterm = XC_xterm,
};

} // end ns

#endif // inc. guard
