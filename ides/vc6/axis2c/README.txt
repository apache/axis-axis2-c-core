DESCRIPTION:
a workspace and associated projects required to build the axis2_client.dll and immediate default dependencies.  

does not include project files for:
samples
guththila
clients
services
tcpmon

requires:
1)libxml2-2.6.27.win32.zip
2)iconv-1.9.2.win32.zip
3)zlib-1.2.3.win32.zip

4)a directory called 'axis2c_deps' into which are extracted these three zips.  The projects reference these directories using a relative path so this axis2c_deps directory must be placed in the AXIS2C_HOME root.  (sibling of ides directory)

5)MS Platform SDK
6)axutil project has references to the platform SDK include and lib directories.  due to NOTE below need to be verified for each installation.

**NOTE old-style condensed paths are REQUIRED which on the workstation used to create the projects are 
"c:/progra~1/micros~4/include" and "c:/progra~1/micros~4/lib".  note that the ~4 is likely to be different depending on target workstation configuration. (use dir /x to see the workstation-specific condensed name)



INSTALL:
extract to AXIS2C_HOME directory.  the workspace will be extracted to /ides/vc/axis2c and the project files will be placed in the appropriate subdirectories.
create axis2c_deps in AXIS2C_HOME and extract dependency zips into it.
ensure path to MS platform SDK is correct in axutil project settings.


OUTPUT:
with axis2_engine selected as active project, "build all" creates target .dll and .lib files in the AXIS2C_HOME/lib directory.


KNOWN BUGS:
some warnings about platform-specific header files appear but compile, run and debug work correctly
