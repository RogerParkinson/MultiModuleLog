// Do not remove the include below
#include "LoggerSample.h"

Logger *loggerA;
Logger *loggerB;
//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	Serial.println("start setup");
	LoggerInst *loggerInstance = {new LoggerInst("A",LOG_LEVEL_ERRORS)};
	LoggerFactory loggerFactory = LoggerFactory(loggerInstance,1);
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

	loggerA->info("A Display my integers myInt1 %d, myInt2 %d\n",myInt1,myInt2); // Should be suppressed
	loggerB->info("B Display my integers myInt1 %d, myInt2 %d\n",myInt1,myInt2); // Should be visible
	delay(10000);
}
