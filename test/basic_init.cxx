#include <iostream>

#include "X++/Xpp.hxx"
#include "X++/RootWin.hxx"
#include "X++/types.hxx"
#include "X++/helpers.hxx"
#include "cosmos/io/ILogger.hxx"
#include "cosmos/cosmos.hxx"
#include "cosmos/formatting.hxx"

class DefLogger :
	public cosmos::ILogger {
public:

	DefLogger() {
		setStreams(std::cerr, std::cout, std::cerr, std::cerr);
		setChannels(true, true, true, true);
	}

};

void testDisplay() {
	auto &display = xpp::XDisplay::getInstance();
	auto window = display.createWindow({0, 0, 100, 100}, 0);
	auto depth = display.getDefaultDepth();
	std::cout << "default depth: " << depth << std::endl;
	if (depth < 8 || depth > 32) {
		throw std::runtime_error("strange depth");
	}
	auto pmap = display.createPixmap(window, xpp::Extent{100, 100});
	display.freePixmap(pmap);
}

void testGC() {
	auto &display = xpp::XDisplay::getInstance();
	XGCValues vals;
	auto gc_ptr = display.createGraphicsContext(
			xpp::to_drawable(xpp::RootWin().id()), xpp::GcOptMask(), vals);
}

void test() {
	cosmos::Init cosmos_init;
	DefLogger logger;
	xpp::Init init(&logger);
	xpp::RootWin root_win;

	xpp::XAtomVector props;
	root_win.getPropertyList(props);

	std::cout << "list of properties on root window: " << std::endl;

	for (const auto &atom: props) {
		std::cout << "- " << atom << "\n";
	}

	testDisplay();

	testGC();
}

int main() {
	try {
		test();
		return 0;
	} catch (const std::exception &ex) {
		std::cerr << "test failed: " << ex.what() << std::endl;
		return 1;
	}
}
