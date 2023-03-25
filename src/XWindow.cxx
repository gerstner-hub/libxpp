// C++
#include <cassert>
#include <sstream>

// cosmos
#include "cosmos/algs.hxx"
#include "cosmos/error/InternalError.hxx"
#include "cosmos/formatting.hxx"
#include "cosmos/memory.hxx"

// X++
#include "X++/Event.hxx"
#include "X++/formatting.hxx"
#include "X++/GraphicsContext.hxx"
#include "X++/helpers.hxx"
#include "X++/private/Xpp.hxx"
#include "X++/Property.hxx"
#include "X++/SizeHints.hxx"
#include "X++/WindowManagerHints.hxx"
#include "X++/XCursor.hxx"
#include "X++/XWindowAttrs.hxx"
#include "X++/XWindow.hxx"

namespace xpp {

XWindow::PropertyTypeMismatch::PropertyTypeMismatch(AtomID expected, AtomID encountered) :
		CosmosError{"PropertyTypeMismatch", "Retrieved property has different type than expected"} {
	std::ostringstream s;
	s << "Expected " << expected << " but encountered " << encountered;
	m_msg += s.str();
}

XWindow::PropertyChangeError::PropertyChangeError(Display *dis, const int errcode) :
		X11Exception{dis, errcode} {
	m_msg = std::string("Error changing property: ") + m_msg;
}

XWindow::PropertyQueryError::PropertyQueryError(Display *dis, const int errcode) :
		X11Exception{dis, errcode} {
	m_msg = std::string("Error querying property: ") + m_msg;
}

XWindow::XWindow(WinID win) :
		XWindow{} {
	m_win = win;
}

Window XWindow::rawID() const {
	return raw_win(m_win);
}

std::string XWindow::getName() const {
	try {
		xpp::Property<utf8_string> utf8_name;

		this->getProperty(atoms::ewmh_window_name, utf8_name);

		return utf8_name.get().str.data();
	} catch(...) {}

	/*
	 * If EWMH name property is not present then try to fall back to ICCCM
	 * WM_NAME property
	 * (at least I think that is ICCCM). This will not be in UTF8 but in
	 * XA_STRING format
	 */

	xpp::Property<const char*> name;

	this->getProperty(atoms::icccm_window_name, name);

	return name.get();
}

void XWindow::setName(const std::string_view name) {
	try {
		xpp::Property<utf8_string> utf8_name;
		utf8_name = utf8_string{name};

		this->setProperty(atoms::ewmh_window_name, utf8_name);

		return;
	} catch(...) {}

	xpp::Property<const char*> name_prop(name.data());

	this->setProperty(atoms::icccm_window_name, name_prop);
}


cosmos::ProcessID XWindow::getPID() const {
	xpp::Property<int> pid;

	this->getProperty(atoms::ewmh_window_pid, pid);

	return cosmos::ProcessID{pid.get()};
}

int XWindow::getDesktop() const {
	xpp::Property<int> desktop_nr;

	this->getProperty(atoms::ewmh_window_desktop, desktop_nr);

	auto ret = desktop_nr.get();
	return ret;
}

std::string XWindow::getClientMachine() const {
	xpp::Property<const char *> name;

	this->getProperty(atoms::icccm_wm_client_machine, name);

	return name.get();
}

std::string XWindow::getCommand() const {
	xpp::Property<const char *> name;

	this->getProperty(atoms::icccm_wm_command, name);

	return name.get();
}

std::string XWindow::getLocale() const {
	xpp::Property<const char *> locale;

	this->getProperty(atoms::icccm_wm_locale, locale);

	return locale.get();
}

WinID XWindow::getClientLeader() const {
	xpp::Property<WinID> leader;

	this->getProperty(atoms::icccm_wm_client_leader, leader);

	return leader.get();
}

AtomID XWindow::getWindowType() const {
	xpp::Property<AtomID> type;

	this->getProperty(atoms::ewmh_wm_window_type, type);

	return type.get();
}

void XWindow::getProtocols(AtomIDVector &protocols) const {
	protocols.clear();

	Atom *ret = nullptr;
	int ret_count = 0;

	const auto status = ::XGetWMProtocols(
		display,
		rawID(),
		&ret,
		&ret_count
	);

	if (status == 0) {
		cosmos_throw (X11Exception(display, status));
	}

	for (int num = 0; num < ret_count; num++) {
		protocols.push_back(AtomID{ret[num]});
	}

	::XFree(ret);
}

void XWindow::setProtocols(const AtomIDVector &protocols) {
	AtomVector plain;
	for (const auto &prot: protocols) {
		plain.push_back(static_cast<Atom>(prot));
	}

	auto res = ::XSetWMProtocols(display, rawID(), plain.data(), plain.size());

	if (res != True) {
		cosmos_throw (X11Exception(display, res));
	}
}

std::shared_ptr<WindowManagerHints> XWindow::getWMHints() const {
	auto hints = ::XGetWMHints(display, rawID());

	if (!hints) {
		return nullptr;
	}

	return make_shared_xptr(reinterpret_cast<WindowManagerHints*>(hints));
}

void XWindow::setWMHints(const WindowManagerHints &hints) {
	const XWMHints *base = &hints;
	// currently always returns 1, hints aren't modified in the lib
	(void)::XSetWMHints(display, rawID(), const_cast<XWMHints*>(base));
}

void XWindow::setClassHints(const ClassHints hints) {
	// no negative returns codes ever occur
	// the hints parameter is declared non-const but the structure is
	// never modified in the implementation.
	const XClassHint *base = &hints;
	(void)::XSetClassHint(display, rawID(), const_cast<XClassHint*>(base));
}

void XWindow::setWMNormalHints(const SizeHints &hints) {
	const XSizeHints *base = &hints;
	// has no return code, doesn't modify the hints structure
	::XSetWMNormalHints(display, rawID(), const_cast<XSizeHints*>(base));
}

XWindow::ClassStringPair XWindow::getClass() const {
	/*
	 * there's a special pair of functions X{Set,Get}ClassHint but that
	 * would be more work for us, we get the raw property which consists
	 * of two consecutive null terminated strings
	 */
	xpp::Property<const char *> clazz;

	this->getProperty(atoms::icccm_wm_class, clazz);

	ClassStringPair ret;

	ret.first = std::string{clazz.get()};
	// this is not very safe but our Property modelling currently lacks
	// support for strings containing null terminators (we can't get the
	// complete size from the property)
	ret.second = std::string{clazz.get() + ret.first.length() + 1};

	return ret;
}

void XWindow::destroy() {
	const auto res = ::XDestroyWindow(display, rawID());
	display.flush();

	if (res != 1) {
		cosmos_throw (X11Exception(display, res));
	}
}

WinID XWindow::createChild() {
	Window new_win = ::XCreateSimpleWindow(
		display,
		rawID(),
		// dimensions and alike don't matter for this hidden window
		-10, -10, 1, 1, 0, 0, 0
	);

	if (new_win == 0) {
		cosmos_throw (X11Exception("Failed to create pseudo child window"));
	}

	display.flush();

	return WinID{new_win};
}

void XWindow::convertSelection(
		const AtomID selection,
		const AtomID target_type,
		const AtomID target_prop,
		const Time t) {

	if (::XConvertSelection(
			display,
			raw_atom(selection),
			raw_atom(target_type),
			raw_atom(target_prop),
			rawID(),
			t) != 1) {
		cosmos_throw (X11Exception("Failed to request selecton conversion"));
	}

	display.flush();
}

void XWindow::makeSelectionOwner(const AtomID selection, const Time t) {
	// libX11 always returns 1 here, so ignore it
	::XSetSelectionOwner(display, raw_atom(selection), rawID(), t);
}

void XWindow::sendDeleteRequest() {
	long data[2];
	data[0] = raw_atom(atoms::icccm_wm_delete_window);
	data[1] = CurrentTime;

	sendRequest(
		atoms::icccm_wm_protocols,
		(const char*)&data[0],
		sizeof(data),
		this
	);
}

void XWindow::sendRequest(
		const AtomID message,
		const char *data,
		const size_t len,
		const XWindow *window) {
	auto &logger = Xpp::getLogger();

	logger.debug()
		<< "Sending request to window " << *this << ":"
		<< "msg = " << message << " with " << len << " bytes of data, window = "
		<< to_string(window ? window->id() : WinID{0}) << std::endl;

	XEvent event;
	cosmos::zero_object(event);

	if (len > sizeof(event.xclient.data)) {
		cosmos_throw(X11Exception("XEvent data exceeds maximum"));
	}

	event.xclient.type = ClientMessage;
	event.xclient.serial = 0;
	event.xclient.send_event = True;
	event.xclient.message_type = raw_atom(message);
	event.xclient.window = window ? window->rawID() : 0;
	event.xclient.format = 32;
	std::memcpy(event.xclient.data.b, data, len);

	sendEvent(event);
}

void XWindow::sendEvent(const Event &event) {
	auto raw = event.raw();
	sendEvent(*raw);
}

void XWindow::sendEvent(const XEvent &event) {
	const Status s = ::XSendEvent(
		display,
		rawID(),
		False,
		m_send_event_mask.raw(),
		const_cast<XEvent*>(&event)
	);

	if (s == BadValue || s == BadWindow) {
		cosmos_throw (X11Exception(display, s));
	}

	// make sure the event gets sent out
	display.flush();
}

void XWindow::selectEvent(const EventMask new_event) const {
	m_input_event_mask.set(new_event);

	const int res = ::XSelectInput(display, rawID(), m_input_event_mask.raw());

	if (res == 0) {
		cosmos_throw (X11Exception("XSelectInput failed"));
	}
}

void XWindow::getPropertyList(AtomIDVector &atoms) {
	atoms.clear();

	int num_atoms = 0;

	Atom *list = ::XListProperties(display, rawID(), &num_atoms);

	if (list == nullptr) {
		// could be an error (probably) or a window without any
		// properties
		return;
	}

	for (int i = 0; i < num_atoms; i++) {
		atoms.push_back(AtomID{list[i]});
	}

	::XFree(list);
}

void XWindow::getPropertyInfo(const AtomID property, PropertyInfo &info) {
	RawProperty p;

	// reuse the code from getRawProperty but don't ask for any actual
	// data (zero length in RawProperty causes that)
	getRawProperty(property, info, p);
}

void XWindow::getRawProperty(const AtomID property, PropertyInfo &info, RawProperty &out) {
	int actual_format = 0;
	unsigned long number_items = 0;
	/*
	 * NOTE: even if we don't want any data returned, the function
	 * expects valid pointers to pointers here, otherwise we segfault
	 */
	unsigned long bytes_left = 0;
	unsigned char *prop_data = nullptr;

	if ((out.length & 0x3) != 0 || (out.offset & 0x3) != 0) {
		cosmos_throw (cosmos::UsageError("length or offset not aligned to 32-bits"));
	}

	const auto res = ::XGetWindowProperty(
		display,
		rawID(),
		raw_atom(property),
		out.offset / 4, /* offset in 32-bit multiples */
		out.length / 4, /* length in 32-bit multiples */
		False, /* deleted property ? */
		AnyPropertyType,
		cosmos::to_raw_ptr(&info.type),
		&actual_format,
		&number_items,
		&bytes_left, /* bytes left to read */
		&prop_data /* output buffer to read into */
	);

	if (res != Success) {
		cosmos_throw (X11Exception(display, res));
	}

	const auto bytes_per_item = actual_format / 8;

	if (actual_format != 0) {
		info.items = (bytes_left / bytes_per_item) + number_items + (out.offset / bytes_per_item);
	} else {
		info.items = 0;
	}

	info.format = actual_format;
	out.left = bytes_left;
	out.length = number_items * bytes_per_item;
	out.data = make_shared_xptr(prop_data);
}

template <typename PROPTYPE>
void XWindow::getProperty(const AtomID name_atom, Property<PROPTYPE> &prop, const PropertyInfo *info) const {
	// shorthand for our concrete property object
	typedef Property<PROPTYPE> THIS_PROP;

	auto x_type = THIS_PROP::getXType();
	assert(x_type != AtomID::INVALID);

	Atom actual_type;
	// if 8, 16, or 32-bit format was actually read
	int actual_format = 0;
	unsigned long ret_items = 0;
	unsigned long remaining_bytes = 0;
	unsigned char *data = nullptr;

	const size_t max_len = info ?
		(info->items * (info->format / 8)) : 65536 / 4;

	const int res = ::XGetWindowProperty(
		display,
		rawID(),
		raw_atom(name_atom),
		// offset into the property data
		0,
		// maximum length of the property to read in 32-bit items
		max_len,
		// delete request
		False,
		// our expected type
		raw_atom(x_type),
		// actually present type, format, number of items
		&actual_type,
		&actual_format,
		&ret_items,
		&remaining_bytes,
		// where data is stored
		&data
	);

	// note: on success data is allocated by Xlib. data always contains
	// one excess byte that is set to zero thus its possible to use data
	// as a c-string without copying it.
	if  (res != Success) {
		cosmos_throw (PropertyQueryError(display, res));
	}

	try {
		AtomID this_type{actual_type};
		if (this_type == AtomID::INVALID) {
			cosmos_throw (PropertyNotExisting());
		} else if (x_type != this_type) {
			cosmos_throw (PropertyTypeMismatch(x_type, this_type));
		} else if (remaining_bytes != 0) {
			cosmos_throw (cosmos::InternalError("Bytes remaining during property read"));
		}

		assert (actual_format == THIS_PROP::Traits::FORMAT);

		// ret_items gives the number of items acc. to actual_format that have been returned
		prop.takeData(data, ret_items * (actual_format / 8));
	} catch(...) {
		::XFree(data);
		throw;
	}
}

template <typename PROPTYPE>
void XWindow::setProperty(const AtomID name_atom, const Property<PROPTYPE> &prop) {
	/*
	 * NOTE: currently only performs mode PropModeReplace
	 *
	 * I don't think that prepend or append are very common use cases.
	 */
	// shorthand for our concrete Property object
	typedef Property<PROPTYPE> THIS_PROP;

	auto x_type = THIS_PROP::getXType();
	assert (x_type != AtomID::INVALID);

	const int siz = THIS_PROP::Traits::numElements(prop.get());

	const int res = ::XChangeProperty(
		display,
		rawID(),
		raw_atom(name_atom),
		raw_atom(x_type),
		THIS_PROP::Traits::FORMAT,
		PropModeReplace,
		(unsigned char*)prop.raw(),
		siz
	);

	// XChangeProperty returns constantly 1 which would result in
	// "BadRequest".
	// Actual errors are dispatched asynchronously via the functions set
	// at XSetErrorHandler. Not very helpful as hard to process
	// asynchronously and inefficient to synchronize to the asynchronous
	// result.
	(void)res;

	// requests to the server are not dispatched immediatly thus we need
	// to flush once
	display.flush();
}

void XWindow::delProperty(const AtomID name_atom) {
	const auto status = ::XDeleteProperty(display, rawID(), raw_atom(name_atom));

	if (status == 0) {
		cosmos_throw (X11Exception(display, status));
	}

	// see setProperty()
	display.flush();
}

void XWindow::nextEvent(XEvent &event, const long event_mask) {
	const auto status = ::XWindowEvent(display, rawID(), event_mask, &event);

	if (status == 0) {
		cosmos_throw (X11Exception(display, status));
	}
}

void XWindow::getAttrs(XWindowAttrs &attrs) {
	const auto status = ::XGetWindowAttributes(display, rawID(), &attrs);

	// stupid error codes again. A non-zero status on success?
	if (status == 0) {
		cosmos_throw(X11Exception(display, status));
	}
}

void XWindow::setWindowAttrs(const XSetWindowAttributes &attrs, const WindowAttrMask &mask) {
	// - the return value is always the 1 here so no need to check
	// - the attrs are never changed in Xlib so const_cast is safe
	::XChangeWindowAttributes(display, rawID(), mask.raw(), const_cast<XSetWindowAttributes*>(&attrs));
}

void XWindow::moveResize(const XWindowAttrs &attrs) {
	const auto status = ::XMoveResizeWindow(
		display, rawID(), attrs.x, attrs.y, attrs.width, attrs.height
	);

	if (status == 0) {
		cosmos_throw (X11Exception(display, status));
	}
}

void XWindow::updateFamily() {
	Window root = 0, parent = 0;
	Window *children = nullptr;
	unsigned int num_children = 0;

	m_children.clear();
	m_parent = WinID::INVALID;

	const Status res = ::XQueryTree(display, rawID(), &root, &parent, &children, &num_children);

	if (res != 1) {
		cosmos_throw (X11Exception(display, res));
	}

	m_parent = WinID{parent};

	for (unsigned int i = 0; i < num_children; i++) {
		m_children.insert(WinID{children[i]});
	}

	::XFree(children);
}

void XWindow::copyArea(const GraphicsContext &gc, const PixmapID px,
		const Extent &ext, const Coord &src_pos, const Coord &dst_pos) {
	// does not return synchronous errors
	(void)::XCopyArea(
		display, cosmos::to_integral(px), rawID(), gc,
		src_pos.x, src_pos.y,
		ext.width, ext.height,
		dst_pos.x, dst_pos.y);
}

void XWindow::defineCursor(const XCursor &cursor) {
	// does not return synchronous errors
	(void)::XDefineCursor(display, raw_win(m_win), cosmos::to_integral(cursor.id()));
}

/*
 * explicit template instantiations
 *
 * allow to outline the above template code
 */
template void XWindow::getProperty(const AtomID, Property<int>&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<const char*>&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<AtomID>&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<WinID>&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<std::vector<AtomID> >&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<std::vector<WinID> >&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<std::vector<int> >&, const PropertyInfo*) const;
template void XWindow::getProperty(const AtomID, Property<std::vector<utf8_string> >&, const PropertyInfo*) const;
template void XWindow::setProperty(const AtomID, const Property<const char*>&);
template void XWindow::setProperty(const AtomID, const Property<int>&);
template void XWindow::setProperty(const AtomID, const Property<utf8_string>&);
template void XWindow::setProperty(const AtomID, const Property<AtomID>&);

} // end ns
