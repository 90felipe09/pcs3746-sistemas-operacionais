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

	// 1. Crio um file descriptor de um arquivo lá dentro de last_scno
	int fd = open("/sys/kernel/last_scno/last_number", O_RDONLY);

	// 2. Guardo caractere por caractere
	char c;

	while(1){

		// 3. Posicionamos o cursor de leitura do arquivo no início.
		//	o segundo argumento é o offset em bytes. O terceiro é uma constante de referência.
		//	No caso, específico para começar do início (SEEK_SET). Começando do início e com offset 0,
		//	começa-se do início mesmo. Caso ele retorne -1, significa que deu ruim. Vou executar tratativa
		//	de erro
		if (lseek(fd, 0, SEEK_SET) < 0){
			printf("Não foi possível rebobinar o arquivo\n");
		}

		// 4. Fico lendo todos os caracteres do arquivo até chegar ao fim.
		printf("Lendo o arquivo\n");
		while (read(fd, &c, sizeof(char)) > 0){
			printf("%c", c);
		}

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