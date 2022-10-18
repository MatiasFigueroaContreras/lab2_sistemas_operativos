#include "yearData.h"
#define READING 0
#define WRITING 1
#define BROKER_WRITE 0
#define BROKER_READ 1

int ***createWorkers(int num_workers); //Retorna los descriptores de archivos asociados a cada worker

char *getLine(FILE *input_file);

int *stopWorkers(int ***fds); // Retorna las lineas trabajadas por cada worker.

void writeOutputFile(char *output_file, YearData *years_data, int num_years);

void printYearsData(YearData *years_data, int num_years);

void printLineNumbersWorkers(int *line_numbers);
