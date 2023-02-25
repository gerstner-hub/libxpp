#ifndef XPP_ROOTWINDOW_HXX
#define XPP_ROOTWINDOW_HXX

// C++
#include <vector>

// X++
#include "X++/XWindow.hxx"

namespace xpp {

class XDisplay;

/// This class represents the special root window.
/**
 * The root window contains information about all other existing windows.
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
	explicit RootWin(XDisplay &display, int screen);

	/// Returns the complete hierarchy of windows, actively queried.
	/**
	 * While getWindowList() returns only windows that are propagated by
	 * the window manager, this function actively queries the hierarchy of
	 * X windows from the root window onwards.
	 * 
	 * Thus this list also contains hidden windows, decoration windows
	 * etc.
	 **/
	const auto& getWindowTree() const { return m_tree; }

	/// Queries all existing windows from the WM and stores them in m_windows.
	void queryWindows();

	/// Queries the complete window tree and stores the windows in m_tree.
	void queryTree();

protected: // data

	/// An array of all main windows existing, in undefined order.
	std::vector<WinID> m_windows;
	/// An array of all (even special) windows existing, in undefined order.
	std::vector<WinID> m_tree;
};

} // end ns

#endif // inc. guard
