#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>

// PROTOTIPOS
void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void crearMatriz(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas,unsigned int columnas_bits);
void bajar(unsigned char** ptr,unsigned int &filaFicha,unsigned int columnas_bits,unsigned int filas);
void MoverDerecha(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas);
void MoverIzquierda(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits);
int main()
{
    unsigned int filas,columnas,columnas_bits,filaFicha = 0;// filas
    unsigned char** ptr;
    bool juegoActivo = true;
    cout << "numero del tamaño de las filas: " << endl;
    cin >> filas;
    cout << "tamaño de las columnas" << endl;
    cin >> columnas;
    columnas_bits = columnas/8;
    ptr = new unsigned char*[filas];
    unsigned char piezas[5][4] = // creo una matriz con todas las fichas a nivel de columnas_bits
        {
            {0b11110000,0,0,0},      // I
            {0b11100000,0b01000000,0,0}, // T
            {0b11000000,0b00110000,0,0}, // Z
            {0b11100000,0b10000000,0,0}, // L
            {0b11000000,0b11000000,0,0}  // O
        };

    //inicializo la aleatoridad
    srand(time(0));
    // Aqui inicializamos y creamos la matriz
    crearMatriz(ptr,filas,columnas_bits);
    inicializarTablero(ptr,filas,columnas_bits);
    // Agregamos la primer ficha
    ingresarFicha(ptr, piezas, columnas, columnas_bits);

    cout << "\n=======================================================\n";
    cout << "  _____     _       _      __  __           _   _ \n";
    cout << " |_   _|___| |_ _ __(_)___|  \\/  | __ _ ___| |_ ___ _ __ \n";
    cout << "   | |/ _ \\ __| '__| / __| |\\/| |/ _` / __| __/ _ \\ '__|\n";
    cout << "   | |  __/ |_| |  | \\__ \\ |  | | (_| \\__ \\ ||  __/ |   \n";
    cout << "   |_|\\___|\\__|_|  |_|___/_|  |_|\\__,_|___/\\__\\___|_|   \n";
    cout << "=======================================================\n";
    while(juegoActivo){
        system("cls"); // limpia la pantalla
        // Mostrar el tablero en cada iteración
        imprimirTablero(ptr, filas, columnas_bits);
        unsigned char opcion;
        cout << "ingrese la opcion: ";
        cin >> opcion;

        switch(opcion) {
        case 'S':
        case 's':
            bajar(ptr,filaFicha,columnas_bits,filas);
            break;
        case 'Q':
        case 'q':
            juegoActivo = false;
            cout << "Gracias por jugar!!" << endl;
            break;
        case 'D':
        case 'd':
            MoverDerecha(ptr,filaFicha,columnas_bits,filas);
            break;
        case 'A':
        case 'a':
            MoverIzquierda(ptr,filaFicha,columnas_bits);
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;


        }



    }
    // liberar memoria
    for(unsigned int i = 0; i < filas; i++)
        delete[] ptr[i];

    delete[] ptr;


    return 0;
}


void crearMatriz(unsigned char** ptr, unsigned int filas, unsigned int columnas){

    for(unsigned int i = 0; i < filas ; i++){

        ptr[i] = new unsigned char[columnas];

    }
}

void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas){

    for(unsigned int i = 0; i < filas; i++){
        for(unsigned int j = 0; j < columnas; j++){

            ptr[i][j] = 0;

        }
    }
}

void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas){

    for(unsigned int i = 0; i < filas; i++){
        cout << "|";
        for(unsigned int j = 0; j < columnas; j++){


            for(int bit = 7; bit >=0; bit--){ // es un int normal porque en la ultima iteracion queda 0-1 y ahi se vuelve un overflow y nunca acabaria, un ciclo infinito

                if((ptr[i][j] >> bit) & 1){ // ptr[i][j] lo que significa es tomar el char de esa posicion y tomar los valores bit a bit

                    cout << " [ ] ";
                }
                else{

                    cout << "  .  ";
                }

            }
        }
        cout << "|" << endl;
    }
    cout << "Accion: [A]Izq  [D]Der [S]Bajar [W]Rotar [Q]Salir: ";
}

void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas, unsigned int columnas_bits)
{
    unsigned int ficha,mitad = (columnas/2) - 2, contador_mitad = 0; // cuadrado

    ficha = rand() % 5;

    for(unsigned int i = 0; i < 4; i++)
    {
        if(piezas[ficha][i] != 0)
        {
            ptr[i][0] |= piezas[ficha][i];
        }
    }
    //ingresar en el medio
    for(unsigned int desplazamiento = 0; desplazamiento < mitad; desplazamiento++)//cuantas veces mover la ficha
    {
        for(unsigned int i = 0; i < 4; i++) // recorrer filas de la ficha
        {
            for(int j = columnas_bits - 1; j >= 0; j--)
            {
                unsigned char llevar_bit = (ptr[i][j] & 1) << 7;

                ptr[i][j] >>= 1;

                if(j + 1 < columnas_bits){
                    ptr[i][j+1] |= llevar_bit;
                }
            }
        }
    }

}

void bajar(unsigned char** ptr,unsigned int &filaFicha,unsigned int columnas_bits,unsigned int filas){


    // verifico sie el movimiento es un movimiento posible
    if (filaFicha + 4 >= filas){
        cout << "movimiento imposible te excediste del tablero estas excediendo los limites de altura del tablero";
    }

    else{

        // Mover la ficha hacia abajo
        for(int i = 3; i >= 0; i--) {
            for(unsigned int j = 0; j < columnas_bits; j++) {

                ptr[filaFicha + i + 1][j] = ptr[filaFicha + i][j];
                ptr[filaFicha + i][j] = 0;
            }
        }

        filaFicha++;
    }
}

void MoverDerecha(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas){

    //aca solo hace una iteracion
    for(unsigned int i = 0; i < 4; i++) // recorrer filas de la ficha
    {
        for(int j = columnas_bits - 1; j >= 0; j--)
        {
            //necesito condiciones para que no haga overflow
            unsigned char llevar_bit = (ptr[i + filaFicha][j] & 1) << 7;

            ptr[i+filaFicha][j] >>= 1;

            if(j + 1 < columnas_bits){
                ptr[i + filaFicha][j+1] |= llevar_bit ;
                // posicionx++;
            }

        }
    }
}

void MoverIzquierda(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits){

    //aca solo hace una iteracion
    for(unsigned int i = 0; i < 4; i++) // recorrer filas de la ficha
    {
        for(int j = 0; j < columnas_bits; j++)
        {
            //necesito condiciones para que no haga overflow
            unsigned char llevar_bit = (ptr[i + filaFicha][j] & 128) >> 7;

            ptr[i+filaFicha][j] <<= 1;

            if(j > 0){
                ptr[i + filaFicha][j-1] |= llevar_bit;
            }

        }
    }
}



