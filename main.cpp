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
void verificarLineas(unsigned char** tablero, int filas, int columnas_bits);

int main() {
    srand(time(0));
    unsigned int filas, columnas, columnas_bits;
    unsigned char** ptr = nullptr;
    int opcion;

    // Representación binaria de las piezas (4x4 bits)
    unsigned char piezas[5][4] = {
        {0b11110000, 0, 0, 0},           // Línea (I)
        {0b11100000, 0b01000000, 0, 0},  // T
        {0b11000000, 0b01100000, 0, 0},  // Z
        {0b11100000, 0b10000000, 0, 0},  // L
        {0b11000000, 0b11000000, 0, 0}   // Cuadrado (O)
    };

    cout << "Ingrese FILAS: "; cin >> filas;
    cout << "Ingrese COLUMNAS (8, 16, 24...): "; cin >> columnas;

    // Cada 'unsigned char' es un contenedor de 8 celdas (bits)
    columnas_bits = columnas / 8;

    crearMatriz(ptr, filas, columnas_bits);
    inicializarTablero(ptr, filas, columnas_bits);

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

            cout << "\nControles: A(Izq), D(Der), S(Bajar), W(Rotar), Q(Salir)\nAccion: ";

            // Borramos la pieza antes de moverla para que la función de colisión
            // no detecte la propia pieza como un obstáculo.
            fijarFicha(ptr, piezaActual, px, py, columnas, false);

            char tecla; cin >> tecla;
            if (tecla == 'q' || tecla == 'Q') break;

            // --- MOVIMIENTO A LA IZQUIERDA ---
            else if (tecla == 'a' || tecla == 'A') {
                // Simulamos el movimiento: ¿Qué pasaría si restamos 1 a la posición X?
                bool hayObstaculo = hayColision(ptr, piezaActual, px - 1, py, columnas, filas);

                // Si NO hay obstáculo, actualizamos la posición real
                if (!hayObstaculo) {
                    px--;
                }
            }

            // --- MOVIMIENTO A LA DERECHA ---
            else if (tecla == 'd' || tecla == 'D') {
                // Simulamos: ¿Qué pasaría si sumamos 1 a la posición X?
                if (!hayColision(ptr, piezaActual, px + 1, py, columnas, filas)) {
                    px++; // Si el camino está libre, nos movemos
                }
            }

            // --- ROTACIÓN DE LA PIEZA ---
            else if (tecla == 'w' || tecla == 'W') {
                // 1. Creamos una copia de la pieza actual
                unsigned char copia[4];
                for(int i = 0; i < 4; i++) {
                    copia[i] = piezaActual[i];
                }

                // 2. Rotamos esa copia fuera del tablero
                rotar90(copia);

                // 3. Verificamos si esa nueva forma rotada cabe en el espacio actual
                // (Esto evita que la pieza se rote si está pegada a una pared o a otra pieza)
                if (!hayColision(ptr, copia, px, py, columnas, filas)) {
                    // Si cabe, transferimos la forma rotada a la pieza real
                    for(int i = 0; i < 4; i++) {
                        piezaActual[i] = copia[i];
                    }
                }
            }
            else if (tecla == 's' || tecla == 'S') {
                // Intentamos ver si un paso más abajo hay choque
                bool choqueAbajo = hayColision(ptr, piezaActual, px, py + 1, columnas, filas);

                if (choqueAbajo) {
                    // Si choca, la fijamos permanentemente
                    fijarFicha(ptr, piezaActual, px, py, columnas, true);
                    verificarLineas(ptr, filas, columnas_bits);

                    // Nueva pieza al inicio
                    idPieza = rand() % 5;
                    for(int i=0; i<4; i++) piezaActual[i] = piezas[idPieza][i];
                    px = (columnas - 4) / 2;
                    py = 0;

                    // Si al aparecer ya choca, es fin del juego
                    if (hayColision(ptr, piezaActual, px, py, columnas, filas)) {
                        juegoActivo = false;
                        cout << "\n¡GAME OVER!" << endl;
                    }
                } else {
                    py++; // No hay choque, puede bajar
                }
            }
        }
    }
    liberarTablero(ptr, filas);
    return 0;
}

