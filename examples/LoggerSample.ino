// Do not remove the include below
#include "MultiModuleLog.h"
#include "TestObject.h"

Logger *loggerA;
Logger *loggerB;
LoggerFactory loggerFactory = LoggerFactory(false);

const int ledPin = LED_BUILTIN;

void setup()
{
	pinMode(ledPin, OUTPUT);
	Serial.begin(9600);
	// Configure the loggers, including the one for TestObject
	loggerFactory.add("A",LOG_LEVEL_ERRORS);
	loggerFactory.add("B",LOG_LEVEL_INFOS);
	loggerFactory.add("TestObject",LOG_LEVEL_INFOS);
	loggerA = loggerFactory.getLogger("A"); // only log errors
	loggerB = loggerFactory.getLogger("B"); // default is log everything

}

// The loop function is called in an endless loop
void loop()
{
	int myInt1 = 232;
	int myInt2 = 32199;
	TestObject t; // TestObject has its own logger

	loggerA->info("error Display my integers myInt1 %d, myInt2 %d",myInt1,myInt2); // Should be suppressed
	loggerA->debug("debug message"); // Should be suppressed
	loggerB->info("info Display my integers myInt1 %d, myInt2 %d",myInt1,myInt2); // Should be visible
	loggerB->debug("debug Display my integers myInt1 %d, myInt2 %d",myInt1,myInt2); // Should be suppressed
	t.testMethod();
	loggerFactory.dump();
	digitalWrite(ledPin, HIGH);   // set the LED on
	delay(1000);                  // wait for a second
	digitalWrite(ledPin, LOW);    // set the LED off
	delay(1000);                  // wait for a second
}
