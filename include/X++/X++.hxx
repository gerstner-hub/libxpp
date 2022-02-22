#ifndef XPP_Xplusplus_HXX
#define XPP_Xplusplus_HXX

// stdlib
#include <optional>

namespace cosmos {
	class ILogger;
}

namespace xpp {

/**
 * \brief
 * 	Initializes the X++ library before first use
 * \details
 * 	The initialization of the library is required before any other
 * 	functionality of libX++ is accessed. This initialization should
 * 	occur after the main() function has been entered and not from within
 * 	static initializers to avoid issues with static initialization order
 * 	in executables / libraries.
 *
 * 	Multiple initializations can be performed but finishLibXpp() needs
 * 	to be called the same number of times for cleanup to occur.
 * \param[in-out] logger
 * 	If set then this Cosmos logger instance will be used for runtime error
 * 	or debugging messages presenting internal library state.
 **/
void XPP_API initLibXpp(std::optional<cosmos::ILogger*> logger);

void XPP_API finishLibXpp();

/**
 * \brief
 * 	Convenience initialization object
 * \details
 * 	During the lifetime of this object the cosmos library remains
 * 	initialized.
 **/
struct XPP_API Init {
	Init(std::optional<cosmos::ILogger*> logger) { initLibXpp(logger); }

	~Init() { finishLibXpp(); }
};

} // end ns

#endif // inc. guard
