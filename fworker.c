#include "yearData.h"

/*
    Entradas:
        -char *line: Cantidad de lineas  de la informacion del juego
        -YearData *years_data: Arreglo de yearData
        -float min_price: Precio minimo a considerar
        -int initial_year: Anio a partir del cual se parte como base hasta el anio actual
    Salidas:
        -YearData: arreglo con YearData, actualizado
    Descripcion: Funcion encargada de actualizar la informacion de YearData
        dada un string (line) con informacion referente a un juego.
*/
void updateYearData(char *line, YearData *years_data, float min_price, int initial_year)
{
    int year, in_windows = 0, in_mac = 0, in_linux = 0, is_free = 0;
    float price;
    char game_name[100];
    
    int column = 0;
    char *value = strtok(line, ",");
    while (value)
    {
        switch (column)
        {
        case 1:
            strcpy(game_name, value);
            break;
        case 3:
            price = atof(value);
            break;
        case 5:
            year = atoi(value);
            break;
        case 6:
            if (strcmp("True", value) == 0)
            {
                is_free = 1;
            }
            break;
        case 7:
            if (strcmp("Yes", value) == 0)
            {
                in_windows = 1;
            }
            break;
        case 8:
            if (strcmp("Yes", value) == 0)
            {
                in_mac= 1;
            }
            break;
        case 9:
            if (value[0] == 'Y')
            {
                in_linux = 1;
            }
            break;
        default:
            break;
        }

        value = strtok(NULL, ",");
        column++;
    }
    if (initial_year <= year && min_price <= price)
    {
        years_data->total_games++;
        years_data->sum_prices += price;
        if(is_free)
        {
            strcpy(years_data->free_games[years_data->num_free_games], game_name);
            years_data->num_free_games++;
        }

        if(in_windows)
        {
            years_data->windows_games++;
        }

        if (in_mac)
        {
            years_data->mac_games++;
        }

        if (in_linux)
        {
            years_data->linux_games++;
        }

        if (price > years_data->price_expensive_game)
        {
            years_data->price_expensive_game = price;
            strcpy(years_data->expensive_game, game_name);
        }
        if (price < years_data->price_cheap_game)
        {
            years_data->price_cheap_game = price;
            strcpy(years_data->cheap_game, game_name);
        }
    }
}
