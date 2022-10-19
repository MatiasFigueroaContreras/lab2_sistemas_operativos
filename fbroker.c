#include "fbroker.h"

/*
    Entradas:
    Salidas:
    Descripcion:
*/
int ***createWorkers(int num_workers, float min_price, int initial_year)
{
    int fds[num_workers][2][2], pid;
    char *fd_read, *fd_write, *s_min_price, *s_initial_year;
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
            sprintf(s_min_price, "%d", min_price);
            sprintf(s_initial_year, "%f", initial_year);
            execl("./worker", "./worker", fd_read, fd_write, s_min_price, s_initial_year, NULL);
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

/*
    Entradas:
        -int initial_year: anio inicial para comenzar buscar informacion. 
    Salidas: arreglo con las estructuras YearData creadas.
    Descripcion: Esta funcion se encarga de crear un arreglo de estructuras
        YearData segun el anio inicial dado hasta el anio actual.
*/
YearData *createYearsDataArray(int initial_year)
{
    
    int num_years = 2022 - initial_year + 1;
    YearData years_data[num_years];
    for(int i = 0; i < num_years; i++)
    {
        years_data[i] = *createYearData();
    }

    return years_data;
}

/*
    Entradas:
        -int ** fds: descriptores de archivos.
        -int num_workers: numero de workers creados.
    Salidas: cantidad de lineas trabajadas por los workers
    Descripcion: Funcion encargada de mandar mensaje "FIN" a cada worker
        y recibir la cantidad de lineas trabajadas por estos.
*/
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

    return line_numbers;
}

/*
    Entradas:
        -char *file_name: nombre del archivo de salida.
        -YearData *years_data: arreglo con los Years Data.
    Salidas: void
    Descripcion: Funcion destinada a escribir en un archivo de salida
*/
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

/*
    Entradas:YearData *years_data (ESTRUCTURA YEAR DATA)
    Salidas: void
    Descripcion: Imprime la estructura para la consola
*/
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

/*
    Entradas: int *line_numbers (NUMERO DE LINEAS DE CADA WORKER)
    Salidas: void
    Descripcion: Imprime la cantidad de lineas de cada worker para la consola en caso de que se active la flag -b
*/
void printLineNumbersWorkers(int *line_numbers)
{
    for (int i = 0; i < line_numbers; i++)
    {
        printf("Hijo %d: %d lineas procesadas", i + 1, line_numbers[i]);
    }
}
