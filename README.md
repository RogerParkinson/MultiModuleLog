# ArduinoLogger

[![build_status](https://travis-ci.org/RogerParkinson/ArduinoLogger.svg?branch=master)](https://travis-ci.org/RogerParkinson/ArduinoLogger)
[![License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](http://doge.mit-license.org)

Arduino library for logging to Serial. It is modelled (slightly) on Java's slf4j. Notably:

 * Use a format string and optional extra arguments to merge into the format string before output.
 * Don't do any merging until we know this call will actually log.
 * Multiple Loggers, each with a different name and each can have a different logging level.
 * Optionally wait for Serial to conect before contining.
 * Obviously don't log if the level doesn't match the call.
 
## Why another logging library?

It is true there are several others out there. I really liked Arduino-logging-library for its format strings and merging arguments and I grabbed that code for this.

But none of them (that I found) support multiple loggers. This makes sense, most Arduino projects are too small to need them. But the ones I write tend to have lots of objects and when you have more than 10 you want to be able turn off sections of the logging based on the class or module or similar while leaving the other logging on.

That's what this does. You create a logger for, say, a class and use that to log everything for that class. When you no longer need logging for that class you set its logging to a lower level and keep any other loggers untouched.

It is only going be be useful when you have a large project. Use one of the other loggers on small projects.

## Configuration 
 
The log levels are:

 * No output
 * Errors
 * Info
 * Debug
 * Everything (which is the default if you don't give a level).

You set these by configuring the LogFactory like this:

```
LoggerFactory *loggerFactory;
LoggerInst loggerInstance[] = {
		LoggerInst("A",LOG_LEVEL_ERRORS),
		LoggerInst("B",LOG_LEVEL_INFOS)
};
loggerFactory = new LoggerFactory(loggerInstance,2,true);
```

This configures two loggers, naming the (A and B) and giving each a logging level. Then we create a new LoggerFactory passing the configuration, the number of lines and, optionally, a flag to wait until the Serial Monitor is connected before going on.

After that each module should create its own logger like this:

```
Logger *loggerA = loggerFactory->getLogger("A");
loggerA->debug("debug message\n");
```

Each Logger is very light weight so you can create them cheaply. All the smart stuff is in the LoggerFactory singleton.

Because the configuration for A will only log errors that call to debug will be suppressed.

## Formatting

It is worth saying here that the logger *never* prints a new line. If you want a new line then add a \\n to the end. It always prints the module name at the start of the line (or at the start of each call) so the debug message above will look like this:

```
[A] debug message
```

That way you know where the message came from.

The formatting is always done *after* the decision is made to go ahead and log, this is important because we don't want to consume resources formatting things that we won't be printing.

The formatting supports printf symbols eg:

```
loggerA->debug("debug message %s %d\n", "somestring", 10);
```

This is using the code from Arduino-logging-library, and this is the full list of symbols:

 * %s replace with an string (char*)
 * %c replace with an character
 * %d replace with an integer value
 * %l replace with an long value
 * %x replace and convert integer value into hex
 * %X like x but combine with 0x123AB
 * %b replace and convert integer value into binary
 * %B like x but combine with 0b10100011
 * %t replace and convert boolean value into "t" or "f"
 * %T like t but convert into "true" or "false"

## Multiple files

The whole point of this is to make it easier to manage logging when you have lots of files with logging spread all over them. So it is worth making sure you know how to manage that.

In your main file, the one with setup() and Loop() you need to have this:
```
#include <Logger.h>
...
LoggerFactory *loggerFactory;
```

You should create and configure your LoggerFactory in your setup() method, just after you start your Serial interface. Something like this:

```
#include <Logger.h>

LoggerFactory *loggerFactory;

void setup() {
	Serial.begin(9600);
	LoggerInst loggerInstance[] = {
		LoggerInst("A",LOG_LEVEL_ERRORS),
		LoggerInst("B",LOG_LEVEL_INFOS)
	};

	loggerFactory = new LoggerFactory(loggerInstance,2, true);
	loggerA = loggerFactory->getLogger("A");
	...
```
## Install

 * Download latest from Github
 * Extract it to the libraries folder inside your Sketchbook. Default is [user]\Arduino\libraries.
 * Restart Arduino IDE if you had it open.
 * Done

## Credit

Based on Arduino-logging-library by 
* [/mrRobot62](https://github.com/mrRobot62)  

