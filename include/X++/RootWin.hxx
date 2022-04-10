#ifndef XPP_ROOTWINDOW_HXX
#define XPP_ROOTWINDOW_HXX

// C++
#include <vector>

// X++
#include "X++/utf8_string.hxx"
#include "X++/XDisplay.hxx"
#include "X++/XWindow.hxx"

namespace xpp {

/// This class represents the special root window.
/**
 * The root window contains information about all other windows opened and it
 * has attached information about the window manager.
 * 
 * Thus appropriate operations are added to the common XWindow in this class.
 **/
class XPP_API RootWin :
	public XWindow {
public: // types

	//! Specialized exception for root window query errors.
	struct QueryError : public cosmos::CosmosError {
		explicit QueryError(const std::string &s) : CosmosError("QueryError", s) {}

		COSMOS_ERROR_IMPL;
	};

	/// Enum for differentiating different window manager implementations.
	/**
	 * This is for future use if we need to adjust our logic to the actual
	 * window manager encountered.
	 **/
	enum class WindowManager {
		//! the fluxbox window manager.
		FLUXBOX,
		//! the i3 tiling window manager.
		I3,
		//! unknown window manager encountered.
		UNKNOWN
	};

public: // functions

	//! Creates a root window representation for the default display/screen
	RootWin();

	//! Creates a root window representation for the given display/screen
	explicit RootWin(XDisplay &display, int screen);

	//! Checks for presence of compatible WM and retrieves information.
	void getInfo();

	//! returns whether a window manager name was found.
	bool hasWM_Name() const { return !m_wm_name.empty(); }

	//! returns whether a window manager PID was found.
	bool hasWM_Pid() const { return m_wm_pid != -1; }

	//! returns whether a window manager class was found.
	bool hasWM_Class() const { return m_wm_class.valid(); }

	//! returns whether a window manager show the desktop mode was found.
	bool hasWM_ShowDesktopMode() const { return m_wm_showing_desktop != -1; }

	//! returns whether the currently active desktop was found.
	bool hasWM_ActiveDesktop() const { return m_wm_active_desktop != -1; }

	//! returns whether the currently active window was found.
	bool hasWM_ActiveWindow() const { return m_wm_active_window != 0; }

	//! returns whether the current number of desktops was found.
	bool hasWM_NumDesktops() const { return m_wm_num_desktops != -1; }

	//! returns the window manager name, if found.
	const char* getWM_Name() const { return m_wm_name.data(); }

	//! returns the window manager PID, if found.
	int getWM_Pid() const { return m_wm_pid; }

	//! returns the window manager class, if found.
	const char* getWM_Class() const { return m_wm_class.get().str.c_str(); }

	//! returns the show the desktop mode, if found.
	bool getWM_ShowDesktopMode() const { return m_wm_showing_desktop == 1; }

	//! returns the currently active desktop, if found.
	int getWM_ActiveDesktop() const { return m_wm_active_desktop; }

	//! changes the active desktop.
	void setWM_ActiveDesktop(const int &num);

	//! returns the number of desktops determined.
	int getWM_NumDesktops() const { return m_wm_num_desktops; }

	//! sets the number of desktops.
	void setWM_NumDesktops(const int &num);

	//! returns the type of the running window manager.
	WindowManager getWM_Type() const { return m_wm_type; }

	//! returns the currently focused window, if any / supported.
	Window getWM_ActiveWindow() const { return m_wm_active_window; }

	//! requests to change the focus to the given window.
	void setWM_ActiveWindow(const XWindow &win);

	//! returns the list of windows managed by the window manager.
	const auto& getWindowList() const { return m_windows; }

	//! returns the vector of known desktop names in order of occurence.
	const auto& getDesktopNames() const { return m_wm_desktop_names; }

	//! Returns the complete hierarchy of windows, actively queried.
	/**
	 * While getWindowList() returns only windows that are propagated by
	 * the window manager, this function actively queries the hierarchy of
	 * X windows from the root window onwards.
	 * 
	 * Thus this list also contains hidden windows, decoration windows
	 * etc.
	 **/
	const auto& getWindowTree() const { return m_tree; }

	/*
	 * These are called from the event thread when property changes have
	 * been reported
	 */

	void updateShowingDesktop();
	void updateActiveDesktop();
	void updateNumberOfDesktops();
	void updateDesktopNames();
	void updateActiveWindow();

	//! Queries all existing windows from the WM and stores them in m_windows.
	void queryWindows();

	//! Queries the complete window tree and stores the windows in m_tree.
	void queryTree();

	/// Asks the window manager to move the given window the the given desktop.
	/**
	 * On error an exception is thrown.
	 **/
	void moveToDesktop(const XWindow &window, const int desktop_nr);

protected: // functions

	/// Checks for existence of WM child window and correct setup of it.
	/**
	 * Sets m_ewmh_child to the child window set by the window manager
	 * \postcondition
	 * 	m_ewmh_child is set to the valid child window of the
	 * 	running WM
	 * \exception
	 * 	WMQueryError if WM couldn't be queried or is not running
	 **/
	void queryWMWindow();

	//! Queries direct properties of the WM like its name, class, pid and show desktop mode.
	void queryBasicWMProperties();

	//! Returns the window manager enum matching the given wm name.
	WindowManager detectWM(const std::string &name);

	//! Determines the PID of the window manager.
	void queryPID();

	//! Generic update function for property members.
	template <typename TYPE>
	void updateProperty(const XAtom &atom, TYPE &property);

private: // data

	//! if valid then this is the child window associated with EWMH compatible WM.
	XWindow m_ewmh_child;

	//! queried name of the window manager in UTF8 (or empty if N/A).
	std::string m_wm_name;
	//! the process ID of the window manager (or -1 if N/A).
	int m_wm_pid = -1;
	//! queried class of the window manager in UTF8 (or empty if N/A).
	xpp::Property<utf8_string> m_wm_class;
	//! integer specifying whether currently the "show desktop mode" is active (or -1 if N/A)
	int m_wm_showing_desktop = -1;

	//! An array of all main windows existing, in undefined order
	std::vector<XWindow> m_windows;
	//! An array of all (even special) windows existing, in undefined order
	std::vector<XWindow> m_tree;

	//! the concrete type of window manager encountered.
	WindowManager m_wm_type = WindowManager::UNKNOWN;

	//! number of available desktops.
	int m_wm_num_desktops = -1;

	//! the currently active/shown desktop nr.
	int m_wm_active_desktop = -1;

	//! the window that currently has focus.
	Window m_wm_active_window = 0;

	//! names of the desktops 0 ... m_wm_num_desktops.
	std::vector<std::string> m_wm_desktop_names;
};

} // end ns

#endif // inc. guard
