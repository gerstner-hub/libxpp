#pragma once

// X11
#include <X11/keysym.h>

// C++
#include <optional>

// cosmos
#include "cosmos/utils.hxx"

// xpp
#include "xpp/dso_export.h"
#include "xpp/fwd.hxx"
#include "xpp/types.hxx"

/**
 * @file
 *
 * Types and misc funcionality related to the keyboard.
 **/

namespace xpp {

enum class BellVolume : int {
	MIN_VOL = -100,
	NONE    =    0,
	MAX_VOL =  100
};

/// Ring the keyboard bell.
/**
 * This call rings the bell in the context of the given window. If `name` is
 * given then this should be a string mapped to an atom that will be provided
 * to event consumers ("named bell").
 **/
bool XPP_API ring_bell(const XWindow &win,
		const BellVolume volume,
		const std::optional<AtomID> name = {},
		std::optional<XDisplay*> p_display = {});

/// Keyboard symbol identifiers.
/**
 * \note These are also considered XID values.
 **/
enum class KeySymID : ::KeySym {
	NO_SYMBOL    = NoSymbol,
	BACKSPACE    = XK_BackSpace,
	BREAK        = XK_Break,
	DELETE       = XK_Delete,
	DOWN         = XK_Down,
	UP           = XK_Up,
	LEFT         = XK_Left,
	RIGHT        = XK_Right,
	F1           = XK_F1,
	F2           = XK_F2,
	F3           = XK_F3,
	F4           = XK_F4,
	F5           = XK_F5,
	F6           = XK_F6,
	F7           = XK_F7,
	F8           = XK_F8,
	F9           = XK_F9,
	F10          = XK_F10,
	F11          = XK_F11,
	F12          = XK_F12,
	F13          = XK_F13,
	F14          = XK_F14,
	F15          = XK_F15,
	F16          = XK_F16,
	F17          = XK_F17,
	F18          = XK_F18,
	F19          = XK_F19,
	F20          = XK_F20,
	F21          = XK_F21,
	F22          = XK_F22,
	F23          = XK_F23,
	F24          = XK_F24,
	F25          = XK_F25,
	F26          = XK_F26,
	F27          = XK_F27,
	F28          = XK_F28,
	F29          = XK_F29,
	F30          = XK_F30,
	F31          = XK_F31,
	F32          = XK_F32,
	F33          = XK_F33,
	F34          = XK_F34,
	F35          = XK_F35,
	HOME         = XK_Home,
	END          = XK_End,
	INSERT       = XK_Insert,
	ISO_LEFT_TAB = XK_ISO_Left_Tab,
	KP_0         = XK_KP_0,
	KP_1         = XK_KP_1,
	KP_2         = XK_KP_2,
	KP_3         = XK_KP_3,
	KP_4         = XK_KP_4,
	KP_5         = XK_KP_5,
	KP_6         = XK_KP_6,
	KP_7         = XK_KP_7,
	KP_8         = XK_KP_8,
	KP_9         = XK_KP_9,
	KP_ADD       = XK_KP_Add,
	KP_BEGIN     = XK_KP_Begin,
	KP_DECIMAL   = XK_KP_Decimal,
	KP_DELETE    = XK_KP_Delete,
	KP_DIVIDE    = XK_KP_Divide,
	KP_DOWN      = XK_KP_Down,
	KP_END       = XK_KP_End,
	KP_ENTER     = XK_KP_Enter,
	KP_HOME      = XK_KP_Home,
	KP_INSERT    = XK_KP_Insert,
	KP_LEFT      = XK_KP_Left,
	KP_MULTIPLY  = XK_KP_Multiply,
	KP_NEXT      = XK_KP_Next,
	KP_PRIOR     = XK_KP_Prior,
	KP_RIGHT     = XK_KP_Right,
	KP_SUBTRACT  = XK_KP_Subtract,
	KP_UP        = XK_KP_Up,
	NEXT         = XK_Next,
	NUM_LOCK     = XK_Num_Lock,
	PRINT        = XK_Print,
	PRIOR        = XK_Prior,
	RETURN       = XK_Return,
	A            = XK_A,
	B            = XK_B,
	C            = XK_C,
	D            = XK_D,
	E            = XK_E,
	F            = XK_F,
	G            = XK_G,
	H            = XK_H,
	I            = XK_I,
	J            = XK_J,
	K            = XK_K,
	L            = XK_L,
	M            = XK_M,
	N            = XK_N,
	O            = XK_O,
	P            = XK_P,
	Q            = XK_Q,
	R            = XK_R,
	S            = XK_S,
	T            = XK_T,
	U            = XK_U,
	V            = XK_V,
	W            = XK_W,
	X            = XK_X,
	Y            = XK_Y,
	Z            = XK_Z
};

inline auto raw_key = cosmos::to_integral<KeySymID>;

} // end ns
