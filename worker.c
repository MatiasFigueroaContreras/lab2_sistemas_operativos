#include "fworker.h"

int main(int argc, char *argv[])
{
    int fd_read, initial_year;
    float min_price;
    fd_read = atoi(argv[1]);
    min_price = atoi(argv[2]);
    initial_year = atof(argv[3]);
    int lines = 0;
    char msg[400];
    YearData years_data;
    while (read(fd_read, msg, sizeof(char) * 400) != -1 && strcmp(msg, "FIN") != 0)
    {
        //Se lee el mensaje del broker
        read(fd_read, &years_data, sizeof(YearData));
        //Se envia la respuesta al broker
        updateYearData(msg, &years_data, min_price, initial_year);
        write(STDOUT_FILENO, &years_data, sizeof(YearData));
        lines++;
    }
    write(STDOUT_FILENO, &lines, sizeof(int));

    close(fd_read);

    return 0;
}
