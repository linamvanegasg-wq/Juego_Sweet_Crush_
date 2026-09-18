#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void guardarFicha(unsigned char* tablero, int posicion, int ficha)
{
    int bitInicio = posicion * 3;

    for (int i = 0; i < 3; i++)
    {
        int bitActual = bitInicio + i;

        int byte = bitActual / 8;
        int posicionBit = bitActual % 8;

        if ((ficha >> i) & 1)
        {
            tablero[byte] =tablero[byte] | (1 << posicionBit);
        }
        else
        {
            tablero[byte] =tablero[byte] & ~(1 << posicionBit);
        }
    }
}
int leerFicha(unsigned char* tablero, int posicion)
{
    int bitInicio = posicion * 3;
    int ficha = 0;

    for (int i = 0; i < 3; i++)
    {
        int bitActual = bitInicio + i;

        int byte = bitActual / 8;
        int posicionBit = bitActual % 8;

        if (tablero[byte] & (1 << posicionBit))
        {
            ficha = ficha | (1 << i);
        }
    }

    return ficha;
}


void mostrarTablero(unsigned char* tablero,int filas,int columnas)
{
    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0;columna < columnas; columna++)
        {
            int posicion =fila * columnas + columna;

            int ficha =leerFicha(tablero, posicion);


            if (ficha == 6)
            {
                cout << ". ";
            }
            else
            {
                char simbolo = 'A' + ficha;

                cout << simbolo << " ";
            }
        }

        cout << endl;
    }
}



void mostrarMemoria(unsigned char* tablero,int bytes)
{
    cout << "\nMemoria del tablero:\n";

    for (int i = 0; i < bytes; i++)
    {
        cout << "Byte " << i << ": ";

        for (int bit = 7; bit >= 0; bit--)
        {
            cout << ((tablero[i] >> bit) & 1);
        }

        cout << endl;
    }
}



bool eliminarFicha(unsigned char* tablero,int filas,int columnas)
{
    int fila;
    int columna;

    cout << "\nIngrese la fila de la ficha: ";
    cin >> fila;

    cout << "Ingrese la columna de la ficha: ";
    cin >> columna;


    if (fila < 0 || fila >= filas ||columna < 0 || columna >= columnas)
    {
        cout << "\nPosicion invalida.\n";

        return false;
    }


    int posicion =fila * columnas + columna;


    int ficha =
        leerFicha(tablero, posicion);


    if (ficha == 6)
    {
        cout << "\nLa posicion ya esta vacia.\n";

        return false;
    }


    guardarFicha(tablero, posicion, 6);

    cout << "\nFicha eliminada correctamente.\n";

    return true;
}



void limpiarMarcador(unsigned char* marcadas,int bytesMarcador)
{
    for (int i = 0; i < bytesMarcador; i++)
    {
        marcadas[i] = 0;
    }
}


bool marcarPosicion(unsigned char* marcadas,int posicion)
{
    int byte = posicion / 8;

    int posicionBit = posicion % 8;

    if (marcadas[byte] & (1 << posicionBit))
    {
        return false;
    }

    marcadas[byte] =marcadas[byte] | (1 << posicionBit);

    return true;
}



bool estaMarcada(unsigned char* marcadas,int posicion)
{
    int byte = posicion / 8;

    int posicionBit = posicion % 8;

    return marcadas[byte] &(1 << posicionBit);
}


