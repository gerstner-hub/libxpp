// C++
#include <atomic>

// libX11
#include <X11/Xlib.h>

// cosmos
#include "cosmos/error/InternalError.hxx"

// X++
#include "X++/XDisplay.hxx"
#include "X++/PropertyTraits.hxx"
#include "X++/Xpp.hxx"
#include "X++/private/Xpp.hxx"

namespace xpp {

static std::atomic<std::size_t> g_init_counter;

void init(std::optional<cosmos::ILogger*> logger) {
	if (g_init_counter++ != 0)
		return;

	// this asks the Xlib to be thread-safe
	// be careful that this must be the first Xlib call in the process
	// otherwise it won't work!
	if (!::XInitThreads()) {
		cosmos_throw (cosmos::InternalError("Error initializing libX11 threads"));
	}

	// only now initialize global convenience variables
	xpp::display = XDisplay{};

	PropertyTraits<utf8_string>::init();
	PropertyTraits<std::vector<utf8_string>>::init();

	if (logger) {
		Xpp::getInstance().setLogger(**logger);
	}
}

void finish() {
	if (--g_init_counter != 0)
		return;

	// nothing to clean up currently
}


Xpp& Xpp::getInstance() {
	static Xpp instance;
	return instance;
}

Xpp::~Xpp() {
	cleanupNullLogger();
}

void Xpp::setupNullLogger() {
	class NullLogger :
			public cosmos::ILogger {
	public:
		NullLogger() {
			auto &noop = this->getNoopStream();
			this->setStreams(noop, noop, noop, noop);
		}
	};

	m_logger = new NullLogger();
	m_null_logger = true;
}

void Xpp::cleanupNullLogger() {
	if (!m_null_logger)
		return;

	delete m_logger;
	m_logger = nullptr;
	m_null_logger = false;
}

} // end ns
