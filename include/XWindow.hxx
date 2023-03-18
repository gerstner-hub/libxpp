#ifndef XPP_WINDOW_HXX
#define XPP_WINDOW_HXX

// C++
#include <memory>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

// X
#include <X11/Xatom.h> // declaration of various atom types like XA_WINDOW
#include <X11/Xlib.h>
#include <X11/Xutil.h> // XWMHints
#include <X11/X.h> // event mask bits

// cosmos
#include "cosmos/ostypes.hxx"

// X++
#include "X++/ClassHints.hxx"
#include "X++/Property.hxx"
#include "X++/SizeHints.hxx"
#include "X++/WindowManagerHints.hxx"
#include "X++/X11Exception.hxx"
#include "X++/XDisplay.hxx"

namespace xpp {

struct XWindowAttrs;
class XCursor;

/// Wrapper for the X Window primitive
/**
 * This class stores an XWindow identifier and provides operations on X Window
 * objects, like retrieving and setting window properties.
 **/
class XPP_API XWindow {
public: // types

	/// Exception used for property query errors.
	struct PropertyQueryError :
			public X11Exception {
		PropertyQueryError(Display *dis, const int errcode);

		COSMOS_ERROR_IMPL;
	};

	/// Exception used for property change errors.
	struct PropertyChangeError :
			public X11Exception {
		PropertyChangeError(Display *dis, const int errcode);

		COSMOS_ERROR_IMPL;
	};

	/// Exception used for the case when property types don't match.
	struct PropertyTypeMismatch :
			public cosmos::CosmosError {
		PropertyTypeMismatch(AtomID expected, AtomID encountered);

		COSMOS_ERROR_IMPL;
	};

	/// Exception used for the case when a requested property doesn't exist.
	struct PropertyNotExisting :
			public cosmos::CosmosError {
		PropertyNotExisting() :
			CosmosError{"PropertyNotExisting", "Requested property is not existing"}
		{}

		COSMOS_ERROR_IMPL;
	};

	/// Exception used in situations when an operation is not implemented by the window manager.
	struct NotImplemented :
			public cosmos::CosmosError {
		NotImplemented() :
			CosmosError{"NotImplemented", "The operation is not implemented"}
		{}

		COSMOS_ERROR_IMPL;
	};

	/// Keeps metadata about a property.
	struct PropertyInfo {
		/// the property's type
		AtomID type;
		/// the number of items of the given format
		size_t items = 0;
		/// the format of the property 8/16/32
		size_t format = 0;

		size_t numBytes() const {
			return items * (format / 8);
		}
	};

	using WindowSet = std::set<WinID>;
	using ClassStringPair = std::pair<std::string, std::string>;

public: // functions

	/// Create an object without binding to a window
	XWindow() = default;

	/// Plain copy
	XWindow(const XWindow &other) = default;

	/// Create an object representing \c win on the default Display
	explicit XWindow(WinID win);

	XWindow& operator=(const WinID &win) {
		*this = XWindow{win};
		return *this;
	}

	/// returns whether the object is bound to window
	bool valid() const { return m_win != WinID::INVALID; }

	/// returns the Xlib primitive Window identifier
	WinID id() const { return m_win; }

	/// Retrieve the name of the represented window.
	/**
	 * An X window does not have an integral name attached to it. Instead
	 * there are properties the window manager can set according to
	 * standards or to its own discretion.
	 *
	 * This function tries to retrieve the property as defined by the EWMH
	 * standard or the ICCCM standard as a fallback.
	 *
	 * If the name cannot be determined an exception is thrown.
	 **/
	std::string getName() const;

	/// Set \c name as the new name of the current window.
	/**
	 * If the window name cannot be set then an exception is thrown.
	 *
	 * This sets both the EMWH and ICCCM name attributes of the window.
	 *
	 * On success the window manager should update the visible window
	 * title accordingly.
	 **/
	void setName(const std::string_view name);

	/// Retrieve the PID that owns the represented window.
	/**
	 * If the pid cannot be determined an exception is thrown.
	 **/
	cosmos::ProcessID getPID() const;

