#!/bin/bash
# copies the files into the arduino library directory, ignoring the ones we don't need there.
# It modifies the sample to be a single ino file because Arduino IDE seems to not support multiple files in samples.

export LIBDIR=~/Arduino/libraries/MultiModuleLog
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
