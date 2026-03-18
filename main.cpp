#include <iostream>

using namespace std;

// --- PROTOTIPOS DE FUNCIONES ---
void liberarTablero(unsigned char** tablero, int alto);
unsigned char** crearTablero(int ancho, int alto);
bool hayColision(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab);
void intentarMover(unsigned char** tablero, unsigned char* piezaBits, int &px, int &py, int dx, int dy, int anchoTab, int altoTab);
void rotar(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab);

// --- DESARROLLO DE FUNCIONES ---

void liberarTablero(unsigned char** tablero, int alto) {
    if (tablero != nullptr) {
        for (int i = 0; i < alto; ++i) {
            delete[] tablero[i];
        }
        delete[] tablero;
    }
}

unsigned char** crearTablero(int ancho, int alto) {
    unsigned char** tablero = new unsigned char*[alto];
    int columnasBytes = ancho / 8;

    for (int i = 0; i < alto; ++i) {
        tablero[i] = new unsigned char[columnasBytes];
        for (int j = 0; j < columnasBytes; ++j) {
            tablero[i][j] = 0;
        }
    }
    return tablero;
}

// Ahora recibe el arreglo de bits y las coordenadas por separado
bool hayColision(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab) {
    for (int i = 0; i < 4; ++i) {
        if (piezaBits[i] == 0) continue;

        int filaTab = py + i;
        if (filaTab >= altoTab) return true; // Colisión con suelo

        // Alineación bitwise
        unsigned int bitsPiezaOcupados = (unsigned int)piezaBits[i] << (anchoTab - 4 - px);
        unsigned int bitsTablero = tablero[filaTab][0];

        if ((bitsTablero & bitsPiezaOcupados) != 0) return true;
        if (px < 0 || (px + 4) > anchoTab) return true; // Paredes
    }
    return false;
}

void intentarMover(unsigned char** tablero, unsigned char* piezaBits, int &px, int &py, int dx, int dy, int anchoTab, int altoTab) {
    if (!hayColision(tablero, piezaBits, px + dx, py + dy, anchoTab, altoTab)) {
        px += dx;
        py += dy;
    }
}

void rotar(unsigned char** tablero, unsigned char* piezaBits, int px, int py, int anchoTab, int altoTab) {
    unsigned char nuevaForma[4] = {0, 0, 0, 0};

    // Lógica de rotación bit a bit
    for (int f = 0; f < 4; ++f) {
        for (int c = 0; c < 4; ++c) {
            if (piezaBits[f] & (1 << (3 - c))) {
                nuevaForma[c] |= (1 << f);
            }
        }
    }

    if (!hayColision(tablero, nuevaForma, px, py, anchoTab, altoTab)) {
        for(int i = 0; i < 4; i++) piezaBits[i] = nuevaForma[i];
    }
}

int main() {
    int opcion = 0;
    unsigned char** tablero = nullptr;
    int alto = 0, ancho = 0;

    // Pieza de ejemplo (Tetrimino T)
    unsigned char piezaActualBits[4] = {0x4, 0xE, 0x0, 0x0};
    int piezaX = 2;
    int piezaY = 0;

    while (opcion != 3) {
        cout << "\n=======================================================\n";
        cout << "  _____     _       _      __  __           _   _ \n";
        cout << " |_   _|___| |_ _ __(_)___|  \\/  | __ _ ___| |_ ___ _ __ \n";
        cout << "   | |/ _ \\ __| '__| / __| |\\/| |/ _` / __| __/ _ \\ '__|\n";
        cout << "   | |  __/ |_| |  | \\__ \\ |  | | (_| \\__ \\ ||  __/ |   \n";
        cout << "   |_|\\___|\\__|_|  |_|___/_|  |_|\\__,_|___/\\__\\___|_|   \n";
        cout << "=======================================================\n";
        cout << "1. Jugar Modalidad Facil (8x8)\n";
        cout << "2. Jugar Modalidad Dificil (16x16)\n";
        cout << "3. Salir\n";
        cout << "Seleccione: ";

        cin >> opcion;


        if (!(cin >> opcion)) { // Si el usuario mete una letra por error
            cin.clear();
            while (cin.get() != '\n'); // Limpia el error
            continue;
        }

        if (opcion == 1 || opcion == 2) {
            ancho = (opcion == 1) ? 8 : 16;
            alto = ancho;
            tablero = crearTablero(ancho, alto);

            bool enJuego = true;
            char tecla;

            while (enJuego) {
                // AQUÍ DEBERÍAS LLAMAR A TU FUNCIÓN DE IMPRIMIR
                cout << "\n--- TABLERO (" << ancho << "x" << alto << ") ---" << endl;
                cout << "Ficha en: X=" << piezaX << " Y=" << piezaY << endl;
                cout << "Controles: A(Izq), D(Der), S(Bajar), W(Rotar), Q(Salir)" << endl;
                cout << "Presione una tecla y Enter: ";

                cin >> tecla;

                if (tecla == 'q' || tecla == 'Q') {
                    enJuego = false;
                } else if (tecla == 'a' || tecla == 'A') {
                    intentarMover(tablero, piezaActualBits, piezaX, piezaY, -1, 0, ancho, alto);
                } else if (tecla == 'd' || tecla == 'D') {
                    intentarMover(tablero, piezaActualBits, piezaX, piezaY, 1, 0, ancho, alto);
                } else if (tecla == 's' || tecla == 'S') {
                    intentarMover(tablero, piezaActualBits, piezaX, piezaY, 0, 1, ancho, alto);
                } else if (tecla == 'w' || tecla == 'W') {
                    rotar(tablero, piezaActualBits, piezaX, piezaY, ancho, alto);
                }

                // Si la pieza llega al suelo o choca al bajar, podrías fijarla aquí
            }

            liberarTablero(tablero, alto);
            tablero = nullptr;
            cout << "\nPartida finalizada. Volviendo al menu...\n";
        }
    }
    return 0;
}
