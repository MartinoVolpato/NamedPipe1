#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_PATH "/tmp/echo_fifo"
#define BUF_SIZE 256

int main(void) {
    //Creiamo la FIFO, se esiste già andiamo avanti
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno == EEXIST) {
            printf("FIFO già esistente\n");
        } else {
            perror("mkfifo");
            exit(-1);
        }
    }

    //Apre in sola lettura: Si blocca finchè non c'è un reader
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("errore");
        exit(EXIT_FAILURE);
    }

    //leggo i datti della fifo
    char buffer[BUF_SIZE];
    ssize_t n = read(fd, buffer, sizeof(buffer));
    if ( n>0)
        printf("(Reader) ricevuto: %s\n", buffer);
    else if ( n == 0 )
        printf("(Reader) il writer ha chiuso senza inviare i dati\n");
    else
        perror("error read");

    //chiude il file descriptor
    close(fd);

    return 0;
}