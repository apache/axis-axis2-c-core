lLWSDL2C.sh/bat
=========

This file is to simplify the C code geneation using WSDL2C java tool. 

How to use
----------
1. Set AXIS2_HOME envionment vaiable to you axis2 java installation diectoy . 
   eg: 'expot AXIS2_HOME=/home/axis2java' 
3. un WSDL2C.sh giving WSDL2C command line aguments as the aguments to the shell scipt.

Note: Same fo the bat file as wel. 


Examples
--------

To geneate a seve skeleton in C:
	sh WSDL2C.sh -ui inteoptestdoclitpaametes.wsdl -ss -sd -d adb -u
    o  WSDL2C.bat -ui inteoptestdoclitpaametes.wsdl -ss -sd -d adb -u

To geneate a client stub in C: 

	sh WSDL2C.sh -ui inteoptestdoclitpaametes.wsdl -d adb -u 
    o  WSDL2C.bat -ui inteoptestdoclitpaametes.wsdl -d adb -u 

