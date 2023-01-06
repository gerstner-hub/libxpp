// C++ stdlib
#include <sstream>
#include <iomanip>

// cosmos
#include "cosmos/errors/InternalError.hxx"

// X++
#include "X++/private/Xpp.hxx"
#include "X++/XAtom.hxx"
#include "X++/XWindowAttrs.hxx"
#include "X++/XWindow.hxx"
#include "X++/helpers.hxx"

namespace xpp {

XWindow::PropertyTypeMismatch::PropertyTypeMismatch(Atom expected, Atom encountered) :
		CosmosError("PropertyTypeMismatch", "Retrieved property has different type than expected") {
	std::ostringstream s;
	s << "Expected " << expected << " but encountered " << encountered;
	m_msg += s.str();
}

XWindow::PropertyChangeError::PropertyChangeError(Display *dis, const int errcode) :
		X11Exception(dis, errcode) {
	m_msg = std::string("Error changing property: ") + m_msg;
}

XWindow::PropertyQueryError::PropertyQueryError(Display *dis, const int errcode) :
	X11Exception(dis, errcode) {
	m_msg = std::string("Error querying property: ") + m_msg;
}

XWindow::XWindow(Window win) : XWindow() {
	m_win = win;
}

std::string XWindow::idStr() const {
	std::stringstream id;
	id << this->id();

	return id.str();
}

std::string XWindow::getName() const {
	try {
		xpp::Property<utf8_string> utf8_name;

		this->getProperty(m_std_props.atom_ewmh_window_name, utf8_name);

		return utf8_name.get().str;
	} catch(...) {}

	/*
	 * If EWMH name property is not present then try to fall back to ICCCM
	 * WM_NAME property
	 * (at least I think that is ICCCM). This will not be in UTF8 but in
	 * XA_STRING format
	 */

	xpp::Property<const char*> name;

	this->getProperty(m_std_props.atom_icccm_window_name, name);

	return name.get();
}

pid_t XWindow::getPID() const {
	xpp::Property<int> pid;

	this->getProperty(m_std_props.atom_ewmh_window_pid, pid);

	return pid.get();
}

int XWindow::getDesktop() const {
	xpp::Property<int> desktop_nr;

	this->getProperty(m_std_props.atom_ewmh_window_desktop, desktop_nr);

	auto ret = desktop_nr.get();
	m_cached_desktop_nr = ret;
	return ret;
}

int XWindow::getCachedDesktop() const {
	if (m_cached_desktop_nr != -1)
		return m_cached_desktop_nr;
	else
		return getDesktop();
}

void XWindow::setName(const std::string &name) {
	try {
		xpp::Property<utf8_string> utf8_name;
		utf8_name = utf8_string(name.c_str());

		this->setProperty(m_std_props.atom_ewmh_window_name, utf8_name);

		return;
	} catch(...) {}

	xpp::Property<const char*> name_prop(name.c_str());

	this->setProperty(m_std_props.atom_icccm_window_name, name_prop);
}

std::string XWindow::getClientMachine() const {
	xpp::Property<const char *> name;

	this->getProperty(m_std_props.atom_icccm_wm_client_machine, name);

	return name.get();
}

std::string XWindow::getCommand() const {
	xpp::Property<const char *> name;

	this->getProperty(m_std_props.atom_icccm_wm_command, name);

	return name.get();
}

std::string XWindow::getLocale() const {
	xpp::Property<const char *> locale;

	this->getProperty(m_std_props.atom_icccm_wm_locale, locale);

	return locale.get();
}

Window XWindow::getClientLeader() const {
	xpp::Property<Window> leader;

	this->getProperty(m_std_props.atom_icccm_wm_client_leader, leader);

	return leader.get();
}

Atom XWindow::getWindowType() const {
	xpp::Property<XAtom> type;

	this->getProperty(m_std_props.atom_ewmh_wm_window_type, type);

	return type.get();
}

void XWindow::getProtocols(AtomVector &protocols) const {
	protocols.clear();

	Atom *ret = nullptr;
	int ret_count = 0;

	const auto status = XGetWMProtocols(
		m_display,
		m_win,
		&ret,
		&ret_count
	);

	if (status == 0) {
		cosmos_throw(X11Exception(m_display, status));
	}

	for (int num = 0; num < ret_count; num++) {
		protocols.push_back(ret[num]);
	}

	XFree(ret);
}

std::shared_ptr<XWMHints> XWindow::getWMHints() const {
	auto hints = XGetWMHints(m_display, m_win);

	if (!hints) {
		return nullptr;
	}

	return make_shared_xptr(hints);
}

void XWindow::setWMHints(const XWMHints &hints) {
	// currently always returns 1, hints aren't modified in the lib
	(void)XSetWMHints(m_display, m_win, const_cast<XWMHints*>(&hints));
}

XWindow::ClassStringPair XWindow::getClass() const {
	/*
	 * there's a special pair of functions X{Set,Get}ClassHint but that
	 * would be more work for us, we get the raw property which consists
	 * of two consecutive null terminated strings
	 */
	xpp::Property<const char *> clazz;

	this->getProperty(m_std_props.atom_icccm_wm_class, clazz);

	ClassStringPair ret;

	ret.first = std::string(clazz.get());
	// this is not very safe but our Property modelling currently lacks
	// support for strings containing null terminators (we can't get the
	// complete size from the property)
	ret.second = std::string(clazz.get() + ret.first.length() + 1);

	return ret;
}

void XWindow::destroy() {
	const auto res = XDestroyWindow(m_display, m_win);
	m_display.flush();

	if (res != 1) {
		cosmos_throw(X11Exception(m_display, res));
	}
}

Window XWindow::createChild() {
	Window new_win = XCreateSimpleWindow(
		m_display,
		this->id(),
		// dimensions and alike don't matter for this hidden window
		-10, -10, 1, 1, 0, 0, 0
	);

	if (new_win == 0) {
		cosmos_throw(X11Exception("Failed to create pseudo child window"));
	}

	m_display.flush();

	return new_win;
}

void XWindow::convertSelection(
	const XAtom &selection,
	const XAtom &target_type,
	const XAtom &target_prop
)
{
	if (XConvertSelection(
		m_display,
		selection,
		target_type,
		target_prop,
		m_win,
		CurrentTime
	) != 1) {
		cosmos_throw(X11Exception("Failed to request selecton conversion"));
	}

	m_display.flush();
}

void XWindow::sendDeleteRequest() {
	long data[2];
	data[0] = m_std_props.atom_icccm_wm_delete_window;
	data[1] = CurrentTime;

	sendRequest(
		m_std_props.atom_icccm_wm_protocols,
		(const char*)&data[0],
		sizeof(data),
		this
	);
}

void XWindow::sendRequest(
	const XAtom &message,
	const char *data,
	const size_t len,
	const XWindow *window
) {
	auto &logger = Xpp::getLogger();

	logger.debug()
		<< "Sending request to window " << *this << ":"
		<< "msg = " << message << " with " << len << " bytes of data, window = "
		<< (window ? window->id() : 0) << std::endl;

	XEvent event;
	std::memset(&event, 0, sizeof(event));

	if (len > sizeof(event.xclient.data)) {
		cosmos_throw(X11Exception("XEvent data exceeds maximum"));
	}

	event.xclient.type = ClientMessage;
	event.xclient.serial = 0;
	event.xclient.send_event = True;
	event.xclient.message_type = message;
	event.xclient.window = window ? window->id() : 0;
	event.xclient.format = 32;
	std::memcpy(event.xclient.data.b, data, len);

	sendEvent(event);
}

void XWindow::sendEvent(const XEvent &event) {
	const Status s = XSendEvent(
		m_display,
		this->id(),
		False,
		m_send_event_mask,
		const_cast<XEvent*>(&event)
	);

	if (s == BadValue || s == BadWindow) {
		cosmos_throw(X11Exception(m_display, s));
	}

	// make sure the event gets sent out
	m_display.flush();
}

void XWindow::selectEvent(const long new_event) const {
	m_input_event_mask |= new_event;

	const int res = ::XSelectInput(
		XDisplay::getInstance(), m_win, m_input_event_mask
	);

	// stupid return codes again
	if (res == 0) {
		cosmos_throw(X11Exception("XSelectInput failed"));
	}
}

void XWindow::getPropertyList(AtomVector &atoms) {
	atoms.clear();

	int num_atoms = 0;

	Atom *list = XListProperties(
		XDisplay::getInstance(),
		m_win,
		&num_atoms
	);

	if (list == nullptr) {
		// could be an error (probably) or a window without any
		// properties
		return;
	}

	for (int i = 0; i < num_atoms; i++) {
		atoms.push_back(list[i]);
	}

	XFree(list);
}

void XWindow::getPropertyInfo(const XAtom &property, PropertyInfo &info) {
	int actual_format = 0;
	unsigned long number_items = 0;
	/*
	 * we don't want any data returned, but the function excepts valid
	 * pointers to pointers here, otherwise we segfault
	 */
	unsigned long bytes_left = 0;
	unsigned char *prop_data = nullptr;

	const auto res = XGetWindowProperty(
		XDisplay::getInstance(),
		m_win,
		property,
		0, /* offset */
		0, /* length */
		False, /* deleted property ? */
		AnyPropertyType,
		&info.type,
		&actual_format,
		&number_items,
		&bytes_left, /* bytes left to read */
		&prop_data /* output buffer to read into */
	);

	if (res != Success) {
		cosmos_throw(X11Exception(XDisplay::getInstance(), res));
	}

	if (actual_format != 0) {
		info.items = bytes_left / (actual_format / 8);
	} else {
		info.items = 0;
	}

	info.format = actual_format;

	XFree(prop_data);
}

template <typename PROPTYPE>
void XWindow::getProperty(
	const Atom name_atom,
	Property<PROPTYPE> &prop,
	const PropertyInfo *info
) const {
	// shorthand for our concrete property object
	typedef Property<PROPTYPE> THIS_PROP;

	Atom x_type = THIS_PROP::getXType();
	assert(x_type != None);

	Atom actual_type;
	// if 8, 16, or 32-bit format was actually read
	int actual_format = 0;
	unsigned long ret_items = 0;
	unsigned long remaining_bytes = 0;
	unsigned char *data = nullptr;

	const size_t max_len = info ?
		(info->items * (info->format / 8)) : 65536 / 4;

	const int res = XGetWindowProperty(
		XDisplay::getInstance(),
		m_win,
		name_atom,
		// offset into the property data
		0,
		// maximum length of the property to read in 32-bit items
		max_len,
		// delete request
		False,
		// our expected type
		x_type,
		// actually present type, format, number of items
		&actual_type,
		&actual_format,
		&ret_items,
		&remaining_bytes,
		// where data is stored
		&data
	);

	// note: on success data is memory allocated by Xlib. data always
	// contains one excess byte that is set to zero thus its possible to
	// use data as a c-string without copying it.
	if  (res != Success) {
		cosmos_throw(PropertyQueryError(XDisplay::getInstance(), res));
	}

	try {
		if (actual_type == None) {
			cosmos_throw(PropertyNotExisting());
		}
		else if (x_type != actual_type) {
			cosmos_throw(PropertyTypeMismatch(x_type, actual_type));
		}
		else if (remaining_bytes != 0) {
			cosmos_throw(cosmos::InternalError("Bytes remaining during property read"));
		}

		assert (actual_format == THIS_PROP::Traits::format);

		// ret_items gives the number of items acc. to actual_format that have
		// been returned
		prop.takeData(data, ret_items * (actual_format / 8));
	} catch(...) {
		XFree(data);
		throw;
	}
}

template <typename PROPTYPE>
void XWindow::setProperty(const Atom name_atom, const Property<PROPTYPE> &prop) {
	/*
	 * NOTE: currently only performs mode PropModeReplace
	 *
	 * I don't think that prepend or append are very common use cases.
	 */
	// shorthand for our concrete Property object
	typedef Property<PROPTYPE> THIS_PROP;

	Atom x_type = THIS_PROP::getXType();
	assert (x_type != None);

	const int siz = THIS_PROP::Traits::getNumElements(prop.get());

	const int res = XChangeProperty(
		m_display,
		m_win,
		name_atom,
		x_type,
		THIS_PROP::Traits::format,
		PropModeReplace,
		(unsigned char*)prop.getRawData(),
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
	m_display.flush();
}

void XWindow::delProperty(const Atom name_atom) {
	const auto status = XDeleteProperty(m_display, m_win, name_atom);

	if (status == 0) {
		cosmos_throw(X11Exception(m_display, status));
	}

	// see setProperty()
	m_display.flush();
}

void XWindow::getNextEvent(XEvent &event, const long event_mask) {
	const auto status = XWindowEvent(m_display, m_win, event_mask, &event);

	if (status == 0) {
		cosmos_throw(X11Exception(m_display, status));
	}
}

void XWindow::getAttrs(XWindowAttrs &attrs) {
	const auto status = XGetWindowAttributes(m_display, m_win, &attrs);

	// stupid error codes again. A non-zero status on success?
	if (status == 0) {
		cosmos_throw(X11Exception(m_display, status));
	}
}

void XWindow::moveResize(const XWindowAttrs &attrs) {
	const auto status = XMoveResizeWindow(
		m_display, m_win, attrs.x, attrs.y, attrs.width, attrs.height
	);

	if (status == 0) {
		cosmos_throw(X11Exception(m_display, status));
	}
}

void XWindow::updateFamily() {
	Window root = 0, parent = 0;
	Window *children = nullptr;
	unsigned int num_children = 0;

	m_children.clear();
	m_parent = 0;

	const Status res = XQueryTree(
		m_display, m_win, &root, &parent, &children, &num_children
	);

	if (res != 1) {
		cosmos_throw(X11Exception(m_display, res));
	}

	m_parent = parent;

	for (unsigned int i = 0; i < num_children; i++) {
		m_children.insert(children[i]);
	}

	XFree(children);
}

void XWindow::copyArea(
		const GcSharedPtr &gc, const PixMap &px,
		const Extent &ext, const Coord &src_pos, const Coord &dst_pos) {
	// does not return synchronous errors
	(void)XCopyArea(
		m_display, px.id(), m_win, &(*gc),
		src_pos.x, src_pos.y,
		ext.width, ext.height,
		dst_pos.x, dst_pos.y);
}

XWindow& XWindow::operator=(const XWindow &other) {
	m_win = other.m_win;
	m_parent = other.m_parent;
	m_children = other.m_children;
	m_input_event_mask = other.m_input_event_mask;
	m_send_event_mask = other.m_send_event_mask;
	m_cached_desktop_nr = other.m_cached_desktop_nr;
	return *this;
}

/*
 * explicit template instantiations
 *
 * allow to outline the above template code
 */
template void XWindow::getProperty(const Atom, Property<int>&, const PropertyInfo*) const;
template void XWindow::getProperty(const Atom, Property<unsigned long>&, const PropertyInfo*) const;
template void XWindow::getProperty(const Atom, Property<const char*>&, const PropertyInfo*) const;
template void XWindow::getProperty(const Atom, Property<std::vector<XAtom> >&, const PropertyInfo*) const;
template void XWindow::getProperty(const Atom, Property<std::vector<unsigned long> >&, const PropertyInfo*) const;
template void XWindow::getProperty(const Atom, Property<std::vector<int> >&, const PropertyInfo*) const;
template void XWindow::getProperty(const Atom, Property<std::vector<utf8_string> >&, const PropertyInfo*) const;
template void XWindow::setProperty(const Atom, const Property<const char*>&);
template void XWindow::setProperty(const Atom, const Property<int>&);
template void XWindow::setProperty(const Atom, const Property<utf8_string>&);

} // end ns

std::ostream& operator<<(std::ostream &o, const xpp::XWindow &w) {
	const auto f = o.flags();

	o << "0x" << std::setw(8)
		<< std::hex << std::setfill('0')
		<< w.id()
		<< std::dec << " (" << w.id() << ")";

	o.flags(f);

	return o;
}
