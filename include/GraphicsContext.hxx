#pragma once

// Cosmos
#include "cosmos/BitMask.hxx"

// X++
#include "X++/dso_export.h"
#include "X++/fwd.hxx"
#include "X++/types.hxx"

namespace xpp {

/// type safe wrapper enum for GC* constants found in X headers
/**
 * These are bitmask values, see X.h
 **/
enum class GcOpts : long {
	FUNCTION           = GCFunction,
	PLANE_MASK         = GCPlaneMask,
	FOREGROUND         = GCForeground,
	BACKGROUND         = GCBackground,
	LINE_WIDTH         = GCLineWidth,
	LINE_STYLE         = GCLineStyle,
	CAP_STYLE          = GCCapStyle,
	JOIN_STYLE         = GCJoinStyle,
	FILL_STYLE         = GCFillStyle,
	FILL_RULE          = GCFillRule,
	TILE               = GCTile,
	STIPPLE            = GCStipple,
	TILE_STIP_XORIGIN  = GCTileStipXOrigin,
	TILE_STIP_YORIGIN  = GCTileStipYOrigin,
	FONT               = GCFont,
	SUB_WINDOW_MODE    = GCSubwindowMode,
	GRAPHICS_EXPOSURES = GCGraphicsExposures,
	CLIP_XORIGIN       = GCClipXOrigin,
	CLIP_YORIGIN       = GCClipYOrigin,
	CLIP_MASK          = GCClipMask,
	DASH_OFFSET        = GCDashOffset,
	DASH_LIST          = GCDashList,
	ARC_MODE           = GCArcMode
};

using GcOptMask = cosmos::BitMask<GcOpts>;

/// Represents an X11 GC graphics context resource.
/**
 * This is a move only type, since there is a resource behind this object that
 * needs to be freed at the appropriate time.
 **/
class XPP_API GraphicsContext {
	GraphicsContext(const GraphicsContext&) = delete;
	GraphicsContext& operator=(const GraphicsContext&) = delete;
public: // functions

	GraphicsContext() = default;

	/// Creates a new graphics context using the given settings
	/**
	 * A graphics context is used for specifying options when operating on
	 * a target drawable. It is allocated internally in Xlib, therefore it
	 * also has to be explicitly freed. Freeing happens during destruction
	 * or explicitly using destroy().
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
	 **/
	GraphicsContext(
		DrawableID d,
		const GcOptMask mask,
		const XGCValues &vals,
		XDisplay &disp = xpp::display);

	~GraphicsContext() {
		if (valid()) {
			destroy();
		}
	}

	GraphicsContext(GraphicsContext &&other) {
		*this = std::move(other);
	}

	GraphicsContext& operator=(GraphicsContext &&other) {
		m_display = other.m_display;
		m_gc = other.m_gc;

		other.invalidate();
		return *this;
	}

	void setForeground(const ColormapIndex index);

	void setBackground(const ColormapIndex index);

	bool valid() const { return m_gc != nullptr; }

	operator GC() const { return m_gc; }

	void destroy();

protected: // functions

	void invalidate() {
		m_display = nullptr;
		m_gc = nullptr;
	}

protected: // data
	XDisplay *m_display = nullptr;
	GC m_gc = nullptr;
};

} // end ns
