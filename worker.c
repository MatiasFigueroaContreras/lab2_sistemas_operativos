#include "fworker.h"

int main(int argc, char *argv[])
{
    int fd_read, fd_write, initial_year;
    float min_price;
    fd_read = atoi(argv[1]);
    fd_write = atoi(argv[2]);
    min_price = atoi(argv[3]);
    initial_year = atof(argv[4]);
    int lines = 0;
    char msg[400];
    YearData years_data;
    while (read(fd_read, msg, sizeof(char) * 400) != -1 && strcmp(msg, "FIN") != 0)
    {
        //Se lee el mensaje del broker
        read(fd_read, &years_data, sizeof(YearData));
        //Se envia la respuesta al broker
        updateYearData(msg, &years_data, min_price, initial_year);
        write(fd_write, &years_data, sizeof(YearData));
        lines++;
    }
    write(fd_write, &lines, sizeof(int));

    close(fd_read);
    close(fd_write);

    return 0;
}
