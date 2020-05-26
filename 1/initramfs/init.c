#include "iterate_sys_calls.h"
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#define __NR_calls 11
#define SECONDS_TO_WAIT 3

void mount_fs(){
	printf("Montando o sistem de arquivos... \n");
	// Tenta criar um diretório chamado /sys com permissão de acesso rwx igual a 444 (0001 0010 0100) = x(100), w(100), r(100)
	if (mkdir("/sys", 0x124) && errno != EEXIST){
		fprintf(stderr, "%s: %s (errno = %d)\n", "mkdir", strerror(errno), errno);
	}
	if (mount("none", "sys", "sysfs", 0, "")){
		fprintf(stderr, "%s: %s (errno = %d)\n", "mount", strerror(errno), errno);
	}
}

int main(){
	mount_fs();

	// Lá no sys_last_scno ele deveria criar uma pasta chamda last_scno.
	//	O arquivo log_number a priori não existe, mas ao abri-lo, ele
	//	passa a existir. A leitura por sua vez deve engatilhar
	//	a função print_to_log lá no sys_last_scno.
	int fd = open("/sys/kernel/last_scno/log_number", O_RDONLY);
	char string_to_read[10];

	while(1){

		// Deveria armazenar o conteúdo do arquivo aqui
		read(fd, string_to_read, 10);
		printf("%s\n",string_to_read);

		int calls[__NR_calls] = {20, 199, 200, 201, 202, 205, 209, 211, 224, 364, 400};

		int system_call_NR;
		int timer;
		for(system_call_NR = 0; system_call_NR < __NR_calls; system_call_NR++){
			printf("Executando a syscall %d\n", calls[system_call_NR]);
			iterate_sys_calls(calls[system_call_NR]);
			
			sleep(SECONDS_TO_WAIT);
		}
	}
	return 0;
}