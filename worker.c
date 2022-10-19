
#include "fworker.h"

int main(int argc, char *argv[])
{
    int lineas=0;
    int fd_read, fd_write, initial_year;
    float min_price;
    initial_year = atoi(argv[3]);
    min_price = atof(argv[4]);
    char msg[100];
    while(read(fd_read,&msg, sizeof(YearData) * (2022 - initial_year + 1))&& msg!='FIN' ) //¿el -1 que indica?
    {
        //Se lee el mensaje del broker
        read(fd_read, &years_data, sizeof(YearData) * (2022 - initial_year + 1));
        //Se envia la respuesta al broker
        write(fd_write, years_data, sizeof(YearData) * (2022 - initial_year + 1));
        lineas++;
    }
    // guardar el numero de lineas que se ha trabajado
    // read para leer la linea con informacion del juego -> Trae palabra FIN si no quedan mas lineas por leer.
    // read para leer el arreglo con la informacion guardada de los juegos por anio (YearData *)
    // write con la informacion actualizada de los years_data
}