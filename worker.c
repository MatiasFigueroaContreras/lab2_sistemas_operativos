
#include "fworker.h"

int main(int argc, char *argv[])
{
    
    int fd_read, fd_write, initial_year;
    float min_price;
    
    initial_year = atoi(argv[3]);
    min_price = atof(argv[4]);
    // guardar el numero de lineas que se ha trabajado
    // read para leer la linea con informacion del juego -> Trae palabra FIN si no quedan mas lineas por leer.
    // read para leer el arreglo con la informacion guardada de los juegos por anio (YearData *)
    // write con la informacion actualizada de los years_data
}