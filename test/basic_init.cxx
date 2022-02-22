#include <iostream>

#include "X++/X++.hxx"
#include "X++/RootWin.hxx"
#include "cosmos/io/ILogger.hxx"

class DefLogger :
	public cosmos::ILogger {
public:

	DefLogger() {
		setStreams(std::cerr, std::cout, std::cerr, std::cerr);
		setChannels(true, true, true, true);
	}

};

int main() {

	DefLogger logger;
	xpp::Init init(&logger);
	xpp::RootWin root_win;
	root_win.getInfo();

	std::cout << "WM name: " << root_win.getWM_Name() << std::endl;;
	std::cout << "WM pid: " << root_win.getWM_Pid() << std::endl;

	xpp::XWindow::AtomVector props;
	root_win.getPropertyList(props);

	std::cout << "list of properties on root window: " << std::endl;

	for (const auto &atom: props) {
		std::cout << "- " << xpp::XAtom(atom) << "\n";
	}

	return 0;
}
