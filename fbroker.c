#include "fbroker.h"

// Entradas:
// Salidas:
// Descripcion:
int ***createWorkers(int num_workers)
{
    int fds[num_workers][2][2], pid;
    char *fd_read, *fd_write;
    for (int i = 0; i < num_workers; i++)
    {
        pipe(fds[i][0]);
        pipe(fds[i][1]);
        if ((pid = fork()) == 0)
        {
            close(fds[i][BROKER_WRITE][WRITING]);
            close(fds[i][BROKER_READ][READING]);
            sprintf(fd_read, "%d", fds[i][0][READING]);
            sprintf(fd_write, "%d", fds[i][1][WRITING]);
            execl("./worker", "./worker", fd_read, fd_write, NULL);
            perror("exec ls failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == -1)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            close(fds[i][BROKER_WRITE][READING]);
            close(fds[i][BROKER_READ][WRITING]);
        }
    }

    return fds;
}

// Entradas:FILE *input_file (archivo de entrada en modo lectura)
// Salidas: char
// Descripcion:Funcion encargada de obtener la cantidad de lineas procesadas por el worker
char *getLine(FILE *input_file)
{
    char game_data[400];
    fgets(game_data, 400, input_file);
    return game_data;
}

// Entradas:int **fds (file descriptor)
// Salidas: int
// Descripcion: Funcion encargada de mandar "FIN" a cada worker
int *stopWorkers(int ***fds, int num_workers)
{
    const char *msg = "FIN";
    int line_numbers[num_workers];
    for (int i = 0; i < num_workers; i++)
    {
        write(fds[i][BROKER_WRITE][WRITING], msg, strlen(msg) + 1);
        wait(NULL);
        read(fds[i][BROKER_READ][READING], &line_numbers[i], sizeof(int));
        close(fds[i][BROKER_WRITE][WRITING]);
        close(fds[i][BROKER_READ][READING]);
    }
}

// Entradas:char *file_name(ARCHIVO DE SALIDA), YearData *years_data (ESTRUCTURA YEAR DATA)
// Salidas:void
// Descripcion: Funcion destinada a escribir en un archivo de salida
void writeOutputFile(char *file_name, YearData *years_data, int initial_year)
{
    int index;
    FILE *file = fopen(file_name, "w");
    for (int y = initial_year; y <= 2022; y++)
    {
        if (!isEmpty(&years_data[y]))
        {
            fputs(toString(&years_data[y]), file);
        }
    }
}

// Entradas:YearData *years_data (ESTRUCTURA YEAR DATA)
// Salidas: void
// Descripcion: Imprime la estructura para la consola
void printYearsData(YearData *years_data, int initial_year)
{
    for (int y = initial_year; y <= 2022; y++)
    {
        if (!isEmpty(&years_data[y]))
        {
            printf("%s", toString(&years_data[y]));
        }
    }
}

// Entradas: int *line_numbers (NUMERO DE LINEAS DE CADA WORKER)
// Salidas: void
// Descripcion: Imprime la cantidad de lineas de cada worker para la consola en caso de que se active la flag -b
void printLineNumbersWorkers(int *line_numbers)
{
    for (int i = 0; i < line_numbers; i++)
    {
        printf("Hijo %d: %d lineas procesadas", i + 1, line_numbers[i]);
    }
}
