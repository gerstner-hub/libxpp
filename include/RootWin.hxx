#ifndef XPP_ROOTWINDOW_HXX
#define XPP_ROOTWINDOW_HXX

// C++
#include <vector>

// X++
#include "X++/dso_export.h"
#include "X++/types.hxx"
#include "X++/fwd.hxx"
#include "X++/XWindow.hxx"

namespace xpp {

/// This class represents the special root window.
/**
 * The root window contains information about all other existing windows and
 * the window manager.
 **/
class XPP_API RootWin :
		public XWindow {
public: // types

	/// Specialized exception for root window query errors.
	struct QueryError :
			public cosmos::CosmosError {
		explicit QueryError(const std::string &s) :
				CosmosError{"QueryError", s} {}

		COSMOS_ERROR_IMPL;
	};

public: // functions

	/// Creates a root window representation for the default display/screen.
	RootWin();

	/// Creates a root window representation for the given display/screen.
	explicit RootWin(XDisplay &display, ScreenID screen);

	/// Returns the complete hierarchy of windows, actively queried.
	/**
	 * While windowList() returns only windows that are propagated by
	 * the window manager, this function actively queries the hierarchy of
	 * X windows from the root window onwards.
	 * 
	 * Thus this list also contains hidden windows, decoration windows
	 * etc.
	 *
	 * You need to call queryTree() to get actual data from this call.
	 **/
	const auto& windowTree() const { return m_tree; }

	/// Returns the list of active application main windows.
	/**
	 * You need to call queryWindows() to get actual data from this call.
	 **/
	const auto& windowList() const { return m_windows; }

	/// Queries all existing windows from the WM and stores them in the object.
	void queryWindows();

	/// Queries the complete window tree and stores the windows in the object.
	/**
	 * This includes also hidden and decoration windows.
	 **/
	void queryTree();

protected: // data

	/// An array of all main windows existing, in undefined order.
	std::vector<WinID> m_windows;
	/// An array of all (even special) windows existing, in undefined order.
	std::vector<WinID> m_tree;
};

} // end ns

#endif // inc. guard