int marcarCombinaciones(unsigned char* tablero,int filas,int columnas,unsigned char* marcadas)
{
    int cantidadMarcadas = 0;


    limpiarMarcador(marcadas,(filas * columnas + 7) / 8);



    for (int fila = 0; fila < filas; fila++)
    {
        int columna = 0;

        while (columna < columnas)
        {
            int posicion =fila * columnas + columna;

            int ficha =leerFicha(tablero, posicion);



            if (ficha >= 6)
            {
                columna++;
                continue;
            }


            int inicio = columna;

            int contador = 1;



            while (columna + contador < columnas)
            {
                int siguiente =fila * columnas +columna + contador;

                int fichaSiguiente =leerFicha(tablero, siguiente);


                if (fichaSiguiente == ficha)
                {
                    contador++;
                }
                else
                {
                    break;
                }
            }


            if (contador >= 3)
            {
                for (int i = 0;
                     i < contador;
                     i++)
                {
                    int pos =fila * columnas +inicio + i;

                    if (marcarPosicion(marcadas, pos))
                    {
                        cantidadMarcadas++;
                    }
                }
            }


            columna =inicio + contador;
        }
    }


    for (int columna = 0;columna < columnas; columna++)
    {
        int fila = 0;

        while (fila < filas)
        {
            int posicion =fila * columnas + columna;

            int ficha =leerFicha(tablero, posicion);


            if (ficha >= 6)
            {
                fila++;
                continue;
            }


            int inicio = fila;

            int contador = 1;


            while (fila + contador < filas)
            {
                int siguiente =
                    (fila + contador) *
                        columnas + columna;

                int fichaSiguiente =
                    leerFicha(tablero, siguiente);


                if (fichaSiguiente == ficha)
                {
                    contador++;
                }
                else
                {
                    break;
                }
            }


            if (contador >= 3)
            {
                for (int i = 0;
                     i < contador;
                     i++)
                {
                    int pos =
                        (inicio + i) *
                            columnas + columna;

                    if (marcarPosicion(marcadas, pos))
                    {
                        cantidadMarcadas++;
                    }
                }
            }


            fila =
                inicio + contador;
        }
    }


    return cantidadMarcadas;
}


int eliminarMarcadas(unsigned char* tablero,unsigned char* marcadas,int posiciones)
{
    int eliminadas = 0;


    for (int posicion = 0;
         posicion < posiciones;
         posicion++)
    {
        if (estaMarcada(marcadas, posicion))
        {
            guardarFicha(tablero,posicion,6);

            eliminadas++;
        }
    }


    return eliminadas;
}


void aplicarGravedad(unsigned char* tablero,int filas,int columnas)
{
    for (int columna = 0;columna < columnas;columna++)
    {
        int destino = filas - 1;

        for (int fila = filas - 1;
             fila >= 0;
             fila--)
        {
            int posicion =
                fila * columnas + columna;

            int ficha =
                leerFicha(tablero, posicion);


            if (ficha < 6)
            {
                int posicionDestino =
                    destino * columnas + columna;


                if (posicion != posicionDestino)
                {
                    guardarFicha(tablero,posicionDestino,ficha);

                    guardarFicha(tablero,posicion,6);
                }


                destino--;
            }
        }

        for (int fila = destino;
             fila >= 0;
             fila--)
        {
            int posicion =
                fila * columnas + columna;

            guardarFicha(tablero,posicion,6);
        }
    }
}


void rellenarTablero(unsigned char* tablero,
                     int filas,
                     int columnas)
{
    int posiciones =
        filas * columnas;


    for (int posicion = 0;
         posicion < posiciones;
         posicion++)
    {
        int ficha =
            leerFicha(tablero, posicion);


        if (ficha == 6)
        {
            int nuevaFicha =
                rand() % 6;

            guardarFicha(
                tablero,
                posicion,
                nuevaFicha
                );
        }
    }
}


void procesarCascadas(unsigned char* tablero,
                      int filas,
                      int columnas,
                      int& totalEliminadas,
                      int& combinaciones,
                      int& cascadas,
                      int& puntuacion)
{
    int posiciones =
        filas * columnas;


    int bytesMarcador =
        (posiciones + 7) / 8;


    unsigned char* marcadas =
        new unsigned char[bytesMarcador];


    limpiarMarcador(
        marcadas,
        bytesMarcador
        );


    int nivelCascada = 0;


    while (true)
    {
        int cantidadMarcadas =
            marcarCombinaciones(
                tablero,
                filas,
                columnas,
                marcadas
                );


        if (cantidadMarcadas == 0)
        {
            break;
        }


        int eliminadas =
            eliminarMarcadas(
                tablero,
                marcadas,
                posiciones
                );


        nivelCascada++;

        combinaciones++;

        cascadas++;

        totalEliminadas += eliminadas;

        puntuacion +=
            eliminadas * 10 * nivelCascada;


        cout << "\nCombinacion encontrada.\n";
        cout << "Fichas eliminadas: "
             << eliminadas << endl;

        cout << "Nivel de cascada: "
             << nivelCascada << endl;


        aplicarGravedad(
            tablero,
            filas,
            columnas
            );


        rellenarTablero(
            tablero,
            filas,
            columnas
            );


        cout << "\nTablero despues de la cascada:\n";

        mostrarTablero(
            tablero,
            filas,
            columnas
            );
    }


    delete[] marcadas;
}


