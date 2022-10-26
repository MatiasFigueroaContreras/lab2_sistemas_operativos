#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/wait.h>

/*
    Entradas:
        - FILE *fp: Salida por consola
        - const char *path: Ruta del archivo
    Salidas: 
        - void
    Descripcion: 
        -Imprime la informacion de uso referente al formato
        correcto para la ejecucion del programa.

*/
void usage(FILE *fp, const char *path)
{
    const char *basename = path + 2;
    fprintf(fp, "usage: %s [OPTION]\n", basename);
    fprintf(fp, "The following is the correct format for using the program:\t\t"
                "\n'./lab2 -i input_file -o output_file -d year -p min_price -n num_workers -b'\n");
    fprintf(fp, "  -h, --help\t\t"
                "Print this help and exit.\n");
    fprintf(fp, "  -i, --input[=INPUTFILENAME]\t"
                "File to read games data.\n");
    fprintf(fp, "  -o, --output[=OUTPUTFILENAME]\t"
                "File to write the output data generated.\n");
    fprintf(fp, "  -d, --year[=YEAR]\t"
                "year required to start the search.\n");
    fprintf(fp, "  -p, --min[=MINPRICE]\t"
                "Minimum price required to start the search.\n");
    fprintf(fp, "  -n, --workers[=NUMWORKERS]\t"
                "Number of workers to create.\n");
    fprintf(fp, "  -b, --print[=PRINTFLAG]\t"
                "print the data in the console.\n");
}

int main(int argc, char *argv[])
{
    char s_num_workers[1000], s_year[6], s_min_price[10];
    char s_print_flag = '0';
    int help_flag = 0;
    int num_workers = 0;
    int opt, year = -1;
    float min_price = -1;
    char input_file[100], output_file[100];

    struct option longopts[] =
        {{"help", no_argument, &help_flag, 1},
         {"input", required_argument, NULL, 'i'},
         {"output", required_argument, NULL, 'o'},
         {"year", required_argument, NULL, 'd'},
         {"min", required_argument, NULL, 'p'},
         {"workers", required_argument, NULL, 'n'},
         {"print", optional_argument, NULL, 'b'}};

    while ((opt = getopt_long(argc, argv, ":i:o:d:p:n:b::h", longopts, 0)) != -1)
    {
        switch (opt)
        {
        case 'i': // Archivo de entrada
            strcpy(input_file, optarg);
            break;
        case 'o': // Archivo de salida
            strcpy(output_file, optarg);
            break;
        case 'd': // Año de inicio juego
            strcpy(s_year, optarg);
            year = atoi(s_year);
            break;
        case 'p': // Precio minimo
            strcpy(s_min_price, optarg);
            min_price = atof(s_min_price);
            break;
        case 'n': // Numero de workers
            strcpy(s_num_workers, optarg);
            num_workers = atoi(s_num_workers);
            break;
        case 'b': // flag para imprimir los datos por pantalla
            s_print_flag = '1';
            break;
        case 'h':
            usage(stdout, argv[0]);
            return 0;
        case '?': // flag que no existe
            usage(stderr, argv[0]);
            exit(EXIT_FAILURE);
        case ':': // cuando se usa mal una flag
            usage(stderr, argv[0]);
            exit(EXIT_FAILURE);
        default: // se aborta cuando falta flag o argumento obligatorio
            abort();
            break;
        }
    }

    if (input_file == NULL || output_file == NULL || year < 0 || min_price < 0 || num_workers <= 0 || help_flag)
    {
        // Entra si no se ingreso alguna opcion o valor necesario para
        //   el funcionamiento del programa, ademas si se ingresa
        //   la flag de ayuda.
        usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    if(access(input_file, 0) != 0)
    {
        // Entra si el archivo de entrada no existe
        perror("Input file doesn't exist\n");
        exit(EXIT_FAILURE);
    }

    if(fork() == 0){
        execl("./broker", "./broker", input_file, output_file, s_year, s_min_price, s_num_workers, &s_print_flag, NULL);
        perror("Exec ls failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL); // Se espera a que el broker termine
        return 0;
    }
}