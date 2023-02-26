#ifndef XPP_ATOMMAPPER_HXX
#define XPP_ATOMMAPPER_HXX

// C++
#include <map>
#include <string>
#include <stdint.h>
#include <iosfwd>

// cosmos
#include "cosmos/thread/RWLock.hxx"

// X++
#include "X++/types.hxx"
#include "X++/X11Exception.hxx"

namespace xpp {

/// Caching of Property name/type to AtomID Mappings.
/**
 * This class allows to cache Xlib atom mappings that can be quickly retrieved
 * in the future. If a mapping is not cached already then it is retrieved via
 * Xlib.
 * 
 * This class is thread safe by means of a read-write lock. Read-access can
 * occur in parallel, write accesses (due to cache misses) are exclusive.
 * 
 * This class is implemented as a singleton type. Get the global instance of
 * the mapper via the getInstance() function.
 *
 * \note
 *
 *  There are Xlib functions that allow to map strings to atoms and vice
 *  versa. However I'm not sure how efficient that mapping is done. One
 *  should assume that any mappings established should be efficiently cached
 *  locally within Xlib. Thus when we request the same mapping again the
 *  lookup will be very fast.
 *
 *  I decided to provide an own caching facility, however, to be on the safe
 *  side. Also the interface of this class allows for more C++-like usage.
 **/
class XPP_API AtomMapper {
public: // functions

	/// Retrieves the global mapper instance
	static AtomMapper& getInstance();

	/// Returns the atom representation of the given property name
	/**
	 * The function performs caching of resolved atom values to avoid
	 * having to excessively talk to the XServer
	 **/
	AtomID getAtom(const std::string_view s) const;

	/// tries to do a reverse lookup to get the name of \c atom
	const std::string& getName(const AtomID atom) const;

protected: // functions

	/// protected constructor to enforce singleton usage
	AtomMapper() {};
	AtomMapper(const AtomMapper&) = delete;

	const std::string& cacheMiss(const AtomID atom) const;
	AtomID cacheMiss(const std::string_view s) const;

protected: // types

	/// The map container that maps strings to X atoms
	using AtomMapping = std::map<std::string, AtomID>;

protected: // data

	/// contains the actual cached mappings
	mutable AtomMapping m_mappings;
	/// synchronizes parallel read and update of \c m_mappings
	cosmos::RWLock m_mappings_lock;
};

/// A struct-like class to hold a number of default property name atoms
/**
 * These properties are statically referenced in the code. We don't want to
 * write them redundantly time and again so we rather use these global
 * constants.
 * 
 * It's a singleton class to defer the time of instantiation to after the
 * entry of the main() function to avoid static initialization issues.
 */
struct XPP_API StandardProps {
	//! window name property (EWMH)
	AtomID atom_ewmh_window_name;
	//! desktop on which a window is currently on
	AtomID atom_ewmh_window_desktop;
	//! PID associated with a window
	AtomID atom_ewmh_window_pid;
	//! name of UTF8 string type
	AtomID atom_ewmh_utf8_string;
	//! property of EWMH comp. wm on root window
	AtomID atom_ewmh_support_check;
	//! property containing EWMH comp. wm PID
	AtomID atom_ewmh_wm_pid;
	//! property indicating that EWMH comp. wm is set to show the desktop
	AtomID atom_ewmh_wm_desktop_shown;
	//! property giving the number of desktops available
	AtomID atom_ewmh_wm_nr_desktops;
	//! property containing a list of utf8 strings denoting the desktops names
	AtomID atom_ewmh_wm_desktop_names;
	//! property denoting the currently active desktop number
	AtomID atom_ewmh_wm_cur_desktop;
	//! property denoting EWMH comp. wm desktop number for a window
	AtomID atom_ewmh_desktop_nr;
	//! property containing an array of windows managed by EWMH comp. WM
	AtomID atom_ewmh_wm_window_list;
	//! property containing the ID of the currently active window
	AtomID atom_ewmh_wm_active_window;
	//! the type of the window
	AtomID atom_ewmh_wm_window_type;
	//! name of the machine running the client as seen from server
	AtomID atom_icccm_client_machine;
	//! window name property acc. to ICCCM spec.
	AtomID atom_icccm_window_name;
	//! contains and defines x11 protocols to be supported by clients or not
	AtomID atom_icccm_wm_protocols;
	//! a WM_PROTOCOLS request to delete a window
	AtomID atom_icccm_wm_delete_window;
	//! contains the name of the machine a window is running on
	AtomID atom_icccm_wm_client_machine;
	//! contains the name of application instance and class as two
	//! consecutive, null terminated strings
	AtomID atom_icccm_wm_class;
	//! contains the command line used to create the window
	AtomID atom_icccm_wm_command;
	//! contains the locale used by a window's application
	AtomID atom_icccm_wm_locale;
	//! contains the ID of the client leader window
	AtomID atom_icccm_wm_client_leader;

	static const StandardProps& instance();

private: // functions

	StandardProps();
};

} // end ns

/// Output operator that prints the human readable name of an atom.
XPP_API std::ostream& operator<<(std::ostream &o, const xpp::AtomID atom);

#endif // inc. guard
