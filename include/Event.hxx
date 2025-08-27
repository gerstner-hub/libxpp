#pragma once

// C++
#include <optional>

// xlib
#include <X11/Xlib.h>

// cosmos
#include <cosmos/utils.hxx>

// xpp
#include <xpp/dso_export.h>
#include <xpp/helpers.hxx>
#include <xpp/XWindow.hxx>

namespace xpp {

/// Wraps an XEvent libX11 union
/**
 * The wrapper provides safe cast helpers like toConfigureNotify() that return
 * the properly typed variant of the union but throw an exception if the type
 * field doesn't match.
 **/
class XPP_API Event {
public: // functions

	explicit Event(const XEvent &ev) :
			m_ev{ev} {}

	explicit Event(const EventType type = EventType::INVALID) {
		m_ev.type = cosmos::to_integral(type);
	}

	XEvent* raw() { return &m_ev; }
	const XEvent* raw() const { return &m_ev; }

	/// Check whether the given event should be filtered.
	/**
	 * Certain layers above libX11 (like XOpenIM) make use of events on
	 * their own. To avoid getting confused by these events and to make
	 * these layers work correctly this call should be used to check
	 * whether this is the case for this event.
	 *
	 * If no window is passed then the window value stored in the event
	 * structure will be used.
	 *
	 * \return `true` if the event is to be filtered, `false` if the
	 *         caller should continue processing the event.
	 **/
	bool filterEvent(std::optional<const XWindow> w = std::nullopt) const {
		// the call shouldn't modify the event, thus const_cast it
		//
		// if it *is* modified then only if filtering applies in which
		// case the event shouldn't be processed by the caller anyway
		auto filter = ::XFilterEvent(const_cast<XEvent*>(&m_ev),
				w ? raw_win(w->id()) : None);
		return filter == True;
	}

	bool isConfigureNotify() const  { return type() == EventType::CONFIGURE_NOTIFY; }
	bool isMapNotify() const        { return type() == EventType::MAP_NOTIFY; }
	bool isUnmapNotify() const      { return type() == EventType::UNMAP_NOTIFY; }
	bool isVisibilityNotify() const { return type() == EventType::VISIBILITY_NOTIFY; }
	bool isFocusChange() const      { return type() == EventType::FOCUS_IN || type() == EventType::FOCUS_OUT; }
	bool isKeyPress() const         { return type() == EventType::KEY_PRESS; }
	bool isClientMessage() const    { return type() == EventType::CLIENT_MESSAGE; }
	bool isButtonEvent() const      { return type() == EventType::BUTTON_RELEASE || type() == EventType::BUTTON_PRESS; }
	bool isPointerMovedEvent() const { return type() == EventType::MOTION_NOTIFY; }
	bool isPropertyNotify() const   { return type() == EventType::PROPERTY_NOTIFY; }
	bool isSelectionNotify() const  { return type() == EventType::SELECTION_NOTIFY; }
	bool isSelectionRequest() const { return type() == EventType::SELECTION_REQUEST; }
	bool isSelectionClear() const   { return type() == EventType::SELECTION_CLEAR; }
	bool isCreateNotify() const     { return type() == EventType::CREATE_NOTIFY; }
	bool isDestroyNotify() const    { return type() == EventType::DESTROY_NOTIFY; }
	bool isReparentNotify() const   { return type() == EventType::REPARENT_NOTIFY; }

	EventType type() const { return EventType{m_ev.type}; }

	// this allows to access some generic data members present on all events
	auto& toAnyEvent() {
		return m_ev.xany;
	}

	auto& toAnyEvent() const {
		return unconst().toAnyEvent();
	}

	auto& toReparentNotify() {
		onMismatch(isReparentNotify());
		return m_ev.xreparent;
	}

	auto& toReparentNotify() const {
		return unconst().toReparentNotify();
	}

	auto& toMapNotify() {
		onMismatch(isMapNotify());
		return m_ev.xmap;
	}

	auto& toMapNotify() const {
		return unconst().toMapNotify();
	}

	auto& toUnmapNotify() {
		onMismatch(isUnmapNotify());
		return m_ev.xunmap;
	}

	auto& toUnmapNotify() const {
		return unconst().toUnmapNotify();
	}

	auto& toDestroyNotify() {
		onMismatch(isDestroyNotify());
		return m_ev.xdestroywindow;
	}

	auto& toDestroyNotify() const {
		return unconst().toDestroyNotify();
	}

	auto& toCreateNotify() {
		onMismatch(isCreateNotify());
		return m_ev.xcreatewindow;
	}

	auto& toCreateNotify() const {
		return unconst().toCreateNotify();
	}

	auto& toConfigureNotify() {
		onMismatch(isConfigureNotify());
		return m_ev.xconfigure;
	}

	auto& toConfigureNotify() const {
		return unconst().toConfigureNotify();
	}

	auto& toVisibilityNotify() {
		onMismatch(isVisibilityNotify());
		return m_ev.xvisibility;
	}

	auto& toVisibilityNotify() const {
		return unconst().toVisibilityNotify();
	}

	auto& toFocusChangeEvent() {
		onMismatch(isFocusChange());
		return m_ev.xfocus;
	}

	auto& toFocusChangeEvent() const {
		return unconst().toFocusChangeEvent();
	}

	auto& toKeyEvent() {
		onMismatch(isKeyPress());
		return m_ev.xkey;
	}

	auto& toKeyEvent() const {
		return unconst().toKeyEvent();
	}

	auto& toClientMessage() {
		onMismatch(isClientMessage());
		return m_ev.xclient;
	}

	auto& toClientMessage() const {
		return unconst().toClientMessage();
	}

	auto& toButtonEvent() {
		onMismatch(isButtonEvent());
		return m_ev.xbutton;
	}

	auto& toButtonEvent() const {
		return unconst().toButtonEvent();
	}

	auto& toPropertyNotify() {
		onMismatch(isPropertyNotify());
		return m_ev.xproperty;
	}

	auto& toPropertyNotify() const {
		return unconst().toPropertyNotify();
	}

	auto& toSelectionRequest() {
		onMismatch(isSelectionRequest());
		return m_ev.xselectionrequest;
	}

	const auto& toSelectionRequest() const {
		return unconst().toSelectionRequest();
	}

	auto& toSelectionNotify() {
		onMismatch(isSelectionNotify());
		return m_ev.xselection;
	}

	auto& toSelectionNotify() const {
		return unconst().toSelectionNotify();
	}

	auto& toSelectionClear() {
		onMismatch(isSelectionClear());
		return m_ev.xselectionclear;
	}

	auto& toSelectionClear() const {
		return unconst().toSelectionClear();
	}

	auto& toPointerMovedEvent() {
		onMismatch(isPointerMovedEvent());
		return m_ev.xmotion;
	}

	auto& toPointerMovedEvent() const {
		return unconst().toPointerMovedEvent();
	}

protected: // functions

	/// Helper to support code reuse in to*() accessors above
	Event& unconst() const { return const_cast<Event&>(*this); }

	void onMismatch(const bool matches) const {
		if (!matches)
			raiseMismatch();
	}

	void raiseMismatch() const;

	void clear();

protected: // data

	XEvent m_ev;
};

} // end ns