// --- EXPLICACIÓN DE OPERACIONES DE BITS ---

void fijarFicha(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, bool colocar) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            // ¿Hay un bloque en esta posición de la pieza?
            // Desplazamos 10000000 (0x80) a la derecha 'j' veces para comparar
            unsigned char mascaraPieza = (0x80 >> j);
            bool existeBloque = piezaBits[i] & mascaraPieza;

            if (existeBloque) {
                int f_real = py + i;
                int c_real = px + j;

                if (f_real >= 0 && f_real < 1000 && c_real >= 0 && c_real < anchoTab) {
                    // Calculamos en qué "caja" de 8 bits cae la columna
                    int byteIdx = c_real / 8;
                    // Calculamos la posición dentro de esa caja (de izquierda a derecha)
                    int bitPos = 7 - (c_real % 8);
                    unsigned char mascaraTablero = (1 << bitPos);

                    if (colocar)
                        tablero[f_real][byteIdx] |= mascaraTablero; // Encendemos con OR
                    else
                        tablero[f_real][byteIdx] &= ~mascaraTablero; // Apagamos con AND NOT
                }
            }
        }
    }
}

bool hayColision(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            bool pixelPiezaActivo = piezaBits[i] & (0x80 >> j);

            if (pixelPiezaActivo) {
                int f_real = py + i;
                int c_real = px + j;

                // 1. Verificar límites físicos del tablero
                if (f_real >= altoTab || c_real < 0 || c_real >= anchoTab) return true;

                // 2. Verificar si el bit en el tablero ya está encendido (valor 1)
                if (f_real >= 0) {
                    int byteIdx = c_real / 8;
                    int bitPos = 7 - (c_real % 8);

                    // Extraemos el valor del bit: desplazamos a la derecha y aislamos el último bit
                    bool celdaOcupada = (tablero[f_real][byteIdx] >> bitPos) & 1;

                    if (celdaOcupada) return true;
                }
            }
        }
    }
    return false;
}

void verificarLineas(unsigned char** tablero, int filas, int columnas_bits) {
    for (int i = filas - 1; i >= 0; i--) {
        bool filaCompleta = true;

        for (int j = 0; j < columnas_bits; j++) {
            // Si el byte es 11111111 (0xFF), significa que 8 bloques están llenos
            if (tablero[i][j] != 0xFF) {
                filaCompleta = false;
                break;
            }
        }

        if (filaCompleta) {
            // Movemos todo el contenido de las filas superiores hacia abajo
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < columnas_bits; j++) {
                    tablero[k][j] = tablero[k - 1][j];
                }
            }
            // La fila superior queda vacía
            for (int j = 0; j < columnas_bits; j++) tablero[0][j] = 0;

            i++; // Volvemos a revisar la misma posición i porque ahora tiene nuevos datos
        }
    }
}

// --- SOPORTE TÉCNICO ---

void rotar90(unsigned char* piezaBits) {
    bool matriz[4][4] = {false};

    // Paso 1: Convertir bits a matriz booleana (fácil de leer)
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            if(piezaBits[i] & (0x80 >> j)) matriz[i][j] = true;

    // Paso 2: Rotar 90 grados y re-empaquetar en bits
    for(int i=0; i<4; i++) piezaBits[i] = 0; // Limpiar pieza

    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if(matriz[i][j]) {
                // El punto (i, j) rotado es (j, 3-i)
                piezaBits[j] |= (0x80 >> (3 - i));
            }
        }
    }
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
                bool activo = (ptr[i][j] >> bit) & 1;
                cout << (activo ? "[]" : " .");
            }
        }
        cout << "|" << endl;
    }
}

void liberarTablero(unsigned char** ptr, unsigned int filas) {
    for (unsigned int i = 0; i < filas; i++) delete[] ptr[i];
    delete[] ptr;
}
