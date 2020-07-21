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

int main()
{
    get_own_process_memory_info();
	char *cursor = malloc(1);
    char cursor_value;
    for(;;){
        cursor_value = *cursor;
        if (cursor == 0xbafff){
            printf("Cheguei à posição 0xbafff\n");
            cursor_value = getchar();
        }
        // printf("%p\n", cursor);
        cursor++;
    }
	return 0;
}