	/// Retrieve the desktop number the window is currently on.
	/**
	 * The same details found at getName() are true here, too.
	 *
	 * If the desktop nr. cannot be determined an exception is thrown.
	 **/
	int getDesktop() const;

	/// returns the client machine the window is associated with.
	std::string getClientMachine() const;

	/// Returns the window class parameters for this window.
	/**
	 * The first returned string is the name of the application, the
	 * second returned string is the class of the application.
	 *
	 * On error an exception is thrown.
	 **/
	ClassStringPair getClass() const;

	/// Returns the command line used to create the window.
	std::string getCommand() const;

	/// Returns the locale used by the window.
	std::string getLocale() const;

	/// Returns the ID of the client leader window.
	/**
	 * The client leader is part of the window manager session management
	 * to identify windows that belong together.
	 *
	 * The client leader window itself also should have this property set
	 * with its own window ID as value.
	 **/
	WinID getClientLeader() const;

	/// Returns the type of the window.
	/**
	 * The returned value is an atom of a predefined set of values like
	 * _NET_WM_WINDOW_TYPE_NORMAL.
	 **/
	AtomID getWindowType() const;

	/// Returns the array of atoms representing the protocols supported by the window.
	void getProtocols(AtomIDVector &protocols) const;

	/// Sets the array of atoms representing the protocols supported by the window
	/**
	 * This can throw an exception on error.
	 **/
	void setProtocols(const AtomIDVector &protocols);

	/// returns the currently set XWMHints for the window.
	/**
	 * This can also return a nullptr in case there are no hints set for
	 * the current window.
	 **/
	std::shared_ptr<WindowManagerHints> getWMHints() const;

	void setWMHints(const WindowManagerHints &hints);

	/// Sets new window class and name hints for the window.
	void setClassHints(const ClassHints hints);

	/// Sets normal window manager size hints for the window.
	void setWMNormalHints(const SizeHints &hints);

	/// Requests the X server to destroy the represented window and all sub-windows.
	/**
	 * This request cannot be ignored by the application owning the
	 * window. It is a forcible way to remove the window from the X
	 * server. An alternative is the function sendDeleteRequest() which is
	 * a cooperative way to ask an application to close the window.
	 *
	 * The window represented by this object will be invalid after a
	 * successful return from this function. Further operations on it will
	 * fail.
	 **/
	void destroy();

	/// Creates a pseudo window as child of the current window.
	/**
	 * For the dimensions and other properties of the window currently
	 * sane defaults for a pseudo window that will never be mapped are
	 * chosen.
	 *
	 * \return The ID of the newly created window
	 **/
	WinID createChild();

	/// Requests the given selection buffer to be sent to this window.
	/**
	 * The result will be received at the target window via a
	 * SelectionNotify event.
	 *
	 * \param[in] selection
	 * 	The selection type to request
	 * \param[in] target_type
	 * 	The target format the selection buffer should be converted to
	 * \param[in] target_prop
	 * 	The target property the selection should be copied to
	 **/
	void convertSelection(
		const AtomID selection,
		const AtomID target_type,
		const AtomID target_prop,
		const Time t = CurrentTime
	);

	/// Makes the current window the owner of the given selection type.
	/**
	 * This means that other clients on the XServer can in the future
	 * request the selection from this window.
	 **/
	void makeSelectionOwner(const AtomID selection, const Time t = CurrentTime);

	/// Requests the targeted window to close itself.
	/**
	 * In contrast to destroy() this is a cooperative call that allows the
	 * targeted window to cleanly close itself and the associated
	 * application, possibly asking the user first.
	 **/
	void sendDeleteRequest();

	/// Retrieves a list of all properties currently present on this window.
	/**
	 * The vector of \c atoms is first cleared in any case and will be
	 * filled with the atoms identifying the properties existing on this
	 * window.
	 **/
	void getPropertyList(AtomIDVector &atoms);

	/// Retrieves property metadata about the given property present on this window.
	/**
	 * This information is useful for processing properties in a generic
	 * way without knowing their type in advance. For example to be used
	 * together with getPropertyList().
	 **/
	void getPropertyInfo(const AtomID property, PropertyInfo &info);

