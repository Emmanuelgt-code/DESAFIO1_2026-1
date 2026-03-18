#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// --- PROTOTIPOS ---
void crearMatriz(unsigned char**& ptr, unsigned int filas, unsigned int columnas_bits);
void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas_bits);
void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas_bits);
void liberarTablero(unsigned char** ptr, unsigned int filas);
bool hayColision(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab);
void fijarFicha(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, bool colocar);
void rotar90(unsigned char* piezaBits);

// NUEVA FUNCIÓN: Detecta y borra líneas completas
void verificarLineas(unsigned char** tablero, int filas, int columnas_bits);

int main() {
    srand(time(0));
    unsigned int filas, columnas, columnas_bits;
    unsigned char** ptr = nullptr;
    int opcion;

    unsigned char piezas[5][4] = {
        {0b11110000, 0, 0, 0},           // I
        {0b11100000, 0b01000000, 0, 0},  // T
        {0b11000000, 0b01100000, 0, 0},  // Z
        {0b11100000, 0b10000000, 0, 0},  // L
        {0b11000000, 0b11000000, 0, 0}   // O
    };

    cout << "Ingrese FILAS: "; cin >> filas;
    cout << "Ingrese COLUMNAS (8, 16...): "; cin >> columnas;
    columnas_bits = columnas / 8;

    crearMatriz(ptr, filas, columnas_bits);
    inicializarTablero(ptr, filas, columnas_bits);

    cout << "\n=======================================================\n";
    cout << "  _____     _       _      __  __           _   _ \n";
    cout << " |_   _|___| |_ _ __(_)___|  \\/  | __ _ ___| |_ ___ _ __ \n";
    cout << "   | |/ _ \\ __| '__| / __| |\\/| |/ _` / __| __/ _ \\ '__|\n";
    cout << "   | |  __/ |_| |  | \\__ \\ |  | | (_| \\__ \\ ||  __/ |   \n";
    cout << "   |_|\\___|\\__|_|  |_|___/_|  |_|\\__,_|___/\\__\\___|_|   \n";
    cout << "=======================================================\n";
    cout << "\n1. JUGAR\n2. SALIR\nSeleccione: "; cin >> opcion;

    if (opcion == 1) {
        bool juegoActivo = true;
        int idPieza = rand() % 5;
        unsigned char piezaActual[4];
        for(int i=0; i<4; i++) piezaActual[i] = piezas[idPieza][i];
        int px = (columnas - 4) / 2;
        int py = 0;

        while (juegoActivo) {
            fijarFicha(ptr, piezaActual, px, py, columnas, true);
            imprimirTablero(ptr, filas, columnas_bits);
            cout << "\nAccion [A,D,S,W,Q]: ";

            fijarFicha(ptr, piezaActual, px, py, columnas, false);

            char tecla; cin >> tecla;
            if (tecla == 'q' || tecla == 'Q') break;
            else if (tecla == 'a' || tecla == 'A') {
                if (!hayColision(ptr, piezaActual, px - 1, py, columnas, filas)) px--;
            }
            else if (tecla == 'd' || tecla == 'D') {
                if (!hayColision(ptr, piezaActual, px + 1, py, columnas, filas)) px++;
            }
            else if (tecla == 'w' || tecla == 'W') {
                unsigned char copia[4];
                for(int i=0; i<4; i++) copia[i] = piezaActual[i];
                rotar90(copia);
                if (!hayColision(ptr, copia, px, py, columnas, filas))
                    for(int i=0; i<4; i++) piezaActual[i] = copia[i];
            }
            else if (tecla == 's' || tecla == 'S') {
                if (hayColision(ptr, piezaActual, px, py + 1, columnas, filas)) {
                    fijarFicha(ptr, piezaActual, px, py, columnas, true);

                    // --- AQUÍ REVISAMOS LAS LÍNEAS ---
                    verificarLineas(ptr, filas, columnas_bits);

                    idPieza = rand() % 5;
                    for(int i=0; i<4; i++) piezaActual[i] = piezas[idPieza][i];
                    px = (columnas - 4) / 2;
                    py = 0;

                    if (hayColision(ptr, piezaActual, px, py, columnas, filas)) {
                        juegoActivo = false;
                        cout << "\n¡GAME OVER!" << endl;
                    }
                } else {
                    py++;
                }
            }
        }
    }
    liberarTablero(ptr, filas);
    return 0;
}

