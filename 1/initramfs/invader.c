#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


void get_own_process_memory_info(){
    const char* maps_path = "/proc/self/maps";
    int fd = open(maps_path, O_RDONLY);
    char c;

    if(!fd){
        perror(maps_path);
        abort();
    }

    if (lseek(fd,0, SEEK_SET) < 0){
        printf("Não foi possível rebobinar o arquivo\n");
    }

    printf("Onde o invader foi alocado:\n");
    while(read(fd,&c, sizeof(char))>0){
        printf("%c", c);
    }
}

void get_vm_size_allocated(){
    const char* maps_path = "/proc/self/stat";
    int fd = open(maps_path, O_RDONLY);
    char c;

    if(!fd){
        perror(maps_path);
        abort();
    }

    if (lseek(fd,0, SEEK_SET) < 0){
        printf("Não foi possível rebobinar o arquivo\n");
    }

    printf("\n\nConteúdo do stat:\n\n");
    int counter = 1;
    int relevant = 0;
    while(read(fd,&c, sizeof(char))>0){
        if(c == ' '){ 
            counter++;
            switch (counter){
                case (23):
                    relevant = 1; 
                    printf("\nvsize: ");
                    break;
                case (24):
                    relevant = 1; 
                    printf("\nrss: ");
                    break;
                case (25):
                    relevant = 1; 
                    printf("\nrsslim: ");
                    break;
                case (26):
                    relevant = 1; 
                    printf("\nstartcode: ");
                    break;
                case (27):
                    relevant = 1; 
                    printf("\nendcode: ");
                    break;
                case (28):
                    relevant = 1; 
                    printf("\nstartstack: ");
                    break;
                case (45):
                    relevant = 1; 
                    printf("\nstart_data: ");
                    break;
                case (46):
                    relevant = 1; 
                    printf("\nend_data: ");
                    break;
                default:
                    relevant = 0;
            }
        }
        if(relevant == 1){
            printf("%c", c);
        }

    }
}

int main()
{
    get_own_process_memory_info();
    get_vm_size_allocated();
    char *teste;
    int mais_um_malloc = 0;
    printf("\nQuer fazer um malloc além do cursor? y = 1\n");
    scanf("%d", &mais_um_malloc);

    int bytes_to_alloc = 1;
    printf("Quantos bytes alocar pro cursor?\n");
    scanf("%d", &bytes_to_alloc);

    if (mais_um_malloc == 1){
        teste = malloc(1);
    }
	char *cursor = malloc(bytes_to_alloc);

    int start_at_end = 0;
    printf("Quer iniciar o cursor em  0xffff0000? y = 1\n");
    scanf("%d", start_at_end);
    if (start_at_end == 1){
        cursor = (char *)0xffff0004;
    }
    
    printf("o cursor começa em %p\n", cursor);
    int printa_tudo = 0;
    int incrementar = 1;
    int get_char = 1;
    printf("Quer printar todos os endereços? 1 = y\n");
    scanf("%d", &printa_tudo);
    printf("Quer incrementar ou decrementar? 1 = incrementar, 0 = decrementar\n");
    scanf("%d", &incrementar);
    printf("Quer fazer captura de char? 1 = y, 0 = n\n");
    scanf("%d", &get_char);
    char cursor_value;
    for(;;){
        if(get_char == 1){
            cursor_value = *cursor;
        }
        if (cursor == 0xffffffff){
            printf("Cheguei à posição 0xffffffff\n");
            cursor_value = getchar();
        }
        if (printa_tudo == 1){
            printf("%p\n", cursor);
        }
        if (incrementar == 1){
            cursor++;
        }
        else{
            cursor--;
        }
        
    }
	return 0;
}
