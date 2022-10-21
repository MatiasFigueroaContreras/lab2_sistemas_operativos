#include "yearData.h"
#include <sys/types.h>
#include <unistd.h>
#define READING 0
#define WRITING 1
#define BROKER_WRITE 0
#define BROKER_READ 1

int ***createWorkers(int num_workers, float min_price, int initial_year); // Retorna los descriptores de archivos asociados a cada worker

YearData *createYearsDataArray(int initial_year);

int *stopWorkers(int ***fds, int num_workers); // Retorna las lineas trabajadas por cada worker.

void writeOutputFile(char *output_file, YearData *years_data, int initial_year);

void printYearsData(YearData *years_data, int initial_year);

void printLineNumbersWorkers(int *line_numbers, int num_workers);

int getYear(char *game_data);
