#include "fworker.h"

int main(int argc, char *argv[])
{
    // Datos entregados por el broker en la creacion del worker.
    int fd_read, initial_year;
    float min_price;
    fd_read = atoi(argv[1]);
    min_price = atoi(argv[2]);
    initial_year = atof(argv[3]);

    int lines = 0;
    char msg[400];
    YearData years_data;
    // Se lee el mensaje con la informacion del juego hasta que este sea FIN.
    while (read(fd_read, msg, sizeof(char) * 400) != -1 && strcmp(msg, "FIN") != 0)
    {
        // Se lee la informacion referente al anio del juego.
        read(fd_read, &years_data, sizeof(YearData));
        // Se envia la respuesta con la informacion actualizada al broker.
        updateYearData(msg, &years_data, min_price, initial_year);
        write(STDOUT_FILENO, &years_data, sizeof(YearData));
        lines++;
    }
    // Se envian las lineas trabajadas al broker.
    write(STDOUT_FILENO, &lines, sizeof(int));

    close(fd_read);

    return 0;
}
