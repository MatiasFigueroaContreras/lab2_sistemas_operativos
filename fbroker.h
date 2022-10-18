#include "yearData.h"
#define STOP_WORKERS "FIN"
#define READING 0
#define WRITING 1

int **createWorkers(int num_workers); //Retorna los descriptores de archivos asociados a cada worker

char *getLine(FILE *input_file);

int *stopWorkers(int **fds); // Retorna las lineas trabajadas por cada worker.

void writeOutputFolder(YearData *years_data);

void printYearsData(YearData *years_data);

void printLineNumbersWorkers(int *line_numbers);
