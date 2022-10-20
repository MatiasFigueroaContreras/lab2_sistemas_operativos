#include "fworker.h"

int main(int argc, char *argv[])
{
    printf("Worker\n");
    int fd_read, fd_write, initial_year;
    float min_price;
    fd_read = atoi(argv[1]);
    fd_write = atoi(argv[2]);
    initial_year = atoi(argv[3]);
    min_price = atof(argv[4]);

    int lines = 0;
    char msg[400];
    YearData *years_data;
    printf("Worker 1\n"); 
    while ( read(fd_read, msg, sizeof(char) * 400) != -1 && strcmp(msg, "FIN") != 0)
    {
        //Se lee el mensaje del broker
        // printf("Worker While 1\n");
        printf("Mensaje recibido: %s", msg);
        read(fd_read, years_data, sizeof(YearData) * (2022 - initial_year + 1));
        // printf("Worker While 2\n");
        printf("Year Data no actualizado:\n%s", toString(&years_data[0]));
        //Se envia la respuesta al broker
        years_data = updateYearData(msg, years_data, min_price, initial_year);
        // printf("Worker While 3\n");
        write(fd_write, years_data, sizeof(YearData) * (2022 - initial_year + 1));
        // printf("Worker While 4\n");
        lines++;
        printf("Linea n°%d\n", lines);
    }

    printf("Worker 2\n");
    write(fd_write, &lines, sizeof(int));

    close(fd_read);
    close(fd_write);

    return 0;
}
