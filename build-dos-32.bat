:: ========================================================
::
:: FILE:			/build-dos-32.bat
::
:: AUTHORS:			Jaycie Ewald
::
:: PROJECT:			Portal2D
::
:: LICENSE:			ACSL 1.4
::
:: DESCRIPTION:		Portal2D Windows compile script.
::
:: LAST EDITED:		October 30th, 2022
::
:: ========================================================

if not exist cmake-build-dos-32\ (
	md cmake-build-dos-32
)

cd cmake-build-dos-32
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS ../source
wmake
