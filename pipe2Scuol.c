#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define dim 5

int main()
{
    int pid;
    //int array[dim]; //creando l'array qui sia il padre che il figlio vedono un array
    int fd[2];
    int richiesta; // numero con il quale moltiplicheremo l'array
    int status;

    if (pipe(fd) == -1) // creiamo la pipe prima della fork
    {
        perror("\nErrore di creazione della pipe");
        exit(-1); // usciamo
    }

    pid = fork();

    if (pid == -1)
    {
        perror("\nErrore nella creazione del processo figlio");
        exit(-1);
    }

    if (pid == 0) // Processo figlio
    {
        int arrayFiglio[dim]; //possiamio dichiarare un array per il padre e per il figlio
        printf("\nSono il figlio, il mio pid è: %d, mio padre ha pid: %d\n", getpid(), getppid());

        for (int i = 0; i < dim; i++)
        {
            printf("\nInserire un numero:\n");
            scanf("%d", &arrayFiglio[i]);
        }

        close(fd[0]); //chiudiamo la read

        write(fd[1], arrayFiglio, sizeof(int)*dim); // utilizzare sizeof per scrivere l'intero array
        close(fd[1]);
        printf("\nInviato array al padre\n");

        exit(0); // Il processo figlio termina qui
    }
    else // Processo padre
    {
        wait(&status); // Attendiamo il completamento del processo figlio perchè la read e la write sono già sincronizzate.

        int arrayPadre[dim]; //array creato per il padres

        printf("\nSono il padre, il mio pid è: %d, mio figlio ha pid: %d\n", getpid(), pid);
        printf("\nInserire un numero con il quale moltiplicare l'array.\n");
        scanf("%d", &richiesta);

        close(fd[1]); //chiudiamo la write

        read(fd[0], arrayPadre, sizeof(int)*dim); // utilizzare sizeof per leggere l'intero array

        for (int i = 0; i < dim; i++)
        {
            printf("\nNumero %d: %d\n", i + 1, arrayPadre[i] * richiesta);
        }

        close(fd[0]);
    }

    return 0;
}