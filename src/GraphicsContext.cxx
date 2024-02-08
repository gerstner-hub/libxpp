// xpp
#include "xpp/GraphicsContext.hxx"
#include "xpp/XDisplay.hxx"

// Cosmos
#include "cosmos/error/RuntimeError.hxx"

namespace xpp {

GraphicsContext::GraphicsContext(DrawableID d, const GcOptMask mask, const XGCValues &vals, XDisplay &disp) :
		m_display(&disp) {
	m_gc = ::XCreateGC(disp, cosmos::to_integral(d), mask.raw(), const_cast<XGCValues*>(&vals));

	if (!m_gc) {
		cosmos_throw (cosmos::RuntimeError("failed to allocate GC"));
	}
}

void GraphicsContext::destroy() {
	if (m_gc) {
		::XFreeGC(*m_display, m_gc);
		invalidate();
	}
}

void GraphicsContext::setForeground(const ColormapIndex index) {
	::XSetForeground(*m_display, m_gc, cosmos::to_integral(index));
}

void GraphicsContext::setBackground(const ColormapIndex index) {
	::XSetBackground(*m_display, m_gc, cosmos::to_integral(index));
}

} // end ns
