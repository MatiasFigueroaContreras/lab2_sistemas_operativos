#include "fbroker.h"

/*
    Entradas:
        - int num_workers: Cantidad de workers a crear.
        - float min_price: Precio minimo de los juegos.
        - int initial_year: Anio inicial de los juegos.
    Salidas:
        - int ***fds: Arreglo de descriptores de archivos generados
            con pipe para comunicacion entre broker y workers.
    Descripcion:
        - Crea los procesos workers y los conecta con el broker mediante pipes,
            pasandole toda la informacion preliminar que necesitaran los workers.
*/
int ***createWorkers(int num_workers, float min_price, int initial_year)
{
    int pid;
    int ***fds = malloc(sizeof(int **) * num_workers);
    for (int i = 0; i < num_workers; i++)
    {
        fds[i] = malloc(sizeof(int *) * 2);
        for (int j = 0; j < 2; j++)
        {
            fds[i][j] = malloc(sizeof(int) * 2);
        }
    }
    char fd_read[10], fd_write[10], s_min_price[10], s_initial_year[6];
    for (int i = 0; i < num_workers; i++)
    {
        pipe(fds[i][BROKER_WRITE]);
        pipe(fds[i][BROKER_READ]);
        if ((pid = fork()) == 0)
        {
            dup2(fds[i][BROKER_READ][WRITING], STDOUT_FILENO);
            close(fds[i][BROKER_WRITE][WRITING]);
            close(fds[i][BROKER_READ][READING]);
            sprintf(fd_read, "%d", fds[i][0][READING]);
            sprintf(fd_write, "%d", fds[i][1][WRITING]);
            sprintf(s_min_price, "%f", min_price);
            sprintf(s_initial_year, "%d", initial_year);
            execl("./worker", "./worker", fd_read, s_min_price, s_initial_year, NULL);
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
        - int initial_year: Anio inicial para comenzar a buscar informacion.
    Salidas:
        - YearData *years_data: Arreglo con las estructuras YearData creadas
            e inicializadas.
    Descripcion:
        - Funcion encargada de crear un arreglo de estructuras
            YearData segun el anio inicial dado hasta el anio actual.
*/
YearData *createYearsDataArray(int initial_year)
{

    int num_years = 2022 - initial_year + 1;
    int index;
    YearData *years_data = malloc(sizeof(YearData) * num_years);
    for (int y = initial_year; y <= 2022; y++)
    {
        index = y % num_years;
        years_data[index] = *createYearData();
        years_data[index].year = y;
        years_data[index].price_cheap_game = 1000000;
    }

    return years_data;
}

/*
    Entradas:
        - int *** fds: descriptores de archivos.
        - int num_workers: numero de workers creados.
    Salidas:
        - int *line_numbers: cantidad de lineas trabajadas por los workers.
    Descripcion:
        -Funcion encargada de mandar mensaje "FIN" a cada worker
            y recibir la cantidad de lineas trabajadas por estos.
*/
int *stopWorkers(int ***fds, int num_workers)
{
    const char *msg = "FIN";
    int *line_numbers = malloc(sizeof(int) * num_workers);
    for (int i = 0; i < num_workers; i++)
    {
        write(fds[i][BROKER_WRITE][WRITING], msg, strlen(msg) + 1);
        read(fds[i][BROKER_READ][READING], &line_numbers[i], sizeof(int));
        close(fds[i][BROKER_WRITE][WRITING]);
        close(fds[i][BROKER_READ][READING]);
    }

    return line_numbers;
}

/*
    Entradas:
        -char *file_name: Nombre del archivo de salida.
        -YearData *years_data: Arreglo con la informacion referente
            a cada anio.
        -int initial_year: anio inicial de los datos.
    Salidas:
        -void
    Descripcion:
        -Funcion destinada a escribir en un archivo de salida la
            informacion referente a cada anio.
*/
void writeOutputFile(char *file_name, YearData *years_data, int initial_year)
{
    int index;
    int num_years = 2022 - initial_year + 1;
    FILE *file = fopen(file_name, "w");
    for (int y = initial_year; y <= 2022; y++)
    {
        index = y % num_years;
        if (!isEmpty(&years_data[index]))
        {
            fputs(toString(&years_data[index]), file);
        }
    }
}

/*
    Entradas:
        - YearData *years_data: Arreglo con la informacion referente
            a cada anio.
    Salidas:
        - void
    Descripcion:
        - Imprime por consola la informacion referente a cada anio.
*/
void printYearsData(YearData *years_data, int initial_year)
{
    int index;
    int num_years = 2022 - initial_year + 1;
    for (int y = initial_year; y <= 2022; y++)
    {
        index = y % num_years;
        if (!isEmpty(&years_data[index]))
        {
            printf("%s", toString(&years_data[index]));
        }
    }
}

/*
    Entradas:
        -int *line_numbers: Numero de lineas de cada worker.
        -int num_workers: numero de workers generados.
    Salidas:
        -void
    Descripcion:
        -Imprime la cantidad de lineas de cada worker por consola.
*/
void printLineNumbersWorkers(int *line_numbers, int num_workers)
{
    for (int i = 0; i < num_workers; i++)
    {
        printf("Hijo %d: %d lineas procesadas\n", i + 1, line_numbers[i]);
    }
}

/*
    Entradas:
        - char *game_data: linea de datos de un juego.
    Salidas:
        - int year: anio del juego.
    Descripcion:
        - Funcion encargada de obtener el anio de un juego.
*/
int getYear(char *game_data)
{
    char game_data_copy[400];
    strcpy(game_data_copy, game_data);
    int year;
    char *value = strtok(game_data_copy, ",");
    for (int column = 0; column < 5; column++)
    {
        value = strtok(NULL, ",");
    }

    year = atoi(value);
    return year;
}
