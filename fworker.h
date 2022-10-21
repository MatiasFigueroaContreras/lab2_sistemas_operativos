#include "yearData.h"
#include <sys/types.h>
#include <unistd.h>
#define STOP_WORKERS "FIN"
#define READING 0
#define WRITING 1

void updateYearData(char *line, YearData *years_data, float min_price, int initial_year);
