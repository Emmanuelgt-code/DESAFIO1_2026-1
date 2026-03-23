#include "funciones.h" //prototipados
#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>


//Emmanuel Guerra Tubequia
//Jose Alejandro Pabón Leal

int main() {
    unsigned char opcionMenu;
    bool programaActivo = true;

    // 1. Bucle Principal del Programa (Menú)
    while (programaActivo) {
        system("cls");
        cout << "\n=======================================================\n";
        cout << "  _____     _       _      __  __           _   _ \n";
        cout << " |_   _|___| |_ _ __(_)___|  \\/  | __ _ ___| |_ ___ _ __ \n";
        cout << "   | |/ _ \\ __| '__| / __| |\\/| |/ _` / __| __/ _ \\ '__|\n";
        cout << "   | |  __/ |_| |  | \\__ \\ |  | | (_| \\__ \\ ||  __/ |   \n";
        cout << "   |_|\\___|\\__|_|  |_|___/_|  |_|\\__,_|___/\\__\\___|_|   \n";
        cout << "=======================================================\n";
        cout << "               1. JUGAR NUEVA PARTIDA\n";
        cout << "               2. SALIR\n";
        cout << "=======================================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcionMenu;

        if (opcionMenu == '2') {
            programaActivo = false;
            cout << "Saliendo del programa... Adios!!!" << endl;
            continue;
        } else if (opcionMenu != '1') {
            cout << "Opcion no valida. Presione una tecla para reintentar...";
            system("pause>nul");
            continue;
        }

        // --- INICIO DE UNA PARTIDA ---
        unsigned int filas, columnas, columnas_bits, dimensiony = 0, dimensionx = 0, filaFicha = 0;
        int posicionx = 0;
        unsigned char** ptr = nullptr;
        unsigned char** tablerofijo = nullptr;
        bool juegoActivo = true;

        system("cls");
        cout << "\n--- Configuracion del Tablero ---\n";

        // 1. VALIDACIÓN DE FILAS (Mínimo 7)
        while (true) {
            cout << "Ingrese numero de filas (Minimo 7): ";

            // Verificamos si es un número y si es mayor o igual a 7
            if (cin >> filas && filas >= 7 && filas <= 20) {
                break; // Única forma de salir del bucle
            } else {
                cout << "[ERROR] Entrada no valida. Debe ser un NUMERO mayor o igual a 7 y menor a 20.\n";
                cin.clear(); // Limpia el error por si metieron letras
                cin.ignore(1000, '\n'); // Descarta el resto de la línea
            }
        }

        // 2. VALIDACIÓN DE COLUMNAS (Múltiplo de 8 y > 0)
        while (true) {
            cout << "Ingrese numero de columnas (Multiplo de 8, ej: 8, 16, 24): ";

            if (cin >> columnas && columnas > 0 && columnas % 8 == 0 && columnas <= 24) {
                break; // Única forma de salir
            } else {
                cout << "[ERROR] Debe ser un NUMERO multiplo de 8 (8, 16, 32...) Y menor a 24.\n";
                cin.clear();
                cin.ignore(1000, '\n');
            }
        }

        columnas_bits = columnas / 8;

        // Reservar memoria al iniciar la partida
        ptr = new unsigned char*[filas];
        tablerofijo = new unsigned char*[filas];
        crearMatriz(ptr, filas, columnas_bits);
        inicializarTablero(ptr, filas, columnas_bits);
        crearMatriz(tablerofijo, filas, columnas_bits);
        inicializarTablero(tablerofijo, filas, columnas_bits);

        // Piezas base
        unsigned char piezas[5][4] = {
            {0b10000000, 0b10000000, 0b10000000, 0b10000000}, // I
            {0b11100000, 0b01000000, 0, 0},                   // T
            {0b11000000, 0b01100000, 0, 0},                   // Z
            {0b10000000, 0b10000000, 0b11000000, 0},          // L
            {0b11000000, 0b11000000, 0, 0}                    // O
        };
        unsigned char piezaActiva[4];

        srand(time(0));
        ingresarFicha(ptr, piezas, columnas, columnas_bits, filaFicha, dimensiony, tablerofijo, juegoActivo, posicionx, dimensionx, piezaActiva);

        // 2. Bucle del Juego (Gameplay)
        while (juegoActivo) {
            system("cls");
            imprimirTablero(ptr, filas, columnas_bits, tablerofijo);

            unsigned char opcionJuego;
            cin >> opcionJuego;

            switch (opcionJuego) {
            case 'S': case 's':
                bajar(ptr, tablerofijo, filaFicha, columnas_bits, filas, piezas, columnas, dimensiony, juegoActivo, posicionx, dimensionx, piezaActiva);
                break;

            case 'D': case 'd':
                MoverDerecha(ptr, tablerofijo, filaFicha, columnas_bits, filas, dimensiony, posicionx, dimensionx);
                break;

            case 'A': case 'a':
                MoverIzquierda(ptr, tablerofijo, filaFicha, columnas_bits, dimensiony, posicionx);
                break;

            case 'W':
            case 'w': {
                unsigned char copiaPieza[4];
                for(int i=0; i<4; i++) copiaPieza[i] = piezaActiva[i];
                unsigned int copiaDimX = dimensionx, copiaDimY = dimensiony;

                rotarDerecha(copiaPieza, copiaDimX, copiaDimY);

                if (esPosibleRotar(copiaPieza, copiaDimX, copiaDimY, filaFicha, posicionx, tablerofijo, filas, columnas_bits)) {
                    //borramos la pieza vieja
                    for (unsigned int i = 0; i < dimensiony; i++) {
                        int b = posicionx / 8;
                        ptr[filaFicha + i][b] = 0;
                        if ((posicionx % 8) > 0 && (b + 1) < (int)columnas_bits)
                            ptr[filaFicha + i][b + 1] = 0;
                    }

                    // aplicamos cambios
                    for(int i=0; i<4; i++) piezaActiva[i] = copiaPieza[i];
                    dimensionx = copiaDimX;
                    dimensiony = copiaDimY;

                    // dibujamos la pieza
                    for (unsigned int i = 0; i < dimensiony; i++) {
                        int b = posicionx / 8;
                        int desp = posicionx % 8;
                        ptr[filaFicha + i][b] |= (piezaActiva[i] >> desp);
                        if (desp > 0 && (b + 1) < (int)columnas_bits) {
                            ptr[filaFicha + i][b + 1] |= (piezaActiva[i] << (8 - desp));
                        }
                    }
                }
                break;
            }

            case 'Q': case 'q':
                juegoActivo = false;
                cout << "\nPartida finalizada por el usuario.\n";
                break;

            default:
                cout << "[!] Opcion no valida. Usa A, S, D, W o Q.\n";
                system("pause");
                break;
            }
        }
        if (!juegoActivo) {
            cout << "\n*** GAME OVER ***\n";
            system("pause");
        }

        // --- LIMPIEZA DE MEMORIA AL FINALIZAR PARTIDA ---
        for (unsigned int i = 0; i < filas; i++) {
            delete[] ptr[i];
            delete[] tablerofijo[i];
        }
        delete[] ptr;
        delete[] tablerofijo;
        // La memoria se libera y volvemos al menu principal
    }

    return 0;
}

