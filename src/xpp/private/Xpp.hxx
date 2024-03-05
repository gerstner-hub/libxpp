#pragma once

// xpp
#include <xpp/Xpp.hxx>

// cosmos
#include <cosmos/io/ILogger.hxx>

namespace xpp {

/// Holds global libxpp library state
class Xpp {
public: // functions

	~Xpp();

	static Xpp& getInstance();
	static cosmos::ILogger& getLogger() {
		return Xpp::getInstance().getSomeLogger();
	}

protected: // functions

	friend void init(std::optional<cosmos::ILogger*>);

	//! protected constructor to enforce singleton usage
	Xpp() {};

	cosmos::ILogger& getSomeLogger() {
		if (!m_logger) {
			setupNullLogger();
		}
		return *m_logger;
	}

	void setLogger(cosmos::ILogger &logger) {
		cleanupNullLogger();
		m_logger = &logger;
	}
	void setupNullLogger();
	void cleanupNullLogger();

protected: // data
	bool m_null_logger = false;
	cosmos::ILogger *m_logger = nullptr;
};

} // end ns
