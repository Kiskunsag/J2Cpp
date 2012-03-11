echo off
cls
 REM Ensure, that we start more or less from scratch
del CMakeCache.txt
echo Generating makefiles...
cmake -G "MinGW Makefiles" .
 REM Binaries are built before creating a visual studio-solution so that we have clean build without makefiles and solution mixing up.
echo Building binaries...
make
 REM Cleanup is needed before cmaking the directory again. 
del CMakeCache.txt
echo Generating Visual Studio-Solution....
cmake -G "Visual Studio 10"
echo Ready.
pause