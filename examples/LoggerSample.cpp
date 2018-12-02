// Do not remove the include below
//#include "Logger.h"
#include "TestObject.h"

Logger *loggerA;
Logger *loggerB;
LoggerFactory loggerFactory = LoggerFactory();

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	Serial.println("start setup");
	// Configure the loggers, including the one for TestObject
	loggerFactory.add(new LoggerInst("A",LOG_LEVEL_ERRORS));
	loggerFactory.add(new LoggerInst("A",LOG_LEVEL_INFOS));
	loggerFactory.add(new LoggerInst("TestObject",LOG_LEVEL_INFOS));
	Serial.println("creating loggers...");
	loggerA = loggerFactory.getLogger("A"); // only log errors
	loggerB = loggerFactory.getLogger("B"); // default is log everything
	Serial.println("end setup");

}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	int myInt1 = 232;
	int myInt2 = 32199;
	TestObject t; // TestObject has its own logger

	loggerA->info("error Display my integers myInt1 %d, myInt2 %d\n",myInt1,myInt2); // Should be suppressed
	loggerA->debug("debug message\n"); // Should be suppressed
	loggerB->info("info Display my integers myInt1 %d, myInt2 %d\n",myInt1,myInt2); // Should be visible
	loggerB->debug("debug Display my integers myInt1 %d, myInt2 %d\n",myInt1,myInt2); // Should be suppressed
	t.testMethod();
	loggerFactory.dump();
	delay(5000);
}
