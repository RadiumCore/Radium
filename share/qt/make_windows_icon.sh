#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/radium.ico

convert ../../src/qt/res/icons/radium-16.png ../../src/qt/res/icons/radium-32.png ../../src/qt/res/icons/radium-48.png ${ICON_DST}
