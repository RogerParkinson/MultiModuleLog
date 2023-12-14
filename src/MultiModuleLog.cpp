/*
 * MultiModuleLog.cpp
 *
 *  Created on: Nov 30, 2018
 *      Author: roger
 */

#include "MultiModuleLog.h"

Logger::Logger(const char *name, LoggerFactory *loggerFactory) {
	m_level = LOG_UNCONFIGURED;
	m_loggerFactory = loggerFactory;
	m_name = name;
}
void Logger::error(const char *msg, ...) {
	figureLevel();
	if (LOG_LEVEL_ERRORS <= m_level) {
		printName("error");
        va_list args;
        va_start(args, msg);
        m_loggerFactory->print(msg,args);
	}
}
void Logger::info(const char *msg, ...) {
	figureLevel();
	if (LOG_LEVEL_INFOS <= m_level) {
		printName("info");
        va_list args;
        va_start(args, msg);
        m_loggerFactory->print(msg,args);
	}
}
void Logger::debug(const char *msg, ...) {
	figureLevel();
	if (LOG_LEVEL_DEBUG <= m_level) {
		printName("debug");
        va_list args;
        va_start(args, msg);
        m_loggerFactory->print(msg,args);
	}
}

void Logger::figureLevel() {
	if (m_level == LOG_UNCONFIGURED) {
		m_level = m_loggerFactory->figureLevel(m_name);
	}
}

void Logger::printName(const char*level) {
	Serial.print("[");
	Serial.print(m_name);
	Serial.print("]");
	Serial.print("[");
	Serial.print(level);
	Serial.print("]");
}

void LoggerFactory::add(const char *name, const int level) {
	LoggerInst *loggerInst = new LoggerInst(name,level);
	if (m_LoggerInstance == NULL) {
		m_LoggerInstance = loggerInst;
		return;
	}
	loggerInst->m_loggerInstance = m_LoggerInstance;
	m_LoggerInstance = loggerInst;
}

Logger *LoggerFactory::getLogger(const char *name) {
	return new Logger(name, this);
}

int LoggerFactory::figureLevel(const char *name) {
	if (!m_active) {
		return LOG_LEVEL_NOOUTPUT;
	}
	int ret = LOG_UNCONFIGURED; // ie everything
	LoggerInst *inst = m_LoggerInstance;
	while (inst != NULL) {
		int cmp = strcmp(inst->getName(),name);
		if (cmp == 0) {
			ret = inst->getLevel();
//			Serial.print("using level ");
//			Serial.print(ret);
//			Serial.print(" for ");
//			Serial.println(name);
			return ret;
		}
		inst = inst->m_loggerInstance;
	}
	return ret;
}

void LoggerFactory::dump() {
	LoggerInst *inst = m_LoggerInstance;
	Serial.println("Dumping log configuration");
	while (inst != NULL) {
		Serial.print("using level ");
		Serial.print(inst->getLevel());
		Serial.print(" for ");
		Serial.println(inst->getName());
		inst = inst->m_loggerInstance;
	}
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
	Serial.println();
 }


