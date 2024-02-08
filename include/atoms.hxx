#pragma once

// X11
#include <X11/Xatom.h>

// xpp
#include "xpp/CachedAtom.hxx"

/**
 * @file
 *
 * A couple of widely used atom definitions provides in form of CachedAtom
 * instances in a dedicated namespace.
 **/

namespace xpp::atoms {

/*
 * TODO: the constexpr cannot currently be declared extern (will be possible
 * in C++20 with module support).
 *
 * constexpr means that each translation unit gets its own copy.
 * inline constexpr means during link time multiple copies will be merged
 * again.
 *
 * For the shared library case this probably means we'll still get multiple
 * instances from different contexts. Since we're caching based on AtomMapper
 * it's not that bad, but also not ideal.
 */

/// Window name property (EWMH).
inline constexpr CachedAtom ewmh_window_name{"_NET_WM_NAME"};
/// Window icon name property (EWMH).
inline constexpr CachedAtom ewmh_icon_name{"_NET_WM_ICON_NAME"};
/// Name of UTF8 string type.
inline constexpr CachedAtom ewmh_utf8_string{"UTF8_STRING"};
/// Property containing an array of windows managed by EWMH comp. WM.
inline constexpr CachedAtom ewmh_wm_window_list{"_NET_CLIENT_LIST"};
/// Desktop on which a window is currently on.
inline constexpr CachedAtom ewmh_window_desktop{"_NET_WM_DESKTOP"};
/// pID associated with a window.
inline constexpr CachedAtom ewmh_window_pid{"_NET_WM_PID"};
/// Property of EWMH comp. wm on root window.
inline constexpr CachedAtom ewmh_support_check{"_NET_SUPPORTING_WM_CHECK"};
/// Property containing EWMH comp. wm PID.
inline constexpr CachedAtom ewmh_wm_pid{"_NET_WM_PID"};
/// Property containing a list of hints describing the window state.
inline constexpr CachedAtom ewmh_wm_state{"_NET_WM_STATE"};
/// Property indicating that a NET_WM_STATE property is to be toggled.
inline constexpr CachedAtom ewmh_wm_state_toggle{"_NET_WM_STATE_TOGGLE"};
/// Property for _NET_WM_STATE that requests fullscreen mode without window decorations.
inline constexpr CachedAtom ewmh_wm_state_fullscreen{"_NET_WM_STATE_FULLSCREEN"};
/// Property indicating that EWMH comp. wm is set to show the desktop.
inline constexpr CachedAtom ewmh_wm_desktop_shown{"_NET_SHOWING_DESKTOP"};
/// Property giving the number of desktops available.
inline constexpr CachedAtom ewmh_wm_nr_desktops{"_NET_NUMBER_OF_DESKTOPS"};
/// Property containing a list of utf8 strings denoting the desktops names.
inline constexpr CachedAtom ewmh_wm_desktop_names{"_NET_DESKTOP_NAMES"};
/// Property denoting the currently active desktop number.
inline constexpr CachedAtom ewmh_wm_cur_desktop{"_NET_CURRENT_DESKTOP"};
/// Property denoting EWMH comp. wm desktop number for a window.
inline constexpr CachedAtom ewmh_desktop_nr{"_NET_WM_DESKTOP"};
/// Property containing the ID of the currently active window.
inline constexpr CachedAtom ewmh_wm_active_window{"_NET_ACTIVE_WINDOW"};
/// The type of the window.
inline constexpr CachedAtom ewmh_wm_window_type{"_NET_WM_WINDOW_TYPE"};
/// Name of the machine running the client as seen from server.
inline constexpr CachedAtom icccm_client_machine{"WM_CLIENT_MACHINE"};
/// Window name property according to ICCCM specification.
inline constexpr CachedAtom icccm_window_name{"WM_NAME"};
/// Contains and defines x11 protocols to be supported by clients or not.
inline constexpr CachedAtom icccm_wm_protocols{"WM_PROTOCOLS"};
/// A WM_PROTOCOLS request to delete a window.
inline constexpr CachedAtom icccm_wm_delete_window{"WM_DELETE_WINDOW"};
/// Contains the name of the machine a window is running on.
inline constexpr CachedAtom icccm_wm_client_machine{"WM_CLIENT_MACHINE"};
/// Contains the name of application instance and class as two consecutive, null terminated strings.
inline constexpr CachedAtom icccm_wm_class{"WM_CLASS"};
/// Contains the command line used to create the window.
inline constexpr CachedAtom icccm_wm_command{"WM_COMMAND"};
/// Contains the locale used by a window's application.
inline constexpr CachedAtom icccm_wm_locale{"WM_LOCALE_NAME"};
/// Contains the ID of the client leader window.
inline constexpr CachedAtom icccm_wm_client_leader{"WM_CLIENT_LEADER"};
/// clipboard selection identifier
inline constexpr CachedAtom clipboard{"CLIPBOARD"};
/// primary selection identifier
inline constexpr CachedAtom primary_selection{AtomID{XA_PRIMARY}};
/// non-UTF8 ASCII string type
inline constexpr CachedAtom string_type{AtomID{XA_STRING}};
inline constexpr CachedAtom wm_icon_name{AtomID{XA_WM_ICON_NAME}};

} // end ns
