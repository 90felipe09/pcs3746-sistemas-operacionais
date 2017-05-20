#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "stack.h"

#define len(_arr) ((int)((&_arr)[1] - _arr))

static const char * const programs[] = { "/stack_push", "/stack_pop" };

void panic(const char *msg)
{
	fprintf(stderr, "%s: %s (errno = %d)\n", msg, strerror(errno), errno);
	exit(-1);
}

int main()
{
	printf("Custom initramfs - forking to run %d programs\n", len(programs));

        printf("-- Begin stack test --\n");
        printf("%ld\n", stack_push(1));
        printf("%ld\n", stack_push(2));
        printf("%ld\n", stack_push(3));
        printf("%ld\n", stack_pop());
        printf("%ld\n", stack_pop());
        printf("%ld\n", stack_pop());
        printf("%ld\n", stack_pop());
        printf("-- End stack test --\n");

	for (int i = 0; i < len(programs); i++) {
		const char *path = programs[i];
		pid_t pid = fork();
		if (pid == -1) {
			panic("fork");
		} else if (pid) {
			printf("Starting %s (pid = %d)\n", path, pid);
		} else {
			execl(path, path, (char *)NULL);
			panic("execl");
		}
	}

	int program_count = len(programs);
	while (program_count) {
		int wstatus;
		pid_t pid = wait(&wstatus);
		if (WIFEXITED(wstatus))
			printf("pid %d exited with %d\n", pid, WEXITSTATUS(wstatus));
		else if (WIFSIGNALED(wstatus))
			printf("pid %d killed by signal %d\n", pid, WTERMSIG(wstatus));
		else
			continue;
		program_count--;
	}

	printf("init finished\n");
	for (;;)
		sleep(1000);
	return 0;
}
