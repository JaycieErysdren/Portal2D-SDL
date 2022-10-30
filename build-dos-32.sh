## ========================================================
##
## FILE:			/build-dos-32.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Portal2D
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Portal2D Linux compile script.
##
## LAST EDITED:		October 30th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-dos-32 ] 
then
	mkdir cmake-build-dos-32
fi

cd cmake-build-dos-32
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS ../source
wmake
