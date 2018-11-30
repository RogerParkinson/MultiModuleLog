/*
 * Logger.cpp
 *
 *  Created on: Nov 30, 2018
 *      Author: roger
 */

#include "Logger.h"

Logger::Logger(int level,const char *name, LoggerFactory *loggerFactory) {
	m_level = level;
	m_loggerFactory = loggerFactory;
	m_name = name;
}
void Logger::error(const char *msg, ...) {
	if (LOG_LEVEL_ERRORS <= m_level) {
		printName();
        va_list args;
        va_start(args, msg);
        m_loggerFactory->print(msg,args);
	}
}
void Logger::info(const char *msg, ...) {
	if (LOG_LEVEL_INFOS <= m_level) {
		printName();
        va_list args;
        va_start(args, msg);
        m_loggerFactory->print(msg,args);
	}
}
void Logger::debug(const char *msg, ...) {
	if (LOG_LEVEL_DEBUG <= m_level) {
		printName();
        va_list args;
        va_start(args, msg);
        m_loggerFactory->print(msg,args);
	}
}
void Logger::printName() {
	Serial.print("[");
	Serial.print(m_name);
	Serial.print("]");
}


