#include "fbroker.h"

int main(int argc, char *argv[])
{
    // Datos validados por lab2:
    int initial_year, num_workers, print_flag;
    float min_price;
    char input_file[100], output_file[100];
    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[2]);
    initial_year = atoi(argv[3]);
    min_price = atof(argv[4]);
    print_flag = atoi(argv[5]);

    char game_data[400]; 
    int random_worker, *num_lines_workers;
    int ***fds = createWorkers(num_workers, min_price, initial_year);
    YearData *years_data = createYearsDataArray(initial_year);
    srand(time(NULL));
    FILE *read_file = fopen(input_file, "r");
    while (fgets(game_data, 400, read_file))
    {
        random_worker = rand() % num_workers;
        write(fds[num_workers][BROKER_WRITE][WRITING], game_data, sizeof(char) * 400);
        write(fds[num_workers][BROKER_WRITE][WRITING], years_data, sizeof(YearData) * (2022 - initial_year + 1));
        //Se espera respuesta del worker
        read(fds[num_workers][BROKER_READ][READING], years_data, sizeof(YearData) * (2022 - initial_year + 1));
    }

    num_lines_workers = stopWorkers(fds, num_workers);
    writeOutputFile(output_file, years_data, initial_year);

    if(print_flag)
    {
        printYearsData(years_data, initial_year);
        printLineNumbersWorkers(num_lines_workers);
    }

    return 0;
}