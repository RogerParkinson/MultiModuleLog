/*
 * LoggerFactory.h
 *
 *  Created on: Nov 30, 2018
 *      Author: roger
 */

#ifndef LOGGERFACTORY_H_
#define LOGGERFACTORY_H_
//#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
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
	LoggerFactory(LoggerInst *loggerInstance, int count) {
		m_LoggerInstance = loggerInstance;
		m_count = count;
	}
	Logger *getLogger(const char *name);
	void print(const char* msg, va_list args);
};

#endif /* LOGGERFACTORY_H_ */
