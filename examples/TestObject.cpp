#include "TestObject.h"

Logger *loggerTestObject = loggerFactory.getLogger("TestObject");

TestObject::TestObject(){
}
void TestObject::testMethod() {
	loggerTestObject->info("hello\n");
}



