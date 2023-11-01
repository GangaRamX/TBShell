# STATUARY WARNING:
This application creates a background daemon in the system for testing purposes. The daemon is then use by a remote machine to exploit the system. After testing, the daemon must be killed. However, it leaves behind an attack surface that can be exploited.


# TBShell
'tbshell' is a basic talk back shell, purpose of this is to test a Linux based system for it's robust security.


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
