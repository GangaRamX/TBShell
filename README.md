# STATUARY WARNING:
This application is for system testing, it creates a background daemon in the system. A remote machine interacts with the daemon to expoit the system.
After testing, the daemon must be killed. It leaves a attack surface which can be exploitted.


# TBShell
'tbshell' is a basic talk back shell, purpose of this is to test a Linux based system for for it's robust security.


# How To Build?
'make' and 'make clean' can be used, to build and clean the apps.
After successfull compilation, it creates two applications 'tbsh_client' and 'tbshell'.

# How To Use?
tbsh_client runs on targeted(test) machine, which starts a background daemon, It prints process id of the daemon. A server machine interacts with the daemon and can issue commands remotely.
To start 'tbsh_client' run following command on the test machine:
> $>tbsh_client <server_ip> <port_no>

Note down the process id of the daemon from the console message

To start serverside tbshell run following command on server:
> $>tbshell <server_ip> <port_no> 

It will connect to the tbsh_client daemon and will give you a shell.
You can run any one shot command from the shell, which will run on the test machine and will give you the output.
Continuous execution commands e.g. 'top', 'ping', 'vim' etc. are prohibited.
 After testing kill the process from test machine.
