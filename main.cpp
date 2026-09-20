#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void guardarFicha(unsigned char* tablero, int bytesTablero, int posicion, int ficha)
{
    if (tablero == nullptr || bytesTablero <= 0 || posicion < 0)
    {
        return;
    }

    int bitInicio = posicion * 3;

    for (int i = 0; i < 3; i++)
    {
        int bitActual = bitInicio + i;
        int byte = bitActual / 8;
        int posicionBit = bitActual % 8;

        if (byte < 0 || byte >= bytesTablero)
        {
            return;
        }

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

int leerFicha(unsigned char* tablero,int bytesTablero,int posicion)
{
    int bitInicio = posicion * 3;
    int ficha = 0;

    for (int i = 0; i < 3; i++)
    {
        int bitActual = bitInicio + i;
        int byte = bitActual / 8;
        int posicionBit = bitActual % 8;

        if (byte < 0 || byte >= bytesTablero)
        {
            return 6;
        }

        if (tablero[byte] & (1 << posicionBit))
        {
            ficha = ficha | (1 << i);
        }
    }

    return ficha;
}


void mostrarTablero(unsigned char* tablero,int bytesTablero,int filas,int columnas)
{
    cout << endl;
    cout << "TABLERO" << endl;
    cout << endl;

    for (int fila = 0; fila < filas; fila++)
    {
        for (int columna = 0; columna < columnas; columna++)
        {
            int posicion = fila * columnas + columna;
            int ficha = leerFicha(tablero,bytesTablero,posicion);

            if (ficha == 6)
            {
                cout << ". ";
            }
            else if (ficha >= 0 && ficha <= 5)
            {
                cout << char('A' + ficha) << " ";
            }
            else
            {
                cout << "? ";
            }
        }

        cout << endl;
    }

    cout << endl;
}

void mostrarMemoria(unsigned char* tablero,int bytesTablero,int bitsNecesarios)
{
    cout << endl;
    cout << "MEMORIA DEL TABLERO" << endl;
    cout << endl;

    int contadorBits = 0;

    for (int byte = 0; byte < bytesTablero; byte++)
    {
        cout << "Byte " << byte << ": ";

        for (int bit = 0; bit < 8; bit++)
        {
            if (contadorBits < bitsNecesarios)
            {
                if (tablero[byte] & (1 << bit))
                {
                    cout << "1";
                }
                else
                {
                    cout << "0";
                }
            }
            else
            {
                cout << "-";
            }
            contadorBits++;
        }
        cout << endl;
    }

    cout << endl;
}

void rellenarInicial(unsigned char* tablero,int bytesTablero,int posiciones)
{
    for (int posicion = 0; posicion < posiciones; posicion++)
    {
        int ficha = rand() % 6;

        guardarFicha(tablero,bytesTablero,posicion,ficha);
    }
}

bool eliminarFichaJugador(unsigned char* tablero,int bytesTablero,int filas,int columnas,int fila,int columna,int& eliminacionesJugador)
{
    if (fila < 0 || fila >= filas ||columna < 0 || columna >= columnas)
    {
        cout << "Posicion fuera del tablero." << endl;
        return false;
    }

    int posicion = fila * columnas + columna;
    int ficha = leerFicha(tablero,bytesTablero,posicion);

    if (ficha == 6)
    {
        cout << "Esa posicion ya esta vacia." << endl;
        return false;
    }

    guardarFicha(tablero,bytesTablero,posicion,6);

    eliminacionesJugador++;

    cout << "Ficha eliminada." << endl;

    return true;
}

int marcarCombinaciones(unsigned char* tablero,int bytesTablero,int filas,int columnas,unsigned char* marcas)
{
    int posiciones = filas * columnas;

    for (int i = 0; i < posiciones; i++)
    {
        marcas[i] = 0;
    }

    int totalMarcadas = 0;

    for (int fila = 0; fila < filas; fila++)
    {
        int columna = 0;

        while (columna < columnas)
        {
            int posicion = fila * columnas + columna;
            int ficha = leerFicha(tablero,bytesTablero,posicion);

            if (ficha == 6)
            {
                columna++;
                continue;
            }

            int inicio = columna;
            int cantidad = 1;

            columna++;

            while (columna < columnas)
            {
                int siguientePosicion =fila * columnas + columna;
                int siguienteFicha =leerFicha(tablero,bytesTablero,siguientePosicion);

                if (siguienteFicha == ficha)
                {
                    cantidad++;
                    columna++;
                }
                else
                {
                    break;
                }
            }

            if (cantidad >= 3)
            {
                for (int c = inicio;c < inicio + cantidad;c++)
                {
                    int posicionMarca =fila * columnas + c;

                    if (marcas[posicionMarca] == 0)
                    {
                        marcas[posicionMarca] = 1;
                        totalMarcadas++;
                    }
                }
            }
        }
    }

    for (int columna = 0; columna < columnas; columna++)
    {
        int fila = 0;

        while (fila < filas)
        {
            int posicion =fila * columnas + columna;
            int ficha =leerFicha(tablero,bytesTablero,posicion);

            if (ficha == 6)
            {
                fila++;
                continue;
            }

            int inicio = fila;
            int cantidad = 1;

            fila++;

            while (fila < filas)
            {
                int siguientePosicion =fila * columnas + columna;
                int siguienteFicha =leerFicha(tablero,bytesTablero,siguientePosicion);

                if (siguienteFicha == ficha)
                {
                    cantidad++;
                    fila++;
                }
                else
                {
                    break;
                }
            }

            if (cantidad >= 3)
            {
                for (int f = inicio;f < inicio + cantidad;f++)
                {
                    int posicionMarca = f * columnas + columna;

                    if (marcas[posicionMarca] == 0)
                    {
                        marcas[posicionMarca] = 1;
                        totalMarcadas++;
                    }
                }
            }
        }
    }

    return totalMarcadas;
}

int eliminarMarcadas(unsigned char* tablero,int bytesTablero,int posiciones,unsigned char* marcas)
{
    int eliminadas = 0;

    for (int posicion = 0;posicion < posiciones;posicion++)
    {
        if (marcas[posicion] == 1)
        {
            guardarFicha(tablero,bytesTablero,posicion,6);

            eliminadas++;
        }
    }

    return eliminadas;
}


void aplicarGravedad(unsigned char* tablero,int bytesTablero,int filas,int columnas)
{
    for (int columna = 0;columna < columnas; columna++)
    {
        int destino = filas - 1;

        for (int fila = filas - 1;fila >= 0;fila--)
        {
            int posicionActual =fila * columnas + columna;
            int ficha =leerFicha(tablero, bytesTablero,posicionActual);

            if (ficha != 6)
            {
                int posicionDestino =destino * columnas + columna;

                guardarFicha(tablero,bytesTablero,posicionDestino,ficha);

                if (destino != fila)
                {
                    guardarFicha(tablero,bytesTablero,posicionActual,6);
                }
                destino--;
            }
        }

        while (destino >= 0)
        {
            int posicionVacia =destino * columnas + columna;

            guardarFicha(tablero,bytesTablero,posicionVacia, 6);

            destino--;
        }
    }
}

int rellenarTablero(unsigned char* tablero,int bytesTablero,int filas,int columnas)
{
    int nuevasFichas = 0;

    for (int fila = 0;fila < filas;fila++)
    {
        for (int columna = 0;columna < columnas;columna++)
        {
            int posicion =fila * columnas + columna;
            int ficha =leerFicha(tablero,bytesTablero,posicion);

            if (ficha == 6)
            {
                int nuevaFicha = rand() % 6;

                guardarFicha(tablero, bytesTablero, posicion, nuevaFicha);

                nuevasFichas++;
            }
        }
    }

    return nuevasFichas;
}

int procesarCascadas(unsigned char* tablero,int bytesTablero,int filas,int columnas,int& cantidadCascadas)
{
    int posiciones = filas * columnas;
    unsigned char* marcas =new unsigned char[posiciones];
    int totalEliminadas = 0;
    cantidadCascadas = 0;

    while (true)
    {
        int encontradas =
            marcarCombinaciones(tablero,bytesTablero,filas, columnas, marcas);

        if (encontradas == 0)
        {
            break;
        }
        cantidadCascadas++;

        int eliminadas =eliminarMarcadas(tablero,bytesTablero,posiciones,marcas);

        totalEliminadas += eliminadas;

        aplicarGravedad(tablero,bytesTablero,filas,columnas);
        rellenarTablero(tablero,bytesTablero,filas,columnas);
    }

    delete[] marcas;

    return totalEliminadas;
}

int calcularBytes(int filas,int columnas)
{
    int posiciones = filas * columnas;
    int bits = posiciones * 3;
    int bytes = (bits + 7) / 8;

    return bytes;
}

void redimensionarTablero(unsigned char*& tablero,int& bytesTablero,int filasViejas,int columnasViejas,int filasNuevas,int columnasNuevas)
{
    int posicionesNuevas =filasNuevas * columnasNuevas;
    int bytesNuevos = calcularBytes(filasNuevas, columnasNuevas);

    unsigned char* nuevoTablero =new unsigned char[bytesNuevos];

    for (int i = 0; i < bytesNuevos; i++)
    {
        nuevoTablero[i] = 0;
    }

    int filasCopiar = filasViejas;

    if (filasNuevas < filasCopiar)
    {
        filasCopiar = filasNuevas;
    }

    int columnasCopiar = columnasViejas;

    if (columnasNuevas < columnasCopiar)
    {
        columnasCopiar = columnasNuevas;
    }

    for (int fila = 0;fila < filasCopiar;fila++)
    {
        for (int columna = 0;columna < columnasCopiar;columna++)
        {
            int posicionVieja =fila * columnasViejas + columna;
            int posicionNueva =fila * columnasNuevas + columna;
            int ficha =leerFicha(tablero,bytesTablero,posicionVieja);

            guardarFicha(nuevoTablero,bytesNuevos,posicionNueva,ficha);
        }
    }


    delete[] tablero;

    tablero = nuevoTablero;

    bytesTablero = bytesNuevos;

    (void)posicionesNuevas;
}

void insertarFila(unsigned char*& tablero,int& bytesTablero,int& filas,int columnas,int posicionFila)
{
    if (posicionFila < 0 ||posicionFila > filas)
    {
        cout << "Posicion de fila invalida." << endl;
        return;
    }

    int filasNuevas = filas + 1;
    int bytesNuevos =calcularBytes(filasNuevas, columnas);

    unsigned char* nuevoTablero = new unsigned char[bytesNuevos];

    for (int i = 0; i < bytesNuevos; i++)
    {
        nuevoTablero[i] = 0;
    }


    for (int fila = 0;fila < filasNuevas;fila++)
    {
        for (int columna = 0;columna < columnas;columna++)
        {
            int posicionNueva =fila * columnas + columna;

            if (fila == posicionFila)
            {
                int ficha = rand() % 6;

                guardarFicha(nuevoTablero,bytesNuevos,posicionNueva,ficha);
            }
            else
            {
                int filaVieja = fila;

                if (fila > posicionFila)
                {
                    filaVieja--;
                }

                int posicionVieja =filaVieja * columnas + columna;
                int ficha =leerFicha(tablero,bytesTablero,posicionVieja);

                guardarFicha(nuevoTablero,bytesNuevos,posicionNueva,ficha);
            }
        }
    }

    delete[] tablero;

    tablero = nuevoTablero;

    bytesTablero = bytesNuevos;

    filas = filasNuevas;
}

void eliminarFila(unsigned char*& tablero,int& bytesTablero,int& filas,int columnas,int posicionFila)
{
    if (filas <= 1)
    {
        cout << "No se puede eliminar la unica fila." << endl;
        return;
    }

    if (posicionFila < 0 ||posicionFila >= filas)
    {
        cout << "Posicion de fila invalida." << endl;
        return;
    }

    int filasNuevas = filas - 1;

    int bytesNuevos =calcularBytes(filasNuevas,columnas);

    unsigned char* nuevoTablero =new unsigned char[bytesNuevos];

    for (int i = 0; i < bytesNuevos; i++)
    {
        nuevoTablero[i] = 0;
    }


    for (int fila = 0;fila < filasNuevas;fila++)
    {
        for (int columna = 0;columna < columnas;columna++)
        {
            int filaVieja = fila;

            if (fila >= posicionFila)
            {
                filaVieja++;
            }

            int posicionVieja =filaVieja * columnas + columna;
            int posicionNueva =fila * columnas + columna;
            int ficha =leerFicha(tablero, bytesTablero, posicionVieja);

            guardarFicha(nuevoTablero,bytesNuevos,posicionNueva,ficha);
        }
    }

    delete[] tablero;

    tablero = nuevoTablero;

    bytesTablero = bytesNuevos;

    filas = filasNuevas;
}

void insertarColumna(unsigned char*& tablero,int& bytesTablero,int filas,int& columnas,int posicionColumna)
{
    if (posicionColumna < 0 ||
        posicionColumna > columnas)
    {
        cout << "Posicion de columna invalida." << endl;
        return;
    }

    int columnasNuevas = columnas + 1;

    int bytesNuevos =calcularBytes(filas,columnasNuevas);

    unsigned char* nuevoTablero =new unsigned char[bytesNuevos];

    for (int i = 0; i < bytesNuevos; i++)
    {
        nuevoTablero[i] = 0;
    }


    for (int fila = 0;fila < filas;fila++)
    {
        for (int columna = 0; columna < columnasNuevas; columna++)
        {
            int posicionNueva =fila * columnasNuevas + columna;

            if (columna == posicionColumna)
            {
                int ficha = rand() % 6;

                guardarFicha(nuevoTablero,bytesNuevos,posicionNueva,ficha);
            }
            else
            {
                int columnaVieja = columna;

                if (columna > posicionColumna)
                {
                    columnaVieja--;
                }

                int posicionVieja =fila * columnas + columnaVieja;

                int ficha =leerFicha(tablero,bytesTablero,posicionVieja);

                guardarFicha(nuevoTablero,bytesNuevos,posicionNueva,ficha);
            }
        }
    }

    delete[] tablero;

    tablero = nuevoTablero;

    bytesTablero = bytesNuevos;

    columnas = columnasNuevas;
}

void eliminarColumna(unsigned char*& tablero,int& bytesTablero,int filas,int& columnas,int posicionColumna)
{
    if (columnas <= 1)
    {
        cout << "No se puede eliminar la unica columna." << endl;
        return;
    }

    if (posicionColumna < 0 || posicionColumna >= columnas)
    {
        cout << "Posicion de columna invalida." << endl;
        return;
    }

    int columnasNuevas = columnas - 1;

    int bytesNuevos = calcularBytes(filas,columnasNuevas);

    unsigned char* nuevoTablero =new unsigned char[bytesNuevos];

    for (int i = 0; i < bytesNuevos; i++)
    {
        nuevoTablero[i] = 0;
    }


    for (int fila = 0;fila < filas;fila++)
    {
        for (int columna = 0;columna < columnasNuevas;columna++)
        {
            int columnaVieja = columna;

            if (columna >= posicionColumna)
            {
                columnaVieja++;
            }

            int posicionVieja =fila * columnas + columnaVieja;

            int posicionNueva =fila * columnasNuevas + columna;

            int ficha =leerFicha(tablero,bytesTablero,posicionVieja);

            guardarFicha(nuevoTablero, bytesNuevos, posicionNueva, ficha);
        }
    }

    delete[] tablero;

    tablero = nuevoTablero;

    bytesTablero = bytesNuevos;

    columnas = columnasNuevas;
}

void mostrarEstadisticas(int filas,int columnas,int bytesTablero,int eliminacionesJugador,int totalEliminadas,int totalCascadas,int puntuacion)
{
    int posiciones = filas * columnas;

    int bits = posiciones * 3;

    cout << endl;
    cout << "           ESTADISTICAS             " << endl;

    cout << "Filas: " << filas << endl;
    cout << "Columnas: " << columnas << endl;
    cout << "Posiciones: " << posiciones << endl;
    cout << "Bits utilizados: " << bits << endl;
    cout << "Bytes reservados: " << bytesTablero << endl;
    cout << "Fichas eliminadas por jugador: " << eliminacionesJugador << endl;
    cout << "Fichas eliminadas por combinaciones: " << totalEliminadas << endl;
    cout << "Cascadas: " << totalCascadas << endl;
    cout << "Puntuacion: " << puntuacion << endl;

}

int main()
{
    srand(time(0));


    int filas;
    int columnas;

    cout << "          SWEET CRUSH               " << endl;

    cout << endl;

    cout << "Ingrese las filas: ";
    cin >> filas;

    cout << "Ingrese las columnas: ";
    cin >> columnas;

    if (filas <= 0 || columnas <= 0)
    {
        cout << "Las dimensiones deben ser mayores que cero."<< endl;

        return 0;
    }

    int posiciones = filas * columnas;

    int bits = posiciones * 3;

    int bytes = (bits + 7) / 8;

    unsigned char* tablero = new unsigned char[bytes];

    for (int i = 0; i < bytes; i++)
    {
        tablero[i] = 0;
    }

    rellenarInicial(tablero,bytes,posiciones);

    int eliminacionesJugador = 0;

    int totalEliminadas = 0;

    int totalCascadas = 0;

    int puntuacion = 0;

    bool salir = false;

    while (!salir)
    {
        cout << endl;
        cout << "              MENU                  " << endl;
        cout << "1. Mostrar tablero" << endl;
        cout << "2. Eliminar ficha" << endl;
        cout << "3. Mostrar memoria" << endl;
        cout << "4. Insertar fila" << endl;
        cout << "5. Eliminar fila" << endl;
        cout << "6. Insertar columna" << endl;
        cout << "7. Eliminar columna" << endl;
        cout << "8. Mostrar estadisticas" << endl;
        cout << "9. Salir" << endl;

        cout << endl;
        cout << "Seleccione una opcion: ";

        int opcion;
        cin >> opcion;

        if (opcion == 1)
        {
            mostrarTablero(tablero,bytes,filas,columnas);
        }
        else if (opcion == 2)
        {
            int fila;
            int columna;

            cout << endl;

            cout << "Ingrese la fila: ";
            cin >> fila;

            cout << "Ingrese la columna: ";
            cin >> columna;

            fila--;
            columna--;

            bool eliminada =
                eliminarFichaJugador(tablero,bytes,filas,columnas,fila,columna,eliminacionesJugador);

            if (eliminada)
            {
                int cascadasActuales = 0;

                int eliminadas =procesarCascadas(tablero, bytes, filas, columnas, cascadasActuales);

                totalEliminadas += eliminadas;

                totalCascadas += cascadasActuales;

                puntuacion += 10;

                puntuacion +=eliminadas * 10;

                if (cascadasActuales > 0)
                {
                    puntuacion += cascadasActuales * 20;
                }
                cout << endl;

                cout << "Fichas eliminadas por " "combinaciones: "<< eliminadas << endl;

                cout << "Cascadas realizadas: " << cascadasActuales << endl;

                cout << "Puntuacion actual: " << puntuacion << endl;
            }
        }
        else if (opcion == 3)
        {
            int bitsNecesarios =filas * columnas * 3;

            mostrarMemoria(tablero,bytes,bitsNecesarios);
        }

        else if (opcion == 4)
        {
            int posicionFila;

            cout << endl;

            cout << "Ingrese la posicion donde ""insertar la fila (1 hasta "<< filas + 1 << "): ";
            cin >> posicionFila;

            posicionFila--;

            insertarFila(tablero,bytes,filas,columnas,posicionFila);

            cout << "Fila insertada." << endl;

            mostrarTablero(tablero,bytes,filas,columnas);
        }
        else if (opcion == 5)
        {
            int posicionFila;

            cout << endl;

            cout << "Ingrese la fila que desea ""eliminar (1 hasta "<< filas << "): ";

            cin >> posicionFila;

            posicionFila--;

            eliminarFila(tablero,bytes,filas,columnas,posicionFila);

            cout << "Fila eliminada." << endl;

            mostrarTablero(tablero,bytes,filas,columnas);
        }
        else if (opcion == 6)
        {
            int posicionColumna;

            cout << endl;

            cout << "Ingrese la posicion donde ""insertar la columna (1 hasta " << columnas + 1 << "): ";

            cin >> posicionColumna;

            posicionColumna--;

            insertarColumna(tablero,bytes,filas, columnas, posicionColumna);

            cout << "Columna insertada." << endl;

            mostrarTablero(tablero,bytes,filas,columnas);
        }
        else if (opcion == 7)
        {
            int posicionColumna;

            cout << endl;

            cout << "Ingrese la columna que desea ""eliminar (1 hasta " << columnas << "): ";

            cin >> posicionColumna;

            posicionColumna--;

            eliminarColumna(tablero,bytes, filas, columnas, posicionColumna);

            cout << "Columna eliminada." << endl;

            mostrarTablero(tablero,bytes,filas,columnas);
        }

        else if (opcion == 8)
        {
            mostrarEstadisticas(filas,columnas,bytes,eliminacionesJugador,totalEliminadas,totalCascadas,puntuacion);
        }

        else if (opcion == 9)
        {
            salir = true;
        }
      else
        {
            cout << "Opcion no valida." << endl;
        }
    }

    delete[] tablero;

    tablero = nullptr;


    cout << endl;
    cout << "Memoria liberada correctamente." << endl;
    cout << "Programa finalizado." << endl;

    return 0;
}
