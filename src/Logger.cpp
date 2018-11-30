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
Logger *LoggerFactory::getLogger(const char *name) {
	int level = figureLevel(name);
	return new Logger(level, name, this);
}

int LoggerFactory::figureLevel(const char *name) {
	int ret = LOG_LEVEL_EVERYTHING; // ie everything
	for (int i=0;i<m_count;i++) {
		int cmp = strcmp(m_LoggerInstance[i].getName(),name);
		if (cmp == 0) {
			ret = m_LoggerInstance[i].getLevel();
//			Serial.print("using level ");
//			Serial.print(ret);
//			Serial.print(" for ");
//			Serial.println(name);
			return ret;
		}
	}
	return ret;
}
/*
 * This code was taken from https://github.com/mrRobot62/Arduino-logging-library
 */
void LoggerFactory::print(const char* format, va_list args) {
	//
	// loop through format string
	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			if (*format == '\0') break;
			if (*format == '%') {
				Serial.print(*format);
				continue;
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				Serial.print(s);
				continue;
			}
			if( *format == 'd' || *format == 'i') {
				Serial.print(va_arg( args, int ),DEC);
				continue;
			}
			if( *format == 'x' ) {
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}
			if( *format == 'X' ) {
				Serial.print("0x");
				Serial.print(va_arg( args, int ),HEX);
				continue;
			}
			if( *format == 'b' ) {
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}
			if( *format == 'B' ) {
				Serial.print("0b");
				Serial.print(va_arg( args, int ),BIN);
				continue;
			}
			if( *format == 'l' ) {
				Serial.print(va_arg( args, long ),DEC);
				continue;
			}

			if( *format == 'c' ) {
				Serial.print(va_arg( args, int ));
				continue;
			}
			if( *format == 't' ) {
				if (va_arg( args, int ) == 1) {
					Serial.print("T");
				}
				else {
					Serial.print("F");
				}
				continue;
			}
			if( *format == 'T' ) {
				if (va_arg( args, int ) == 1) {
					Serial.print("true");
				}
				else {
					Serial.print("false");
				}
				continue;
			}

		}
		Serial.print(*format);
	}
 }


