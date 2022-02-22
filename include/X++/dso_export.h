#ifndef XPP_EXPORT_HXX
#define XPP_EXPORT_HXX

/*
 * Helper macro to control visibility of public APIs of the X++ shared
 * library.
 * The object files are compiled with -fvisibility=hidden, thus each public
 * interface needs to be explicitly made visible again.
 */
#if defined(XPP_EXPORT)
#	define XPP_API __attribute__ ((visibility ("default")))
#else
#	define XPP_API
#endif

#endif // inc. guard