void mostrarEstadisticas(int filas,
                         int columnas,
                         int eliminacionesUsuario,
                         int totalEliminadas,
                         int combinaciones,
                         int cascadas,
                         int puntuacion)
{
    cout << "\n";
    cout << "          ESTADISTICAS\n";

    cout << "Filas actuales: "
         << filas << endl;

    cout << "Columnas actuales: "
         << columnas << endl;

    cout << "Eliminaciones del usuario: "
         << eliminacionesUsuario << endl;

    cout << "Total fichas eliminadas: "
         << totalEliminadas << endl;

    cout << "Combinaciones detectadas: "
         << combinaciones << endl;

    cout << "Cascadas producidas: "
         << cascadas << endl;

    cout << "Puntuacion acumulada: "
         << puntuacion << endl;

    cout << "=====================================\n";
}


int main()
{
    srand(time(0));


    int filas;
    int columnas;


    cout << "=====================================\n";
    cout << "          SWEET CRUSH\n";
    cout << "=====================================\n";

    cout << "\nIngrese las filas: ";
    cin >> filas;

    cout << "Ingrese las columnas: ";
    cin >> columnas;


    if (filas <= 0 || columnas <= 0)
    {
        cout << "\nLas dimensiones deben ser mayores que cero.\n";

        return 0;
    }

    int posiciones =
        filas * columnas;


    cout << "\nCantidad de posiciones: "
         << posiciones << endl;


    int bits =
        posiciones * 3;


    cout << "Cantidad de bits: "
         << bits << endl;


    int bytes =
        (bits + 7) / 8;


    cout << "Cantidad de bytes: "
         << bytes << endl;


    unsigned char* tablero =
        new unsigned char[bytes];


    for (int i = 0;
         i < bytes;
         i++)
    {
        tablero[i] = 0;
    }

    for (int posicion = 0;
         posicion < posiciones;
         posicion++)
    {
        int ficha =
            rand() % 6;


        guardarFicha(
            tablero,
            posicion,
            ficha
            );
    }


    cout << "\nTablero inicial:\n";

    mostrarTablero(
        tablero,
        filas,
        columnas
        );


    mostrarMemoria(
        tablero,
        bytes
        );

    int eliminacionesUsuario = 0;

    int totalEliminadas = 0;

    int combinaciones = 0;

    int cascadas = 0;

    int puntuacion = 0;


    char continuar = 's';


    while (continuar == 's' ||
           continuar == 'S')
    {
        cout << "\n-------------------------------------\n";
        cout << "Nueva jugada\n";
        cout << "-------------------------------------\n";


        bool eliminada =
            eliminarFicha(
                tablero,
                filas,
                columnas
                );


        if (eliminada)
        {
            eliminacionesUsuario++;


            cout << "\nTablero despues de la eliminacion:\n";

            mostrarTablero(
                tablero,
                filas,
                columnas
                );


            procesarCascadas(
                tablero,
                filas,
                columnas,
                totalEliminadas,
                combinaciones,
                cascadas,
                puntuacion
                );

            aplicarGravedad(
                tablero,
                filas,
                columnas
                );


            rellenarTablero(
                tablero,
                filas,
                columnas
                );


            cout << "\nTablero final de la jugada:\n";

            mostrarTablero(
                tablero,
                filas,
                columnas
                );


            mostrarMemoria(
                tablero,
                bytes
                );


            mostrarEstadisticas(
                filas,
                columnas,
                eliminacionesUsuario,
                totalEliminadas,
                combinaciones,
                cascadas,
                puntuacion
                );
        }


        cout << "\nDesea realizar otra jugada? (s/n): ";
        cin >> continuar;
    }


    cout << "\n=====================================\n";
    cout << "        FIN DEL JUEGO\n";
    cout << "=====================================\n";


    mostrarEstadisticas(filas,columnas,eliminacionesUsuario,totalEliminadas,combinaciones,cascadas,puntuacion);


    delete[] tablero;


    return 0;
}
