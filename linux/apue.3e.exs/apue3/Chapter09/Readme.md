1. Refer back to our discussion of the `utmp` and `wtmp` files in Section 6.8.
   Why are the logout records written by the `init` process? Is this handled the
   same way for a network login?

   For console-based logins, `init` manages `agetty` (or something similar),
   so it is the process that gets notified via a `SIGCHLD` signal when a that
   process terminates (and that process termination implies a user logout).

   The `init` process isn't involved for network logins, because it is not
   the process that creates and manages the processes associated with the login.
   The daemon process (e.g., `sshd`) is responsible for updating those files
   in network login cases.

   Consider this example of monitoring the `sysdig` tool during a remote
   login via ssh:

   ```
   $ sudo sysdig fd.filename=wtmp
   21814 15:12:22.674427184 0 sshd (23474) < openat fd=5(<f>/var/log/wtmp) ...
   21823 15:12:22.674433650 0 sshd (23474) > lseek fd=5(<f>/var/log/wtmp) offset=0 whence=2(SEEK_END)
   21824 15:12:22.674434071 0 sshd (23474) < lseek res=1044864
   21825 15:12:22.674434597 0 sshd (23474) > write fd=5(<f>/var/log/wtmp) size=384
   21826 15:12:22.674440028 0 sshd (23474) < write res=384 data=...
   ```

   Note that `sshd` opened and wrote to `/var/log/wtmp`.

2. Write a small program that calls `fork` and has the child create a new
   session. Verify that the child becomes a process group leader and that
   the child no longer has a controlling terminal.

   Consider the following program:

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/types.h>
   #include <sys/wait.h>
   #include <unistd.h>
   
   int
   main(void)
   {
   	const pid_t pid = fork();
   
   	if (pid < 0) {
   		perror("fork");
   		return 1;
   	}
   
   	if (pid == 0) {
   		char buffer[128];
   
   		if (setsid() < 0) {
   			perror("setsid");
   			return 1;
   		}
   
   		snprintf(buffer, sizeof(buffer), "ps -p %d -opid,cmd,pgrp,tpgid,session", getpid());
   		system(buffer);
   	} else {
   		int status;
   
   		// Wait for child to die
   		wait(&status);
   	}
   
   	return 0;
   }
   ```

   A sample run of program produces the following output:

   ```
   $ ./a.out
     PID CMD                          PGRP TPGID  SESS
   28150 ./a.out                     28150    -1 28150
   ```

   _Verify that the child becomes a process group leader..._

   Notice that the `ps` output generated by the program includes the
   process group ID, and that matches the PID of the process.  This shows
   that the process is the process group leader.
   
   _... and that the child no longer has a controlling terminal._

   Again notice in the `ps` output above, the TPGID column has a value of
   `-1`.  This shows that the process no longer has a controlling terminal.