	/// Retrieves raw property data without interpreting type and format.
	/**
	 * Similar to getPropertyInfo() this returns the actual property info
	 * in \c info. On input the RawProperty's length and offset members
	 * will determine the offset into the property and maximum number of
	 * bytes to retrieve.
	 *
	 * \warning The length and offset requested need to be aligned to
	 * 32-bit boundaries, otherwise an exception is thrown. This is due to
	 * the design of the X API.
	 **/
	void getRawProperty(const AtomID property, PropertyInfo &info, RawProperty &out);

	/// Retrieve a property for this window object by name.
	/**
	 * The property \c name will be queried from the current window and
	 * stored in \c p.
	 *
	 * On error an exception is thrown.
	 *
	 * The PROPTYPE type must match the property's type.
	 **/
	template <typename PROPTYPE>
	void getProperty(const std::string_view name, Property<PROPTYPE> &p) const {
		getProperty(display.mapAtom(name), p);
	}

	/// Gets a property for this window object by AtomID.
	/**
	 * \see getProperty(copnst std::string_view name, ...)
	 * \param[in] info
	 * 	An optional pointer to the PropertyInfo for the given atom for
	 * 	helping determining the correct amount of data to retrieve.
	 **/
	template <typename PROPTYPE>
	void getProperty(const AtomID name_atom, Property<PROPTYPE> &p, const PropertyInfo *info = nullptr) const;

	/// Store a property in this window object by name.
	/**
	 * Sets the property \c name for the current window to the value
	 * stored in \c p.
	 *
	 * On error an exception is thrown.
	 **/
	template <typename PROPTYPE>
	void setProperty(const std::string_view name, const Property<PROPTYPE> &p) {
		setProperty(display.mapAtom(name), p);
	}

	/// Store a property in this window object by AtomID.
	/**
	 * \see setProperty(const std::string&, const Property<PROPTYPE>&)
	 **/
	template <typename PROPTYPE>
	void setProperty(const AtomID name_atom, const Property<PROPTYPE> &p);


	/// Removes the property of the given name identifier from the window.
	void delProperty(const std::string &name) {
		delProperty(display.mapAtom(name));
	}

	/// Removes the property of the given atom identifier from the window.
	void delProperty(const AtomID name_atom);

	/// compares the WinIDs of the given window objects for equality.
	bool operator==(const XWindow &o) const { return m_win == o.m_win; }
	bool operator!=(const XWindow &o) const { return !operator==(o); }

	/// Returns the next queued window event that matches the given event mask.
	/**
	 * If no matching event is currently pending for the window then this
	 * call flushes the output buffer and blocks until an event is received.
	 **/
	void nextEvent(XEvent &event, const long event_mask);

	/// Inform the X server that we want to be notified of window creation events.
	/**
	 * This call mostly makes sense for the root window to get notified of
	 * new windows that come to life.
	 **/
	void selectCreateEvent() const {
		// This is the only way to get CreateNotify events from the X
		// server.
		//
		// This gets us events for all the child windows like menus,
		// too.
		//
		// Thus if we don't want grandchildren Windows of the root
		// window then we need to filter on the event receiving side
		// within our process
		selectEvent(EventMask::SubstructureNotify);
	}

	/// Inform the X server that we want to be notified of window destruction events.
	/**
	 * If the current window will be destroyed an event will be sent to
	 * your X application.
	 **/
	void selectDestroyEvent() const {
		selectEvent(EventMask::StructureNotify);
	}

	/// Inform the X server that we want to be notified if properties of the current window change.
	/**
	 * This enables you to get events if properties of the current window
	 * are added, changed or deleted.
	 **/
	void selectPropertyNotifyEvent() const {
		selectEvent(EventMask::PropertyChange);
	}

	/// transparently cast the object into the raw WinID identifier.
	operator WinID() const { return m_win; }

	/// Retrieve the attributes for this window.
	/**
	 * If this fails then an X11Exception is thrown.
	 **/
	void getAttrs(XWindowAttrs &attrs);

