/*
 * Logger.h
 *
 *  Created on: Nov 30, 2018
 *      Author: roger
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "LoggerFactory.h"

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

#endif /* LOGGER_H_ */
