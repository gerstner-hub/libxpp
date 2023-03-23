#include <iostream>

#include "cosmos/cosmos.hxx"
#include "cosmos/formatting.hxx"
#include "cosmos/io/StdLogger.hxx"
#include "X++/GraphicsContext.hxx"
#include "X++/Pixmap.hxx"
#include "X++/RootWin.hxx"
#include "X++/Xpp.hxx"
#include "X++/formatting.hxx"
#include "X++/helpers.hxx"
#include "X++/types.hxx"

void testDisplay() {
	auto &display = xpp::display;
	auto window = display.createWindow({0, 0, 100, 100}, 0);
	auto depth = display.defaultDepth();
	std::cout << "default depth: " << depth << std::endl;
	if (depth < 8 || depth > 32) {
		throw std::runtime_error("strange depth");
	}
	xpp::Pixmap pm{window, xpp::Extent{100, 100}};
	pm.destroy();
}

void testGC() {
	XGCValues vals;
	xpp::GraphicsContext gc{
		xpp::to_drawable(xpp::RootWin().id()),
		xpp::GcOptMask(),
		vals};
	gc.destroy();
}

void test() {
	cosmos::Init cosmos_init;
	cosmos::StdLogger logger;
	xpp::Init init(&logger);
	xpp::RootWin root_win;

	xpp::AtomIDVector props;
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
