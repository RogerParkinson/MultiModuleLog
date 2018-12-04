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

It is true there are several others out there. I really liked Arduino-logging-library for its format strings and merging arguments and I grabbed that code for this (see credit below).

But none of them (that I found) support multiple loggers. This makes sense, most Arduino projects are too small to need them. But the ones I write tend to have lots of objects and when you have more than 10 files you want to be able turn off sections of the logging based on the class or module or similar while leaving the other logging on.

That's what this does. You create a logger for, say, a class and use that to log everything for that class. When you no longer need logging for that class you set its logging to a lower level and keep any other loggers untouched.

It is only going be be useful when you have a large project. Use one of the other loggers on small projects.

## Configuration 
 
The log levels are:

 * No output
 * Errors
 * Info
 * Debug
 * Everything (which is the default if you don't give a level).

Start with this:

```
#include <Logger.h>
#include "TestObject.h"

Logger *loggerA;
Logger *loggerB;
LoggerFactory loggerFactory = LoggerFactory(true);
```

Include the library file. In this case I have another file that defines a class called TestObject and I include that too. You'll see why later.
Then I've declared two different Logger variables as well as the logger factory.

Somewhere near the top of your setup() function do something like this:

```
Serial.begin(9600);
// Configure the loggers, including the one for TestObject. 
// Come back here when you want to turn logging on/off
loggerFactory.add("A",LOG_LEVEL_ERRORS);
loggerFactory.add("B",LOG_LEVEL_INFOS);
loggerFactory.add("TestObject",LOG_LEVEL_INFOS);

// Now create the two Loggers (but not the one for TestObject)
loggerA = loggerFactory.getLogger("A"); // only log errors
loggerB = loggerFactory.getLogger("B"); // default is log everything
```

This configures two loggers, naming the (A and B) and giving each a logging level. Each Logger is very light weight so you can create them cheaply. All the smart stuff is in the LoggerFactory singleton. 

## Usage

Now lets do some logging:

```
int myInt1 = 232;
int myInt2 = 32199;
TestObject t; // TestObject has its own logger

loggerA->info("error Display my integers myInt1 %d, myInt2 %d",myInt1,myInt2); // Should be suppressed
loggerA->debug("debug message"); // Should be suppressed
loggerB->info("info Display my integers myInt1 %d, myInt2 %d",myInt1,myInt2); // Should be visible
loggerB->debug("debug Display my integers myInt1 %d, myInt2 %d",myInt1,myInt2); // Should be suppressed
t.testMethod();
```

Ignore TestObject for now, notice LoggerA and LoggerB are being called to log different kinds of messages. These correspond to the log levels. But first notice that each call is able to use a format string and a list of arguments. There is a section below on formatting.

The logger knows what level it was configured to accept and it prints messages for that level and above. So LoggerA will only print messages for *error* and will ignore the *info* message sent here. Importantly it will not try formatting messages that it is going to ignore, so there is little overhead for messages that are turned off. LoggerB is set to print *info* messages so it will print the first message but not the second which is a *debug* message.

Ideally your logging messages are never more complext than this but sometimes they might be and if you need to you can query the current log level like this:

```
if (loggerA->isDebug()) {
	... 
	some resource heavy operation
	...
	loggerA->debug(...) // log the result
} 
```

Being able to query the logging level of the logger allows you to skip the resource heavy operation when you wouldn't logi it anyway.

## Formatting

It is worth saying here that the logger always prints a new line at the end. It also always prints the module name and the level at the start of the line (or at the start of each call) so the debug message above will look like this:

```
[A][debug] debug message
```

That way you know where the message came from. You can add new line characters in your format strings and those will give new lines without the above prefixes.

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
 
As with printf if you mismatch your format rags and your arguments there will be strange errors, so take some care to get them right.

## Multiple files

The whole point of this is to make it easier to manage logging when you have lots of files with logging spread all over them. So it is worth making sure you know how to manage that. The example above showed us using a TestObject. This is in another two files, TestObject.h and TestObject.cpp.

```
//TestObject.h
#include <Logger.h>

class TestObject {
public:
	TestObject();
	void testMethod();
};
```

```
//TestObject.cpp
#include "TestObject.h"

Logger *loggerTestObject = loggerFactory.getLogger("TestObject");

TestObject::TestObject(){
}
void TestObject::testMethod() {
	loggerTestObject->info("hello\n");
}
```

TestObject is a simple object with one method. All the method does is log a message.
But the Logger in TestObject is controlled by the configuration we set up earlier. If we want to change TestObject's logging we don't need to come back here and change this.

## Advanced Stuff

Internally the individual loggers get lazily configured. When first created they are flagged as unconfigured. The Logger Factory holds the configuration details and the first time a logger is called to print it asks the Logger Factory for its configured level. If there isn't one it remains unconfigured (and logs eveything). Once it gets a configured level it remebers it and no longer asks the Logger Factory.

This means we don't have to be very careful about initialising the LoggerFactory and the loggers in any particular order.

If you look at the sample file in the zip you will see it omits the TestObject files. As far as I can tell the Arduino IDE does not like samples that aren't a single ino file so it did not work when I added TestObject. So the distributed sample is cut down from the one shown here.

However the project in github has the full sample with the TestObject.

You can display the whole logging configuration to Serial by calling LoggerFactory.dump().

The LoggerFactory will wait for a Serial connection before it starts logging. But eventually you will want to stop doing that and turn off all logging. Just do this:

```
LoggerFactory loggerFactory = LoggerFactory(false);
```

This will suppress the wait and turn off all logging. You can actually leave out the argument, it defaults to false.

## Build

To build this I use [Sloeber](http://eclipse.baeyens.it/index.shtml). This is a free IDE based on Eclipse that supports Arduino and, of course, Teensy. I say free because you can use it for free, but you should do the right thing and support Jante's hard work on his Patreon.

Clone the repo from [github](https://github.com/RogerParkinson/ArduinoLogger) and build the project. You can flash the sample code to your device fro there. The project is configured for Teensy 3.2 by default so adjust athat as needed.

Once built I also run the distribute.sh file which will copy the files you actually need to your ~/Arduino/libraries directory, it also tidies up the sample so it can run as a single file (ie without TestObject).

## Install

 * Download zip from [madura](https://www.madurasoftware.com/ArduinoLogger.zip) or pull it from github and build it yourself (see above).
 * Import the zip file into your Arduino IDE
 * Restart your IDE if you had it open. Both Arduino and Sloeber will then be able to find the library.
 * Done

## Credit

Based on Arduino-logging-library by 
* [/mrRobot62](https://github.com/mrRobot62)  

