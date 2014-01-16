#!/bin/bash

PKG_NAME="BaroboLink_2.0"


if [ -z "$QTDIR" ]; then
  echo "Please set environment variable QTDIR to point to the correct Qt installation."
  exit 1
fi

if [ -z "$STAGE" ]; then
  echo "Please set the STAGE environment variable to point to the staging directory containing barobo libraries."
  exit 1
fi

if [ ! -f "$STAGE/bin/QtBaroboLink" ]; then
  echo "QtBaroboLink executable not found in staging directory."
  exit 1
fi

QTFRAMEWORKS="QtWidgets QtNetwork QtWebKitWidgets QtMultimediaWidgets QtMultimedia QtQuick QtPositioning QtQml QtOpenGL QtPrintSupport QtWebKit QtGui QtSensors QtCore"

mkdir -p $PKG_NAME.app/Contents/Frameworks
mkdir -p $PKG_NAME.app/Contents/MacOS

# Copy the executable
cp $STAGE/bin/QtBaroboLink $PKG_NAME.app/Contents/MacOS/	

# Copy all the frameworks to the right place
for f in $QTFRAMEWORKS; do
	cp -R $QTDIR/lib/$f.framework $PKG_NAME.app/Contents/Frameworks
done

# Change the id of the frameworks
for f in $QTFRAMEWORKS; do
	install_name_tool -id @executable_path/../Frameworks/$f.framework/Versions/5/$f $PKG_NAME.app/Contents/Frameworks/$f.framework/Versions/5/$f
done

# Change the executable to look for frameworks in the right place
for f in $QTFRAMEWORKS; do
	install_name_tool -change $QTDIR/lib/$f.framework/Versions/5/$f @executable_path/../Frameworks/$f.framework/Versions/5/$f $PKG_NAME.app/Contents/MacOS/QtBaroboLink
done
