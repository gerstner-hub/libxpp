#include <iostream>

#include "X++/AtomMapper.hxx"
#include "X++/Xpp.hxx"
#include "X++/RootWin.hxx"
#include "X++/types.hxx"
#include "cosmos/io/StdLogger.hxx"
#include "cosmos/cosmos.hxx"

void printInfo(const xpp::XWindow::PropertyInfo &info, xpp::AtomMapper &mapper) {
	std::cout << "property type: " << mapper.mapName(xpp::AtomID{info.type}) << std::endl;
	std::cout << "number of items: " << info.items << std::endl;
	std::cout << "format: " << info.format << std::endl;
	std::cout << "bytes: " << info.numBytes() << std::endl;
}

void getRawProp(xpp::RootWin root_win, xpp::AtomID prop, const xpp::XWindow::PropertyInfo &info) {
	auto &mapper = xpp::atom_mapper;
	auto prop_name = mapper.mapName(prop);

	std::cout << "Got info about " << prop_name << std::endl;
	printInfo(info, mapper);

	xpp::RawProperty rprop;
	rprop.length = info.numBytes() >> 1;
	rprop.length &= ~(0x3);
	rprop.offset = 4;
	std::cout << "Asking for " << rprop.length << " bytes of property at offset " << rprop.offset << "\n";
	xpp::XWindow::PropertyInfo raw_info;
	root_win.getRawProperty(prop, raw_info, rprop);

	std::cout << "Retrieved " << rprop.length << " bytes of property at offset " << rprop.offset << "\n";
	std::cout << "remaining: " << rprop.left << "\n";

	std::cout << "dump of received data:\n";
	auto data = rprop.data.get();
	for (size_t byte = 0; byte < rprop.length; byte++) {
		std::cout << data[byte];
	}
	std::cout << std::endl;

	printInfo(raw_info, mapper);
}

void test() {
	cosmos::Init cosmos_init;
	cosmos::StdLogger logger;
	xpp::Init init(&logger);
	xpp::RootWin root_win;

	xpp::AtomIDVector atoms;
	root_win.getPropertyList(atoms);

	bool found = false;

	for (const auto prop: atoms) {
		xpp::XWindow::PropertyInfo info;
		root_win.getPropertyInfo(prop, info);

		if (info.items == 0)
			continue;

		found = true;
		getRawProp(root_win, prop, info);
		break;
	}

	if (!found) {
		std::cout << "no suitable properties found on root window!";
	}
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