// --- LÓGICA DE LIMPIEZA ---
void verificarLineas(unsigned char** tablero, int filas, int columnas_bits) {
    // Escaneamos de abajo hacia arriba
    for (int i = filas - 1; i >= 0; i--) {
        bool filaLlena = true;
        for (int j = 0; j < columnas_bits; j++) {
            // Un byte lleno es 11111111 en binario (255 en decimal)
            if (tablero[i][j] != 0xFF) {
                filaLlena = false;
                break;
            }
        }

        if (filaLlena) {
            // Desplazar todo lo de arriba hacia abajo
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < columnas_bits; j++) {
                    tablero[k][j] = tablero[k - 1][j];
                }
            }
            // Limpiar la fila superior (ahora vacía)
            for (int j = 0; j < columnas_bits; j++) {
                tablero[0][j] = 0;
            }
            // Volvemos a revisar la misma fila 'i' porque la de arriba bajó
            i++;
        }
    }
}

// --- RESTO DE FUNCIONES (IGUALES A LAS ANTERIORES) ---
void fijarFicha(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, bool colocar) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piezaBits[i] & (0x80 >> j)) {
                int f_real = py + i;
                int c_real = px + j;
                if (f_real >= 0 && f_real < 1000 && c_real >= 0 && c_real < anchoTab) {
                    int byteIdx = c_real / 8;
                    int bitPos = 7 - (c_real % 8);
                    if (colocar) tablero[f_real][byteIdx] |= (1 << bitPos);
                    else tablero[f_real][byteIdx] &= ~(1 << bitPos);
                }
            }
        }
    }
}

bool hayColision(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piezaBits[i] & (0x80 >> j)) {
                int f_real = py + i;
                int c_real = px + j;
                if (f_real >= altoTab || c_real < 0 || c_real >= (int)anchoTab) return true;
                if (f_real >= 0) {
                    int byteIdx = c_real / 8;
                    int bitPos = 7 - (c_real % 8);
                    if ((tablero[f_real][byteIdx] >> bitPos) & 1) return true;
                }
            }
        }
    }
    return false;
}

void rotar90(unsigned char* piezaBits) {
    bool temporal[4][4] = {false};
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            if(piezaBits[i] & (0x80 >> j)) temporal[i][j] = true;
    unsigned char nueva[4] = {0,0,0,0};
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if(temporal[i][j]) nueva[j] |= (0x80 >> (3 - i));
        }
    }
    for(int i=0; i<4; i++) piezaBits[i] = nueva[i];
}

void crearMatriz(unsigned char**& ptr, unsigned int filas, unsigned int columnas_bits) {
    ptr = new unsigned char*[filas];
    for (unsigned int i = 0; i < filas; i++) ptr[i] = new unsigned char[columnas_bits];
}

void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas_bits) {
    for (unsigned int i = 0; i < filas; i++)
        for (unsigned int j = 0; j < columnas_bits; j++) ptr[i][j] = 0;
}

void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas_bits) {
    system("cls");
    for (unsigned int i = 0; i < filas; i++) {
        cout << "|";
        for (unsigned int j = 0; j < columnas_bits; j++) {
            for (int bit = 7; bit >= 0; bit--) {
                if ((ptr[i][j] >> bit) & 1) cout << "[]";
                else cout << " .";
            }
        }
        cout << "|" << endl;
    }
}

void liberarTablero(unsigned char** ptr, unsigned int filas) {
    for (unsigned int i = 0; i < filas; i++) delete[] ptr[i];
    delete[] ptr;
}
