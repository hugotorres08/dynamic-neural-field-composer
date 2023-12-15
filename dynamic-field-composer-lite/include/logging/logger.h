#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

#include "exceptions/exception.h"
#include "user_interface/logger_window.h"

namespace dnf_composer
{
	enum LogLevel : int
	{
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	class Logger
	{
	private:
		LogLevel logLevel;
	public:
		Logger(LogLevel level);
		void log(const std::string& message) const;
	private:
		static std::string getColorCode(LogLevel level);
	};

	void log(LogLevel level, const std::string& message);

	static Logger logger(INFO);
}