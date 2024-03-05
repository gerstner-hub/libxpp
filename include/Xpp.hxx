#pragma once

// C++
#include <optional>

// xpp
#include <xpp/dso_export.h>

namespace cosmos {
	class ILogger;
}

/**
 * @file
 *
 * Library initialization helpers.
 **/

namespace xpp {

/// Initializes the xpp library before first use.
/**
 * The initialization of the library is required before any other
 * functionality of libxpp is accessed. This initialization should occur after
 * the main() function has been entered and not from within static
 * initializers to avoid issues with static initialization order in
 * executables / libraries.
 *
 * Multiple initializations can be performed but finishLibXpp() needs to be
 * called the same number of times for cleanup to occur.
 *
 * \param[in,out] logger If set then this Cosmos logger instance will be used
 * for runtime error or debugging messages presenting internal library state.
 **/
void XPP_API init(std::optional<cosmos::ILogger*> logger);

void XPP_API finish();

/// Convenience initialization object.
/**
 * During the lifetime of this object the cosmos library remains initialized.
 **/
struct XPP_API Init {
	explicit Init(std::optional<cosmos::ILogger*> logger = std::nullopt) { init(logger); }

	~Init() { finish(); }
};

} // end ns
