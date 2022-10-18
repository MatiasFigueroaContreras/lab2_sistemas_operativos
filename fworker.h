#include "yearData.h"
#define STOP_WORKERS "FIN"
#define READING 0
#define WRITING 1

char *getLine(int fd);

YearData *getYearsData(int fd);

YearData *updateYearData(char *line, YearData *years_data);
