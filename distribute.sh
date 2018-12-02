#!/bin/bash

export LIBDIR=~/Arduino/libraries/ArduinoLogger
mkdir $LIBDIR/examples
#cp -R ./examples/TestObject.* $LIBDIR
cp ./examples/LoggerSample.cpp $LIBDIR/examples/LoggerSample.ino
sed -i '/TestObject/d;/testObject/d;/testMethod/d' $LIBDIR/examples/LoggerSample.ino 
cp -R ./src $LIBDIR
cp library.* $LIBDIR
cp keywords.txt $LIBDIR
cp LICENSE $LIBDIR
cp README.md $LIBDIR
exit 0