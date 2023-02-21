#ifndef XPP_XDISPLAY_HXX
#define XPP_XDISPLAY_HXX

// C
#include <stdint.h>

// C++
#include <optional>

// X11
#include <X11/Xlib.h>
#include <X11/Xatom.h>

// cosmos
#include "cosmos/fs/FileDescriptor.hxx"

// X++
#include "X++/types.hxx"
#include "X++/X11Exception.hxx"
#include "X++/XAtom.hxx"

namespace xpp {

class Event;
class XWindow;

/// Wrapper around the Xlib Display type.
/**
 * This class associates the Xlib Display type with relevant operations. Most
 * importantly the Display provides the actual Atom mapping operations and is
 * also required to create instances of the XWindow type.
 * 
 * I decided to make this type a singleton as the one and only Display
 * instance is required at a lot of places and passing it from one place to
 * another becomes very annoying.
 *
 * Making it possible to operate on the non-default Display should be not too
 * hard to implement, if ever needed.
 **/
class XPP_API XDisplay {
public: // types

	/// Specialized X11Exception for Atom Mapping Errors.
	struct AtomMappingError :
			public xpp::X11Exception {
		AtomMappingError(Display *dis, const int errcode, const std::string_view s);

		COSMOS_ERROR_IMPL;
	};

	/// Specialized Exception for errors regarding opening the Display.
	struct DisplayOpenError :
			public cosmos::CosmosError {
		DisplayOpenError();

		COSMOS_ERROR_IMPL;
	};

public: // functions

	~XDisplay();

	/// returns a file descriptor representing the connection to the X server
	cosmos::FileDescriptor getConnectionNumber() {
		auto fd = cosmos::FileNum{XConnectionNumber(m_dis)};
		return cosmos::FileDescriptor{fd};
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

 	/// Creates an X atom for the given string and returns it.
	/**
	 * The function always returns a valid Atom, even if it first needs to
	 * be created by X11.
	 * 
	 * Can throw AtomMappingError.
	 **/
	XAtom getAtom(const std::string_view name) {
		Atom ret = XInternAtom(m_dis, name.data(), False);

		if (ret == BadAlloc || ret == BadValue || ret == None) {
			cosmos_throw (AtomMappingError(m_dis, ret, name));
		}

		return XAtom{ret};
	}

	std::string getName(const XAtom atom) {
		auto str = XGetAtomName(m_dis, atom.get());
		std::string ret{str};
		XFree(str);
		return ret;
	}

	/// Flushes any commands not yet issued to the server.
	/**
	 * Xlib, if not running in synchronous mode, assumes that an X
	 * application is frequently sending some X commands to the server and
	 * thus buffers commands according to some implementation defined
	 * strategy.
	 * 
	 * To make sure that any recently issued communication to the X server
	 * takes place right now you can call this function.
	 **/
	void flush() {
		if (XFlush(m_dis) == 0) {
			cosmos_throw (X11Exception("XFlush failed"));
		}
	}

	/// Returns the next event pending for this client.
	/**
	 * If no event is currently queued at the display then output buffers
	 * are flushed and the call blocks until a new event is received.
	 * 
	 * This call returns any events for any windows regardless of their
	 * event types.
	 * 
	 * This call does not pass on error conditions, it will always
	 * succeed.
	 **/
	void getNextEvent(Event &event);

	/// Creates a new window with the given properties.
	/**
	 * If depth is not provided then the default depth for the current
	 * display and default screen is used.
	 * 
	 * If visual is not provided then the default visual will be be used.
	 * 
	 * If attrs and value_mask are not provided then default attributes
	 * apply. If one is supplied then the other needs to be supplied, too.
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

	/// requests to map the given window to make is visible on the screen
	/**
	 * If an ancestor of the given window is not yet mapped, then the
	 * mapping of the window will not take place until the ancestor is
	 * also mapped.
	 **/
	void mapWindow(const XWindow &win);

	/// Flushes any commands not yet issued to the server and waits for it to process them
	/**
	 * This is just like flush(), with the extra functionality that this
	 * call will block until all requests have also been processed by the
	 * XServer. This is helpful, for example, if we have registered new
	 * events to be notified of and want to make sure the XServer knows
	 * this at some point in time.
	 **/
	void sync() {
		if (XSync(m_dis, False) == 0) {
			cosmos_throw (X11Exception("XSync failed"));
		}
	}

	/// puts libX11 into synchronized or unsynchronized mode.
	/**
	 * By default the client library caches various operations and only
	 * sends them out to the X server during certain actions or on
	 * explicit flush() requests. By turning synchronized mode on this
	 * behaviour will be changed and every bit of communication towards
	 * the server will be carried out right away.
	 *
	 * Setting this to on is mostly useful for debugging issues in the X
	 * protocol e.g. errors will be reported right away and not only
	 * asynchronously a longer time later. Performance will be poor
	 * in this mode.
	 **/
	void setSynchronized(bool on_off);

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

	/// Creates a new graphics context using the given settings
	/**
	 * A graphics context is used for specifying options when operating on
	 * a target drawable. It is allocated internally to Xlib, therefore it
	 * also has to be explicitly freed. This is simplified by returning a
	 * shared_ptr wrapper that does the right thing and the right time to
	 * free the resource.
	 *
	 * \param[in] d The drawable object that is the target or a parent of
	 * the target drawable with which this context is to be used. The
	 * target drawable needs to have the same depth as the drawable
	 * specified here, otherwise operational errors will occur.
	 *
	 * \param[in] mask Specifies the elements of \c vals that should be
	 * evaluated.
	 *
	 * \param[in] vals The settings that should be in effect for the new
	 * graphics context. Only the values marked in \c mask will be
	 * evaluated.
	 *
	 * \return The managed graphics context resource
	 **/
	GcSharedPtr createGraphicsContext(Drawable d, const GcOptMask &mask, const XGCValues &vals);

	/// return the window ID owning the given selection type
	/**
	 * If \c selection is invalid or the given selection has no owner then
	 * no value is.
	 **/
	std::optional<Window> getSelectionOwner(const XAtom &selection) const;

	/// transparently casts the instance to the Xlib Display primitive
	operator Display*() { return m_dis; }

	/// Returns a reference to the single XDisplay instance.
	/**
	 * \note
	 * 	The first access to this function (construction) can generate
	 * 	errors that will be propagated by means of exceptions.
	 **/
	static XDisplay& getInstance();

protected: // functions

	/// Opens the default display, protected due to singleton pattern
	XDisplay();

	// disallow copy of the singleton XDisplay
	XDisplay(const XDisplay &other) = delete;

protected: // data

	/// The Xlib primitive for the Display
	mutable Display *m_dis = nullptr;
};

} // end ns

#endif // inc. guard
