This is a sample to help test rampart.
This sample works with echo service.

How to run
----------
1. Create the binary 
    make
    make install

2. Create client repository
    e.g. ~/client_repo [CLIENT_REPO]

3. Copy ./data/axis2.xml to CLIENT_REPO

4. Copy AXIS2_HOME/lib to CLIENT_REPO

5. Copy AXIS2_HOME/modules to CLIENT_REPO
   
6. Enable rampart in axis2.xml in the server and add In/Outflow parameters as
    in ./data/server.axis2.xml

7. Go to samples/calback/ and run make install. This will create pwcb.so inside
    rampart/targt directory. Specify the correct path to pwcb.so using
    passwordCallbackClass parameter in the configuration file.
    <action>
        ....
        <passwordCallbackClass>/your/path/to/libpwcb.so</passwordCallbackClass>
        ....
    </action>
    NOTE: Right now this sample callback will provide only two pairs of
    Username/password. You may add more by editing the get_password() function
    in pwcb.c
    
    
8. To view the message flow use TCPMonitor
    Listen port: 9090

9. Run the echo sample as ./echo


NOTE : Also you can use other samples available under AXIS2C.
