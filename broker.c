#include "fbroker.h"
#include <time.h>

int main(int argc, char *argv[])
{
    // Datos validados por lab2:
    printf("Broker\n");
    int initial_year, num_workers, print_flag;
    float min_price;
    char input_file[100], output_file[100];
    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[2]);
    initial_year = atoi(argv[3]);
    min_price = atof(argv[4]);
    num_workers = atoi(argv[5]);
    print_flag = atoi(argv[6]);

    char game_data[400]; 
    int random_worker, *num_lines_workers;
    int ***fds = createWorkers(num_workers, min_price, initial_year);
    for(int i = 0; i < num_workers; i++)
    {
        printf("Worker n°%d\n", i + 1);
        printf("Escritura Broker Fd[0]: %d, Fd[1]: %d\n", fds[i][0][0], fds[i][0][1]);
        printf("Lectura Broker Fd[0]: %d, Fd[1]: %d\n", fds[i][1][0], fds[i][1][1]);
    }

    printf("1\n");
    int max_years = 2022 - initial_year + 1;
    int game_year;
    YearData *years_data = malloc(sizeof(YearData) * max_years);
    unsigned long cap = sizeof(YearData) * max_years;
    printf("Cap: %lu\n", cap);
    years_data = createYearsDataArray(initial_year);
    printf("2\n");
    for(int i = 0; i < max_years;  i++)
    {
        printf("%s", toString(&years_data[i]));
    }
    srand(time(NULL));
    printf("3\n");
    FILE *read_file = fopen(input_file, "r");
    while (fgets(game_data, 400, read_file))
    {
        printf("while 1\n");
        printf("Game data: %s", game_data);
        random_worker = rand() % num_workers;
        game_year = getYear(game_data);
        printf("Random worker: %d\n", random_worker);
        printf("while 2\n");
        write(fds[random_worker][BROKER_WRITE][WRITING], game_data, sizeof(char) * 400);
        printf("while 3\n");
        write(fds[random_worker][BROKER_WRITE][WRITING], &years_data[game_year], sizeof(YearData));
        printf("while 4\n");
        //Se espera respuesta del worker
        read(fds[random_worker][BROKER_READ][READING], &years_data[game_year], sizeof(YearData));
        printf("while 5\n");
        printf("%s\n", toString(&years_data[game_year]));
    }
    printf("4\n");
    num_lines_workers = stopWorkers(fds, num_workers);
    printf("5\n");
    writeOutputFile(output_file, years_data, initial_year);
    printf("6\n");
    if(print_flag)
    {
        printYearsData(years_data, initial_year);
        printLineNumbersWorkers(num_lines_workers, num_workers);
    }
    printf("7\n");
    return 0;
}