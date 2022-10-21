#include "fworker.h"

int main(int argc, char *argv[])
{
    printf("Worker\n");
    int fd_read, fd_write, initial_year;
    float min_price;
    fd_read = atoi(argv[1]);
    fd_write = atoi(argv[2]);
    min_price = atoi(argv[3]);
    initial_year = atof(argv[4]);
    printf("Worker 1.0\n");
    int lines = 0;
    int max_years = 2022 - initial_year + 1;
    printf("Min price: %f, Initial Year: %d, Max Years: %d\n", min_price, initial_year, max_years);
    char msg[400];
    printf("Worker 1.1\n");
    YearData years_data;
    printf("Worker 1.2\n");
    while (read(fd_read, msg, sizeof(char) * 400) != -1 && strcmp(msg, "FIN") != 0)
    {
        //Se lee el mensaje del broker
        // printf("Worker While 1\n");
        printf("Mensaje recibido: %s\n", msg);
        read(fd_read, &years_data, sizeof(YearData));
        printf("Worker While 2\n");
        printf("%s", toString(&years_data)); // con max_years - 16 no recibe bien los datos
        //Se envia la respuesta al broker
        updateYearData(msg, &years_data, min_price, initial_year);
        // printf("Worker While 3\n");
        write(fd_write, &years_data, sizeof(YearData) * max_years);
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
