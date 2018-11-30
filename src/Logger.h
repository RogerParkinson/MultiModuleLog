/*
 * Logger.h
 *
 *  Created on: Nov 30, 2018
 *      Author: roger
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdarg.h>
#include <Arduino.h>

class Logger;

class LoggerInst {
private:
	const char *m_name;
	int m_level;
public:
	LoggerInst(const char *name, const int level) {
		m_name = name;
		m_level = level;
	}
	const char *getName() {
		return m_name;
	}
	int getLevel() {
		return m_level;
	}
};
#define LOG_LEVEL_NOOUTPUT 0
#define LOG_LEVEL_ERRORS 1
#define LOG_LEVEL_INFOS 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_EVERYTHING 4

class LoggerFactory {
private:
	int figureLevel(const char *name);
	LoggerInst *m_LoggerInstance;
	int m_count;
public:
	LoggerFactory(LoggerInst *loggerInstance, int count) : LoggerFactory(loggerInstance,count,false){}
	LoggerFactory(LoggerInst *loggerInstance, int count, bool wait) {
		m_LoggerInstance = loggerInstance;
		m_count = count;
		if (wait) {
			while (!Serial) {
			    ; // wait for serial port to connect. Needed for native USB
			}
		}
	}
	Logger *getLogger(const char *name);
	void print(const char* msg, va_list args);
};


class Logger {
private:
	LoggerFactory *m_loggerFactory;
	int m_level;
	const char* m_name;
	void printName();
public:
	Logger(int level,const char *name,LoggerFactory *loggerFactory);
	void error(const char* msg, ...);
	void info(const char* msg, ...);
	void debug(const char* msg, ...);
};

extern LoggerFactory *loggerFactory;

#endif /* LOGGER_H_ */
