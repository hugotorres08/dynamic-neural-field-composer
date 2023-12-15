// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "logging/logger.h"

namespace dnf_composer
{
    Logger::Logger(LogLevel level)
	: logLevel(level)
	{}

    void Logger::log(const std::string& message) const
	{
        const auto now = std::chrono::system_clock::now();
        const auto in_time_t = std::chrono::system_clock::to_time_t(now);
        const std::string levelStr = getColorCode(logLevel);

        std::tm buf;
        if (localtime_s(&buf, &in_time_t))
            throw Exception(ErrorCode::LOG_LOCAL_TIME_ERROR);

        std::ostringstream oss;
        oss << levelStr << " " << std::put_time(&buf, "%Y-%m-%d %X") << " " << message;

        const std::string finalMessage = oss.str() + "\033[0m" + '\n'; // Reset color code

        std::cout << finalMessage;
        user_interface::LoggerWindow::addLog(finalMessage.c_str());
    }

    void log(LogLevel level, const std::string& message)
	{
#ifndef _DEBUG
		if(level == LogLevel::DEBUG)
			return;
#endif
        logger = Logger(level);
        logger.log(message);
    }

    std::string Logger::getColorCode(LogLevel level)
	{
        switch (level)
    	{
	        case DEBUG: return      "\033[97m[DEBUG]  "; // White
	        case INFO: return       "\033[97m[INFO]   ";   // White
	        case WARNING: return    "\033[93m[WARNING]"; // Yellow
	        case ERROR: return      "\033[91m[ERROR]  ";   // Red
	        case FATAL: return      "\033[91m[FATAL]  ";   // Red
	        default: return "";
        }
    }
}