	/// Sets new window attributes according to the given mask.
	void setWindowAttrs(const XSetWindowAttributes &attrs, const WindowAttrMask &mask);

	/// Move and or resize the window
	/**
	 * The x, y, width and height parameters from \c attrs will be used to
	 * perform the operation.
	 **/
	void moveResize(const XWindowAttrs &attrs);

	void setParent(const XWindow &parent) { m_parent = parent.id(); }

	WinID getParent() const { return m_parent; }

	/// Returns a set of the known child windows
	/**
	 * call updateFamily() to get ia snapshort of the current set of sub
	 * windows.
	 **/
	const WindowSet& children() const { return m_children; }

	void addChild(const XWindow &child) { m_children.insert(child.id()); }
	void delChild(const XWindow &child) { m_children.erase(child.id()); }

	/// Queries parent and child windows of this window and sets them acc.
	void updateFamily();

	/// Sends the given XEvent structure to the represented X11 window.
	void sendEvent(const XEvent &event);

	/// Copies image data from the given Pixmap into the window.
	/**
	 * \param[in] ext The extent of the area to copy
	 * \param[in] src_pos The upper-left coordinate of the copy area in the source pixmap
	 * \param[in] dst_pos The upper-left coordinate of the copy area in the dest window
	 **/
	void copyArea(const GcSharedPtr &gc, const PixmapID px,
			const Extent &ext, const Coord &src_pos = Coord{0,0}, const Coord &dst_pos = Coord{0,0});

	/// Use the given XCursor type as cursor in the associated window.
	void defineCursor(const XCursor &cursor);

protected: // functions

	/// Adds the given event(s) to the set of window events we want to be notified about.
	void selectEvent(const EventMask new_event) const;

	/// Sends a request to the window with a single long parameter as data.
	void sendRequest(const AtomID message, long data, const XWindow *window = nullptr) {
		return sendRequest(message, (const char*)&data, sizeof(data), window);
	}

	/// Sends a request to the window.
	/**
	 * To have the window (manager) actively do something on our request
	 * we need to send it an event.
	 *
	 * This event contains a message of what to do and parameters to that
	 * message.
	 *
	 * Throws an exception on error.
	 *
	 * \param[in] message
	 * 	The basic message type that defines the further parameters of
	 * 	the request
	 * \param[in] data
	 * 	The raw data making up the parameters to the message
	 * \param[in] len
	 * 	The length of the raw data in bytes
	 * \param[in] window
	 * 	An optional window parameter. Mostly used for the RootWin to
	 * 	specify another window upon which should be acted.
	 **/
	void sendRequest(
		const AtomID message,
		const char *data = nullptr,
		const size_t len = 0,
		const XWindow *window = nullptr
	);

protected: // functions

	Window rawID() const;

protected: // data

	/// The X11 window ID this object represents
	WinID m_win = WinID::INVALID;
	/// The X11 window ID of the parent of this window
	WinID m_parent = WinID::INVALID;
	/// A set of X11 window IDs that represents the children of this window
	WindowSet m_children;

	/// The X11 input event mask currently associated with this window
	mutable EventSelectionMask m_input_event_mask;
	/// The X11 event mask used for sending out events via sendEvent()
	EventSelectionMask m_send_event_mask;
};

extern template XPP_API void XWindow::getProperty(const AtomID, Property<int>&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<const char*>&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<AtomID>&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<WinID>&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<std::vector<AtomID> >&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<std::vector<WinID> >&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<std::vector<int> >&, const PropertyInfo*) const;
extern template XPP_API void XWindow::getProperty(const AtomID, Property<std::vector<utf8_string> >&, const PropertyInfo*) const;
extern template XPP_API void XWindow::setProperty(const AtomID, const Property<const char*>&);
extern template XPP_API void XWindow::setProperty(const AtomID, const Property<int>&);
extern template XPP_API void XWindow::setProperty(const AtomID, const Property<utf8_string>&);
extern template XPP_API void XWindow::setProperty(const AtomID, const Property<AtomID>&);

} // end ns

#endif // inc. guard
