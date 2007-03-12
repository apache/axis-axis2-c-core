The scenarios available here can be deployed using either deploy.sh(UNIX) or
deploy.bat(WIN32).

Simply give the scenario name as an argument to the script
e.g. %sh deploy.sh scenario1 

Each scenario has a README file describing it's purpose.

Make sure you have run the deploy_client_repo.sh.

These scenarios will only copy the security policy xml files. To run the
client use the script samples/client/sec_echo/update_n_run.sh or
samples/client/sec_echo/update_n_run.bat

Following is a summary of scenarios available.

Scenario    Summary
-------------------
1.          Timestamp
2.          UsernameToken+Timestamp
3.          Client-Encrypt(IssuerSerial)           
4.          Client-Encrypt(RefKeyIdentifier)       
5.          Client-Encrypt(Embedded)               
6.          Client-Encrypt(DirectReference)        
7.          Both client and server Encrypts
