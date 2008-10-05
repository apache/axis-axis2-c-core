@echo off
cls

echo "Compiling..."
cl.exe /nologo /D "WIN32" /D "_WINDOWS" /D "_MBCS" /I %AXIS2C_HOME%\include /I ..\include /I ..\src /c test_windows.c ../src/*.c

echo "Linking..."
link.exe /LIBPATH:%AXIS2C_HOME%\lib axutil.lib axiom.lib axis2_parser.lib axis2_engine.lib /OUT:test.exe *.obj

del *.obj

echo "Running..."
test.exe