deploy.sh will copy the relevent policy files for client
and services.

Execute the deploy.sh as follows.

sh deploy.sh scenarioX

eg:- To copy the policy files in scenario1,

sh deploy.sh scenario1

Important
-----------
1.Make sure to run $rampart_directory/samples/client/sec_echo/deploy_client_repo.sh before this.

2.README.txt file inside each scenario directory describes
  what each scenario contains in its policy.

3.After copying policy files run $rampart_directory/samples/client/sec_echo/update_n_run.sh.
