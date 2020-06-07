#include <sys/syscall.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#define sys_father_pid_NO 404

int main(){
	while(1){
		// Child process
		if (fork() == 0){
			// Grandson process
			if (fork() == 0){
				int pid = getpid();
				while(1){
					printf("I'm the grandson process. My PID is: %d. My parent PID is: %d\n", pid, syscall(sys_father_pid_NO, pid));
					sleep(1);
				}
			}
			// child process
			else {
				int pid = getpid();
				while(1){
					printf("I'm the child process. My PID is: %d. My parent PID is: %d\n", pid, syscall(sys_father_pid_NO, pid));
					sleep(1);
				}
			}
		}
		// Parent process
		else {
			int pid = getpid();
			while(1){
				printf("I'm the Parent process. My PID is: %d.\n", pid);
				sleep(1);
			}
		}
	}

	return 0;
}