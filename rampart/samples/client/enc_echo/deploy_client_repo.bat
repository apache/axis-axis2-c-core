echo off

rem if your client repository is different, change the value.
set CLIENT_REPO=%AXIS2C_HOME%\clientrepo

echo Start creating a client repository at %CLIENT_REPO%

if exist  "%CLIENT_REPO%" (
    echo %CLIENT_REPO% exists.
) else (
    echo Creating a new directory for client repo
    mkdir "%CLIENT_REPO%"
)


@echo Copying axis2.xml to client_repo
copy /Y ".\data\client.enc.axis2.xml" "%CLIENT_REPO%\axis2.xml"

@echo Copying axis2.xml to AXIS2C_HOME
copy /Y ".\data\server.enc.axis2.xml" "%AXIS2C_HOME%\axis2.xml"

@echo Copying libraries to client_repo
xcopy /S/I/Q/Y "%AXIS2C_HOME%\lib" "%CLIENT_REPO%\lib"

@echo Copying latest modules to client_repo
xcopy /S/I/Q/Y "%AXIS2C_HOME%\modules" "%CLIENT_REPO%\modules"

echo on