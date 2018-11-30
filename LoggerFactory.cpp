/*
 * LoggerFactory.cpp
 *
 *  Created on: Nov 30, 2018
 *      Author: roger
 */

#include "LoggerFactory.h"
#include "Logger.h"

Logger *LoggerFactory::getLogger(const char *name) {
	int level = figureLevel(name);
	return new Logger(level, name, this);
}

int LoggerFactory::figureLevel(const char *name) {
	int ret = LOG_LEVEL_EVERYTHING; // ie everything
//	Serial.print("figuring level for [");
//	Serial.print(name);
//	Serial.println("]");
	for (int i=0;i<m_count;i++) {
		int cmp = strcmp(m_LoggerInstance[i].getName(),name);
//		Serial.print(i);
//		Serial.print(" [");
//		Serial.print(m_LoggerInstance[i].getName());
//		Serial.print("] ");
//		Serial.print(cmp);
//		Serial.println("");
		if (cmp == 0) {
			ret = m_LoggerInstance[i].getLevel();
			Serial.print("using level ");
			Serial.print(ret);
			Serial.print(" for ");
			Serial.println(name);
			return ret;
		}
	}
//	Serial.print("using default level ");
//	Serial.print(ret);
//	Serial.print(" for ");
//	Serial.println(name);
	return ret;
}
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
