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
	friend class LoggerFactory;
private:
	const char *m_name;
	int m_level;
protected:
	LoggerInst *m_loggerInstance;
	const char *getName() {
		return m_name;
	}
	int getLevel() {
		return m_level;
	}
	LoggerInst(const char *name, const int level) {
		m_loggerInstance = NULL;
		m_name = name;
		m_level = level;
	}
public:
};

#define LOG_LEVEL_NOOUTPUT 0
#define LOG_LEVEL_ERRORS 1
#define LOG_LEVEL_INFOS 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_EVERYTHING 4
#define LOG_UNCONFIGURED 5

class LoggerFactory {
	friend class Logger;
private:
	LoggerInst *m_LoggerInstance;
protected:
	int figureLevel(const char *name);
	void print(const char* msg, va_list args);
public:
	LoggerFactory() : LoggerFactory(false){}
	LoggerFactory(bool wait) {
		m_LoggerInstance = NULL;
		if (wait) {
			while (!Serial) {
			    ; // wait for serial port to connect. Needed for native USB
			}
		}
	}
	void add(const char *name, const int level);
	Logger *getLogger(const char *name);
	void dump();
};

class Logger {
	friend class LoggerFactory;
private:
	LoggerFactory *m_loggerFactory;
	int m_level;
	const char* m_name;
	void printName(const char *level);
	void figureLevel();
protected:
	Logger(const char *name,LoggerFactory *loggerFactory);
public:
	void error(const char* msg, ...);
	void info(const char* msg, ...);
	void debug(const char* msg, ...);
	boolean isError() {return LOG_LEVEL_ERRORS <= m_level;}
	boolean isInfo() {return LOG_LEVEL_INFOS <= m_level;}
	boolean isDebug() {return LOG_LEVEL_DEBUG <= m_level;}
};

extern LoggerFactory loggerFactory;

#endif /* LOGGER_H_ */
