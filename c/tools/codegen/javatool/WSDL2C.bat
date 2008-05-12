echo off
REM set AXIS2_HOME=C:\axis2-SNAPSHOT
setlocal EnableDelayedExpansion
set AXIS2_CLASSPATH=%AXIS2_HOME%
FOR %%c in ("%AXIS2_HOME%\lib\*.jar") DO set AXIS2_CLASSPATH=!AXIS2_CLASSPATH!;%%c;

java -classpath %AXIS2_CLASSPATH% org.apache.axis2.wsdl.WSDL2C %*


