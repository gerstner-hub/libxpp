// cosmos
#include "cosmos/formatting.hxx"
#include "cosmos/string.hxx"

// X++
#include "X++/helpers.hxx"
#include "X++/formatting.hxx"
#include "X++/private/Xpp.hxx"
#include "X++/RootWin.hxx"

namespace xpp {

RootWin::RootWin(XDisplay &p_display, int screen) :
		XWindow{WinID{XRootWindow(p_display, screen)}} {
		
	Xpp::getLogger().debug() << "root window has id: " << *this << "\n";

	// the event mask influences which X clients will receive the event.
	// For the root window to react to our requests these masks seem to be
	// helpful. NoEvent value doesn't work. I'm not sure if this is not
	// too broad but there is no clear documentation which specific value
	// might be correct for our use cases
	m_send_event_mask.set({
		EventMask::SubstructureRedirect,
		EventMask::SubstructureNotify
	});
}

RootWin::RootWin() :
		RootWin{display, display.defaultScreen()}
{}

void RootWin::queryWindows() {
	m_windows.clear();
	auto &logger = Xpp::getLogger();

	/*
	 * The _NET_CLIENT_LIST, if supported, is set on the root window and
	 * contains an array of X windows that are managed by the WM.
	 * 
	 * According to EWMH the array is in "initial mapping order" which I
	 * guess is the order in which windows have been created (or more
	 * correct: mapped). Alongside there is the _NET_CLIENT_LIST_STACKING
	 * property that contains the same data but in "bottom-to-top stacked
	 * order" which means it is ordered according to the layer the window
	 * is in.
	 * 
	 * NOTE: There's also _WIN_CLIENT_LIST with a similar purpose,
	 * probably from ICCCM
	 */
	try {
		Property<std::vector<WinID>> windows;
		this->getProperty(atoms::ewmh_wm_window_list, windows);

		const auto &wins = windows.get();

		logger.debug() << "window list acquired:\n";

		for (const auto &win: wins) {
			m_windows.push_back(WinID{win});
			logger.debug() << "- " << raw_win(win) << "\n";
		}

	} catch (const cosmos::CosmosError &ex) {
		logger.warn() << "Couldn't query window list: " << ex.what();
		throw;
	}

}

void RootWin::queryTree() {
	/*
	 * this query operation is inherently racy ... windows might
	 * appear/disappear and we thus might construct an invalid state
	 *
	 * there could be a possibility to lock the complete X server for the
	 * duration of this operation but I didn't look that up yet ...
	 */

	// helper class for storing tree traversal state
	class TreeNode :
			public XWindow {
	public:
		explicit TreeNode(WinID w) : XWindow{w} {
			// query all children
			updateFamily();
			m_next = m_children.begin();
		}

		TreeNode(const TreeNode &o) = delete;

		bool isFinished() const { return m_next == m_children.end(); }
		void nextChild() { m_next++; }
		WinID currentChild() { return *m_next; }

		// next child to process
		WindowSet::const_iterator m_next;
	};

	m_tree.clear();

	// Use heap allocated node instances, otherwise the copying
	// within containers becomes troublesome. Using C++11
	// movables there might be a more efficient and elegant
	// approach
	std::vector<TreeNode*> to_process;

	try {

		// start with the root window
		to_process.push_back(new TreeNode{this->id()});

		while (!to_process.empty()) {
			auto &current = *(to_process.back());

			if (current.isFinished()) {
				// no more childs, add this window to the list
				// and remove it
				m_tree.push_back(WinID{current.id()});
				delete &current;
				to_process.pop_back();
			} else {
				// add the next child to the list to process
				// and select the next child window for the
				// currently processed one
				auto next = current.currentChild();
				current.nextChild();

				to_process.push_back(new TreeNode{next});
			}
		}
	} catch (const cosmos::CosmosError &ex) {
		Xpp::getLogger().warn() << "Couldn't query window tree: " << ex.what();
		while (!to_process.empty()) {
			delete to_process.back();
			to_process.pop_back();
		}
		throw;
	}
}

} // end ns
