#include <sys/syscall.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>


int main(){
	char c[2] = "KH";
	int aux_control;
	while(1){
		puts(c);
		scanf("%d", &aux_control);
	}

	return 0;
}