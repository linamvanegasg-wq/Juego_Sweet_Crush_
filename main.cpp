#include <iostream>
using namespace std;

int main()
{
    int filas, columnas;

    cout << "Ingrese las filas: ";
    cin >> filas;

    cout << "Ingrese las columnas: ";
    cin >> columnas;

    int posiciones = filas * columnas;

    cout << "Cantidad de posiciones: " << posiciones << endl;

    int bits = posiciones * 3;

    cout << "Cantidad de bits: " << bits << endl;

    int bytes = (bits + 7) / 8;

    cout << "Cantidad de bytes: " << bytes << endl;

    unsigned char* tablero = new unsigned char[bytes];

    for (int i = 0; i < bytes; i++)
    {
        tablero[i] = 0;
    }

    cout << "Memoria creada correctamente." << endl;

    delete[] tablero;


    return 0;
}
