#ifndef XPP_XDISPLAY_HXX
#define XPP_XDISPLAY_HXX

// C
#include <stdint.h>

// stdlib
#include <optional>

// xlib
#include <X11/Xlib.h>
#include <X11/Xatom.h>

// cosmos
#include "cosmos/fs/FileDescriptor.hxx"

// X++
#include "X++/types.hxx"
#include "X++/X11Exception.hxx"

namespace xpp {

class Event;
class XWindow;

/**
 * \brief
 * 	Wrapper around the Xlib Display type.
 * \details
 *	This class associates the Xlib Display type with relevant operations.
 *	Most importantly the Display provides the actual Atom mapping
 *	operations and is also required to create instances of the XWindow
 *	type.
 *
 *	I decided to make this type a singleton as the one and only Display
 *	instance is required at a lot of places and passing it from one place
 *	to another becomes very annoying.
 **/
class XPP_API XDisplay {
public: // types

	//! Specialized X11Exception for Atom Mapping Errors
	struct AtomMappingError : public xpp::X11Exception {
		AtomMappingError(Display *dis, const int errcode, const std::string &s);

		COSMOS_ERROR_IMPL;
	};

	//! Specialized Exception for errors regarding opening the Display
	struct DisplayOpenError : public cosmos::CosmosError {
		DisplayOpenError();

		COSMOS_ERROR_IMPL;
	};

public: // functions

	~XDisplay();

	/// returns a file descriptor representing the connection to the X server
	cosmos::FileDescriptor getConnectionNumber() {
		return cosmos::FileDescriptor(XConnectionNumber(m_dis));
	}

	/// returns the numbers of events currently queued but not yet processes
	size_t getPendingEvents() const {
		auto ret = XPending(m_dis);
		if (ret < 0) {
			cosmos_throw (X11Exception("XPending() failed"));
		}

		return static_cast<size_t>(ret);
	}

	bool hasPendingEvents() const {
		return getPendingEvents() != 0;
	}

	/**
	 * \brief
	 *	Creates an X atom for the given string and returns it.
	 * \details
	 *	The function always returns a valid Atom, even if it first
	 *	needs to be created by X11.
	 *
	 *	Can throw AtomMappingError.
	 **/
	Atom getAtom(const std::string &name) {
		return getAtom(name.c_str());
	}

	//! see getAtom(const std::string&)
	Atom getAtom(const char *name) {
		Atom ret = XInternAtom(m_dis, name, False);

		if (ret == BadAlloc || ret == BadValue || ret == None) {
			cosmos_throw (AtomMappingError(m_dis, ret, name));
		}

		return ret;
	}

	std::string getName(const Atom atom) {
		auto str = XGetAtomName(m_dis, atom);
		std::string ret(str);
		XFree(str);
		return ret;
	}

	/**
	 * \brief
	 *	Flushes any commands not yet issued to the server
	 * \details
	 * 	Xlib, if not running in synchronous mode, assumes that an X
	 * 	application is frequently sending some X commands to the
	 * 	server and thus buffers commands according to some
	 * 	implementation defined strategy.
	 *
	 * 	To make sure that any recently issued communication to the X
	 * 	server takes place right now you can call this function.
	 **/
	void flush() {
		if (XFlush(m_dis) == 0) {
			cosmos_throw (X11Exception("XFlush failed"));
		}
	}

	/**
	 * \brief
	 * 	Returns the next event pending on the display
	 * \details
	 * 	If no event is currently queued at the display then output
	 * 	buffers are flushed and the call blocks until a new event is
	 * 	received.
	 *
	 * 	This call returns any events for any windows regardless of
	 * 	their event types.
	 *
	 * 	This call does not pass on error conditions, it will always
	 * 	succeed.
	 **/
	void getNextEvent(Event &event);

	/**
	 * \brief
	 * 	Creates a new window with the given properties
	 * \details
	 * 	If depth is not provided then the default depth for the
	 * 	current display and default screen is used.
	 *
	 * 	If visual is not provided then the default visual will be be
	 * 	used.
	 *
	 * 	If attrs and value_mask are not provided then default
	 * 	attributes apply. If one is supplied then the other needs to
	 * 	be supplied, too.
	 **/
	XWindow createWindow(
		const WindowSpec &spec,
		unsigned int border_width,
		unsigned int clazz = CopyFromParent,
		const std::optional<XWindow*> parent = std::nullopt,
		const std::optional<int> &depth = std::nullopt,
		const std::optional<Visual*> &visual = std::nullopt,
		const std::optional<unsigned long> &value_mask = std::nullopt,
		const std::optional<XSetWindowAttributes*> &attrs = std::nullopt
	);

	/**
	 * \brief
	 * 	Flushes any commands not yet issued to the server and waits
	 * 	for it to process them
	 * \details
	 * 	This is just like flush(), with the extra functionality that
	 * 	this call will block until all requests have also been
	 * 	processed by the XServer. This is helpful, for example, if we
	 * 	have registered new events to be notified of and want to make
	 * 	sure the XServer knows this at some point in time.
	 **/
	void sync() {
		if (XSync( m_dis, False ) == 0) {
			cosmos_throw (X11Exception("XSync failed"));
		}
	}

	int getDefaultScreen() const {
		return XDefaultScreen(m_dis);
	}

	Visual* getDefaultVisual(const std::optional<int> screen = std::nullopt) const {
		return XDefaultVisual(m_dis, screen ? *screen : getDefaultScreen());
	}

	int getDefaultDepth(const std::optional<int> screen = std::nullopt) const {
		return XDefaultDepth(m_dis, screen ? *screen : getDefaultScreen());
	}

	Colormap getDefaultColormap(const std::optional<int> screen = std::nullopt) const {
		return XDefaultColormap(m_dis, screen ? *screen : getDefaultScreen());
	}

	int getDisplayWidth(const std::optional<int> screen = std::nullopt) const {
		return DisplayWidth(m_dis, screen ? *screen : getDefaultScreen());
	}

	int getDisplayHeight(const std::optional<int> screen = std::nullopt) const {
		return DisplayHeight(m_dis, screen ? *screen : getDefaultScreen());
	}

	/// creates a pixmap for the given window and of the given size
	/**
	 * The returned object is only a simple wrapper for type safety.
	 * Resource management is still explicit i.e. you have to free the
	 * Pixmap when no longer needed via freePixmap().
	 *
	 * \param[in] depth the depth of the pixmap, if given, otherwise the
	 *            default depth for the display and window involved.
	 **/
	PixMap createPixmap(
		const XWindow &win,
		const Extent &extent,
		const std::optional<int> depth = std::nullopt) const;

	/// frees a pixmap previously obtained via createPixmap()
	void freePixmap(PixMap &pm) const;

	//! transparently casts the instance to the Xlib Display primitive
	operator Display*() { return m_dis; }

	/**
	 * \brief
	 * 	Returns a reference to the single XDisplay instance
	 * \note
	 * 	The first access to this function (construction) can generate
	 * 	errors that will be propagated by means of exceptions.
	 **/
	static XDisplay& getInstance();

protected: // functions

	//! Opens the default display, protected due to singleton pattern
	XDisplay();

	// disallow copy of the singleton XDisplay
	XDisplay(const XDisplay &other) = delete;

protected: // data

	//! The Xlib primitive for the Display
	mutable Display *m_dis = nullptr;
};

} // end ns

#endif // inc. guard
