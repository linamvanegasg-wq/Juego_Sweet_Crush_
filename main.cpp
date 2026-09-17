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
            tablero[byte] = tablero[byte] | (1 << posicionBit);
        }
        else
        {
            tablero[byte] = tablero[byte] & ~(1 << posicionBit);
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

void mostrarTablero(unsigned char* tablero, int filas, int columnas)
{
    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0; columna < columnas; columna++)
        {
            int posicion = fila * columnas + columna;

            int ficha = leerFicha(tablero, posicion);

            char simbolo = 'A' + ficha;

            cout << simbolo << " ";
        }

        cout << endl;
    }
}
int main()
{
    srand(time(0));

    int filas, columnas;

    cout << "Ingrese las filas: ";
    cin >> filas;

    cout << "Ingrese las columnas: ";
    cin >> columnas;


    int posiciones = filas * columnas;

    cout << "Cantidad de posiciones: "
         << posiciones << endl;


    int bits = posiciones * 3;

    cout << "Cantidad de bits: "
         << bits << endl;


    int bytes = (bits + 7) / 8;

    cout << "Cantidad de bytes: "
         << bytes << endl;


    unsigned char* tablero = new unsigned char[bytes];

    for (int i = 0; i < bytes; i++)
    {
        tablero[i] = 0;
    }

    for (int i = 0; i < posiciones; i++)
    {
        int ficha = rand() % 6;

        guardarFicha(tablero, i, ficha);
    }

    mostrarTablero(tablero, filas, columnas);

    delete[] tablero;

    return 0;
}
