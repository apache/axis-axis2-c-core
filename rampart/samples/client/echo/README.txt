This is a sample to help test rampart.
This sample works with echo service.

How to run
----------
1. Create the binary 
    make
    make install

2. Create client repository
    e.g. ~/client_home [CLIENT_HOME]
    
3. Copy rampart/samples/client/data/axis2.xml to CLIENT_REPO

4. Copy AXIS2_HOME/lib to CLIENT_HOME

5. Copy AXIS2_HOME/modules to CLIENT_HOME
   
6. Enable rampart in axis2.xml in the AXIS2_HOME and add In/Outflow parameters as
    in rampart/samples/client/data/server.axis2.xml. (Or copy the
    server.axis2.xml file to AXIS2_HOME/ directory and rename it as axis2.xml)

7. Go to samples/calback/ and run make install. This will create pwcb.so inside
    rampart/targt directory. Specify the correct path to pwcb.so using
    passwordCallbackClass parameter in the configuration file. Windows user
    please specify the path to pwcb.DLL.

    <action>
        ....
        <passwordCallbackClass>/your/path/to/libpwcb.so</passwordCallbackClass>
        ....
    </action>
    
    NOTE: Right now this sample callback will provide only two pairs of
    Username/passwords.

    Username    Password
    --------------------
    Raigama     RaigamaPW
    Gampola     GampolaPW

    You may add more by editing the get_password() function
    in pwcb.c
    
    
8. To view the message flow use TCPMonitor
    Listen port: 9090

9. Start server in port 8080 
     ./axis2_http_server -p8080

10. Run the echo sample 
     Syntax : ./echo [service_address] [client_home]
     e.g.   : ./echo http://localhost:9090/axis2/services/echo /home/kau/client_home   

11. You should be able to see the security token is attached to the header of
    the SOAP message in the TCP Monitor. The client request is using UsernamToken with diegested 
    password, while the server response is using a PlainText password.

NOTE : Also you can use other samples available under AXIS2C.

